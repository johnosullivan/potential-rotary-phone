#include <iostream>
#include "parser.h"

using namespace core::parser;

Parser::Parser(lexer::Lexer* lex) : lexer(lex) {
    ////std::cout << "lexer::Lexer" << std::endl;

    current_token = lex->next_token();
    next_token    = lex->next_token();

    //std::cout << "current_token = " << current_token.value << std::endl;
    //std::cout << "next_token = " << next_token.value << std::endl;
}

Parser::Parser(lexer::Lexer* lex, unsigned int tokens) : lexer(lex) {
    next_token = lex->next_token();
    for(unsigned int i = 0; i < tokens; i++)
        consume_token();
}

void Parser::consume_token() {
    //std::cout << "next_token = " << next_token.value << std::endl;

    current_token = next_token;
    next_token = lexer->next_token();
}

ASTProgramNode* Parser::parse_program() {
    //std::cout << "parse_program" << std::endl;

    auto statements = new std::vector<ASTNode*>;

    while(current_token.type != lexer::TK_EOF){
        auto statement = parse_statement();
        statements->push_back(statement);
        consume_token();
    }

    return new ASTProgramNode(*statements);
}

ASTDeclarationNode* Parser::parse_declaration_statement() {
    //std::cout << "parse_declaration_statement" << std::endl;

    // Node attributes
    TYPE type;
    std::string identifier;
    ASTExprNode* expr;
    unsigned int line_number;

    // Determine line number
    line_number = current_token.line_number;

    // Consume identifier
    consume_token();
    if(current_token.type != lexer::TK_IDENTIFIER)
        throw std::runtime_error("expected variable name after 'var' on line "
                                 + std::to_string(current_token.line_number) + ".");
    identifier = current_token.value;

    consume_token();
    if(current_token.type != lexer::TK_COLON)
        throw std::runtime_error("expected ':' after " + identifier + " on line "
                                 + std::to_string(current_token.line_number) + ".");

    consume_token();
    type = parse_type(identifier);

    consume_token();
    if(current_token.type != lexer::TK_EQUALS)
        throw std::runtime_error("expected assignment operator '=' for " + identifier + " on line "
                                 + std::to_string(current_token.line_number) + ".");

    // Parse the right hand side
    expr = parse_expression();

    /*consume_token();
    if(current_token.type != lexer::TK_SEMICOLON)
        throw std::runtime_error("Expected ';' after assignment of " + identifier + " on line "
                                 + std::to_string(current_token.line_number) + ".");*/

    // Create ASTExpressionNode to return
    return new ASTDeclarationNode(type, identifier, expr, line_number);
}

ASTStatementNode* Parser::parse_statement() {
    //std::cout << "parse_statement" << std::endl;

    switch(current_token.type){
        case lexer::TK_VAR:
            return parse_declaration_statement();
        case lexer::TK_IDENTIFIER:
            return parse_assignment_statement();
        default:
            throw std::runtime_error("Invalid statement starting with '" +
                                     current_token.value
                                     + "' encountered on line " +
                                     std::to_string(current_token.line_number) + ".");
    }
}

ASTExprNode* Parser::parse_simple_expression() {
    //std::cout << "parse_simple_expression" << std::endl;
    //std::cout << "consume_token: " << current_token.value << std::endl;

    ASTExprNode *term = parse_term();
    unsigned int line_number = current_token.line_number;

    if(next_token.type == lexer::TK_ADDITIVE_OP) {
        consume_token();
        return new ASTBinaryExprNode(current_token.value, term, parse_simple_expression(), line_number);
    }

    return term;
}

ASTExprNode* Parser::parse_term() {
    //std::cout << "parse_term" << std::endl;

    ASTExprNode *factor = parse_factor();
    unsigned int line_number = current_token.line_number;



    return factor;
}

ASTExprNode* Parser::parse_factor() {
    //std::cout << "parse_factor" << std::endl;

    consume_token();

    //std::cout << "parse_factor_current_token_value " << current_token.value << std::endl;

    // Determine line number
    unsigned int line_number = current_token.line_number;

    switch(current_token.type){
        // Literal Cases
        case lexer::TK_INT:
            return new ASTLiteralNode<int>(std::stoi(current_token.value), line_number);
        case lexer::TK_FLOAT:
            return new ASTLiteralNode<float>(std::stof(current_token.value), line_number);
        case lexer::TK_IDENTIFIER:
            return new ASTIdentifierNode(current_token.value, line_number);
        default:
            throw std::runtime_error("expected expression on line "
                                     + std::to_string(current_token.line_number) + ".");

    }

}

ASTAssignmentNode* Parser::parse_assignment_statement() {
    // Node attributes
    std::string identifier;
    ASTExprNode* expr;

    unsigned int line_number = current_token.line_number;
    identifier = current_token.value;

    consume_token();
    if(current_token.type != lexer::TK_EQUALS)
        throw std::runtime_error("Expected assignment operator '=' after " + identifier + " on line "
                                 + std::to_string(current_token.line_number) + ".");

    // Parse the right hand side
    expr = parse_expression();

    consume_token();
    if(current_token.type != lexer::TK_SEMICOLON)
        throw std::runtime_error("Expected ';' after assignment of " + identifier + " on line "
                                 + std::to_string(current_token.line_number) + ".");

    return new ASTAssignmentNode(identifier, expr, line_number);
}

ASTExprNode* Parser::parse_expression() {
    //std::cout << "parse_expression" << std::endl;

    ASTExprNode *simple_expr = parse_simple_expression();
    unsigned int line_number = current_token.line_number;

    return simple_expr;
}

TYPE Parser::parse_type(std::string& identifier) {
    switch(current_token.type){
        case lexer::TK_INT_TYPE:
            return INT;
        case lexer::TK_FLOAT_TYPE:
            return FLOAT;
        default:
            throw std::runtime_error("expected type for " + identifier + " after ':' on line "
                                     + std::to_string(current_token.line_number) + ".");
    }
}
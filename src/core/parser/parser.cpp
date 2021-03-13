#include <iostream>
#include "parser.h"

using namespace core::parser;

Parser::Parser(lexer::Lexer* lex) : lexer(lex) {
    ////std::cout << "lexer::Lexer" << std::endl;

    current_token = lex->next_token();
    next_token    = lex->next_token();

    ////std::cout << "current_token = " << current_token.value << std::endl;
    ////std::cout << "next_token = " << next_token.value << std::endl;
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
    auto statements = new std::vector<ASTNode*>;

    //std::cout << "parse_program " << std::endl;

    while(current_token.type != lexer::TK_EOF){
        auto statement = parse_statement();
        //std::cout << "statement = " << statement << std::endl;
        statements->push_back(statement);
        consume_token();
    }

    return new ASTProgramNode(*statements);
}

ASTStatementNode* Parser::parse_statement() {
    ////std::cout << current_token.value << std::endl;
    ////std::cout << current_token.type << std::endl;

    switch(current_token.type){
        default:
            throw std::runtime_error("Invalid statement starting with '" +
                                     current_token.value
                                     + "' encountered on line " +
                                     std::to_string(current_token.line_number) + ".");
    }
}

ASTExprNode* Parser::parse_simple_expression() {
    //////std::cout << "parse_simple_expression_current_token.type " << current_token.type << std::endl;

    ASTExprNode *term = parse_term();
    unsigned int line_number = current_token.line_number;

    if(next_token.type == lexer::TK_ADDITIVE_OP) {
        consume_token();
        return new ASTBinaryExprNode(current_token.value, term, parse_simple_expression(), line_number);
    }

    return term;
}

ASTExprNode* Parser::parse_term() {
    ASTExprNode *factor = parse_factor();
    unsigned int line_number = current_token.line_number;


    return factor;
}

ASTExprNode* Parser::parse_factor() {
    consume_token();

    // Determine line number
    unsigned int line_number = current_token.line_number;

    //////std::cout << "parse_factor_current_token.type " << current_token.type << std::endl;

    switch(current_token.type){
        // Literal Cases
        case lexer::TK_INT:
            return new ASTLiteralNode<int>(std::stoi(current_token.value), line_number);
        default:
            throw std::runtime_error("Expected expression on line "
                                     + std::to_string(current_token.line_number) + ".");

    }

}

ASTExprNode* Parser::parse_expression() {
    ASTExprNode *simple_expr = parse_simple_expression();
    unsigned int line_number = current_token.line_number;

    return simple_expr;
}

TYPE Parser::parse_type(std::string& identifier) {
    switch(current_token.type){
        case lexer::TK_INT_TYPE:
            return INT;
        default:
            throw std::runtime_error("Expected type for " + identifier + " after ':' on line "
                                     + std::to_string(current_token.line_number) + ".");
    }
}
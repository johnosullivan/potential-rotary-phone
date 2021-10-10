#include <iostream>
#include <algorithm>

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

std::pair<std::string, TYPE>* Parser::parse_param() {

    std::string identifier;
    TYPE type;

    // Make sure current token is identifier
    if(current_token.type != lexer::TK_IDENTIFIER)
        throw std::runtime_error("Expected variable name in function definition on line "
                                 + std::to_string(current_token.line_number) + ".");
    identifier = current_token.value;

    // Consume ':'
    consume_token();

    if(current_token.type != lexer::TK_COLON)
        throw std::runtime_error("Expected ':' after '" + identifier + "' on line "
                                 + std::to_string(current_token.line_number) + ".");

    // Consume type
    consume_token();
    type = parse_type(identifier);

    return new std::pair<std::string, TYPE>(identifier, type);

};

std::vector<ASTExprNode*>* Parser::parse_call_params() {
    auto parameters = new std::vector<ASTExprNode*>;

    parameters->push_back(parse_expression());
    consume_token();

    // If there are more
    while(current_token.type == lexer::TK_COMMA) {
        parameters->push_back(parse_expression());
        consume_token();
    }

    return parameters;
}

ASTFuncNode* Parser::parse_func() {
    // Node attributes
    std::string identifier;
    std::vector<std::pair<std::string, TYPE>> parameters;
    TYPE type = RES_NONE;
    ASTBlockNode* block;
    unsigned int line_number = current_token.line_number;

    // Consume identifier
    consume_token();

    // Make sure it is an identifier
    if(current_token.type != lexer::TK_IDENTIFIER)
        throw std::runtime_error("Expected function identifier after keyword 'fn' on line "
                                 + std::to_string(current_token.line_number) + ".");

    identifier = current_token.value;

    // Consume '('
    consume_token();
    if(current_token.type != lexer::TK_LEFT_PARENTHESES)
        throw std::runtime_error("Expected '(' after '" + identifier + "' on line "
                                 + std::to_string(current_token.line_number) + ".");

    // Consume ')' or parameters
    consume_token();

    if(current_token.type != lexer::TK_RIGHT_PARENTHESES){

        // Parse first parameter
        parameters.push_back(*parse_param());

        // Consume ',' or ')'
        consume_token();

        while(current_token.type == lexer::TK_COMMA){

            // Consume identifier
            consume_token();

            // Parse parameter
            parameters.push_back(*parse_param());

            // Consume ',' or ')'
            consume_token();
        }

        // Exited while-loop, so token must be ')'
        if(current_token.type != lexer::TK_RIGHT_PARENTHESES)
            throw std::runtime_error("Expected ')' or more parameters on line "
                                     + std::to_string(current_token.line_number) + ".");
    }

    // Consume ':'
    consume_token();

    if(current_token.type != lexer::TK_COLON) {
        std::cout << current_token.get_tk_type_as_string() << std::endl;
        if(current_token.type == lexer::TK_LEFT_CURLY) {
            std::cout << "return none type" << std::endl;
        } else {
            throw std::runtime_error("Expected ':' or '{' after ')' on line " + std::to_string(current_token.line_number) + ".");
        }
    } else {
        // Consume type
        consume_token();
        type = parse_type(identifier);

        // Consume '{'
        consume_token();

        if(current_token.type != lexer::TK_LEFT_CURLY)
            throw std::runtime_error("Expected '{' after function '" + identifier +
                                    "' definition on line "
                                    + std::to_string(current_token.line_number) + ".");
    }

    

    // Parse block
    block = parse_block();
   
    // Return function definition node
    return new ASTFuncNode(identifier, parameters, type, block, 0);
}

ASTBlockNode* Parser::parse_block() {
    auto statements = new std::vector<ASTStatementNode*>;

    // Determine line number
    unsigned int line_number = current_token.line_number;

    // Current token is '{', consume first token of statement
    consume_token();

    // While not reached end of block or end of file
    while(current_token.type != lexer::TK_RIGHT_CURLY &&
          current_token.type != lexer::TK_ERROR &&
          current_token.type != lexer::TK_EOF){

        // Parse the statement
        statements->push_back(parse_statement());

        // Consume first token of next statement
        consume_token();
    }

    // If block ended by '}', return block
    if(current_token.type == lexer::TK_RIGHT_CURLY)
        return new ASTBlockNode(*statements, line_number);

    // Otherwise the user left the block open
    else throw std::runtime_error("Reached end of file while parsing."
                                  " Mismatched scopes.");
}

ASTReturnNode* Parser::parse_return() {

    // Determine line number
    unsigned int line_number = current_token.line_number;

    // Get expression to return
    ASTExprNode* expr = parse_expression();

    // Consume ';' token
    /*consume_token();
    if(current_token.type != lexer::TK_SEMICOLON)
        throw std::runtime_error("Expected ';' after return statement on line "
                                 + std::to_string(current_token.line_number) + ".");*/

    // Return return node
    return new ASTReturnNode(expr, line_number);
}

ASTExprFuncCallNode* Parser::parse_function_call() {
    std::cout << "parse_function_call" << std::endl;
    // current token is the function identifier
    std::string identifier = current_token.value;
    auto *parameters = new std::vector<ASTExprNode*>;
    unsigned int line_number = current_token.line_number;

    consume_token();
    if(current_token.type != lexer::TK_LEFT_PARENTHESES)
        throw std::runtime_error("Expected '(' on line "
                                 + std::to_string(current_token.line_number) + ".");

    // If next token is not right bracket, we have parameters
    if(next_token.type != lexer::TK_RIGHT_PARENTHESES) {
        parameters = parse_call_params();
    } else {
        consume_token();
    }

    // Ensure right close bracket after fetching parameters
    if(current_token.type != lexer::TK_RIGHT_PARENTHESES)
        throw std::runtime_error("Expected ')' on line "
                                 + std::to_string(current_token.line_number)
                                 + " after function parameters.");

    return new ASTExprFuncCallNode(identifier, *parameters, line_number);
}

ASTStatementNode* Parser::parse_statement() {
    std::cout << "parse_statement" << std::endl;
    std::cout << current_token.get_tk_type_as_string() << std::endl;

    switch(current_token.type){
        case lexer::TK_VAR:
            return parse_declaration_statement();
        case lexer::TK_PRINT:
            return parse_std_out_statement();
        /*case lexer::TK_IDENTIFIER: {
            
        }*/
        case lexer::TK_FUNC: 
            return parse_func();
        case lexer::TK_RETURN:
            return parse_return();
        case lexer::TK_LEFT_CURLY:
            return parse_block();
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
        case lexer::TK_STRING: {
            current_token.value.erase(std::remove(current_token.value.begin(), current_token.value.end(), '"'), current_token.value.end());
            return new ASTLiteralNode<std::string>(current_token.value, line_number);
        }
        case lexer::TK_LEFT_PARENTHESES: {
            ASTExprNode *sub_expr = parse_expression();
            consume_token();
            if (current_token.type != lexer::TK_RIGHT_PARENTHESES) {
                throw std::runtime_error("Expected ')' after expression on line " + std::to_string(current_token.line_number) + ".");
            }
            return sub_expr;
        }
        case lexer::TK_IDENTIFIER: {
            if(next_token.type == lexer::TK_LEFT_PARENTHESES) {
                return parse_function_call();
            }
            return new ASTIdentifierNode(current_token.value, line_number);
        }
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

    return new ASTAssignmentNode(identifier, expr, line_number);
}

ASTStdOutNode* Parser::parse_std_out_statement() {
    //std::cout << "parse_std_out_statement" << std::endl;

    unsigned int line_number = current_token.line_number;

    ASTExprNode* expr = parse_expression();

    return new ASTStdOutNode(expr, line_number);
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
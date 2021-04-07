#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../lexer/lexer.h"

namespace core::parser {

    class Parser {
        public:
            explicit Parser(lexer::Lexer*);
            Parser(lexer::Lexer*, unsigned int);

            ASTProgramNode* parse_program();
            ASTExprNode* parse_expression();
        private:
            lexer::Lexer* lexer;
            lexer::Token current_token;
            lexer::Token next_token;

            void consume_token();

            // Statement Nodes
            ASTStatementNode*             parse_statement();
            ASTDeclarationNode*           parse_declaration_statement();

            // Expression Nodes
            ASTExprNode*               parse_simple_expression();
            ASTExprNode*               parse_term();
            ASTExprNode*               parse_factor();

            // Parse Types and parameters
            TYPE parse_type(std::string&);
    };

}

#endif //PARSER_H
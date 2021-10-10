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
            ASTStatementNode*             parse_statement_function_call();
            ASTDeclarationNode*           parse_declaration_statement();
            ASTAssignmentNode*            parse_assignment_statement();
            ASTStdOutNode*                parse_std_out_statement();

            // Expression Nodes
            ASTExprNode*                  parse_simple_expression();
            ASTExprNode*                  parse_term();
            ASTExprNode*                  parse_factor();

            // Function / Return 
            ASTFuncNode*                  parse_func();
            ASTBlockNode*                 parse_block();
            ASTReturnNode*                parse_return();
            ASTExprFuncCallNode*          parse_function_call();

            // Parse Types and parameters
            TYPE parse_type(std::string&);
            std::pair<std::string, TYPE>* parse_param();
            std::vector<ASTExprNode*> *parse_call_params();
    };

}

#endif //PARSER_H
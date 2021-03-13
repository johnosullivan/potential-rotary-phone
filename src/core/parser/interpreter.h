#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include <stack>
#include "visitor.h"
#include "./ast.h"

namespace core::visitor {
    
    class Interpreter : public Visitor {
    public:
        Interpreter();
        ~Interpreter();

        void visit(parser::ASTProgramNode*) override;
        void visit(parser::ASTLiteralNode<int>*) override;
        void visit(parser::ASTBinaryExprNode*) override;

        std::pair<parser::TYPE, value_t> current_expr();

    private:
        parser::TYPE current_expression_type;
        value_t current_expression_value;
    };

    std::string type_str(parser::TYPE);
}

#endif //INTERPRETER_H

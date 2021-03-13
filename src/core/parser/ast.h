#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include "visitor.h"

namespace core::parser {

    // Types
    enum TYPE {INT};

    // Abstract Syntax Tree Nodes
    class ASTNode {
    public:
        virtual void accept(visitor::Visitor*) = 0;
    };

    class ASTStatementNode : public ASTNode {
    public:
        void accept(visitor::Visitor*) override = 0;
    };

    class ASTExprNode : public ASTNode {
    public:
        void accept(visitor::Visitor*) override = 0;
    };

    // Statement Nodes
    class ASTProgramNode : public ASTNode {
    public:
        explicit ASTProgramNode(std::vector<ASTNode*>);
        std::vector<ASTNode*> statements;
        void accept(visitor::Visitor*) override;
    };

    // Expression Nodes
    template <typename T>
    class ASTLiteralNode : public ASTExprNode {
    public:
        ASTLiteralNode(T val, unsigned int line_number) : val(val), code_line_number(code_line_number) {};
        T val;
        unsigned int code_line_number;
        void accept(visitor::Visitor*) override;
    };

    class ASTBinaryExprNode : public ASTExprNode {
    public:
        ASTBinaryExprNode(std::string, ASTExprNode*, ASTExprNode*, unsigned int);
        std::string op;
        ASTExprNode *left;
        ASTExprNode *right;
        unsigned int code_line_number;
        void accept(visitor::Visitor*) override;
    };
}


#endif //AST_H
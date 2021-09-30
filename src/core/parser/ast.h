#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

#include "../../common/visitor.h"

namespace core::parser {

    // Return Types 
    enum TYPE {
        INT         = 0,
        FLOAT       = 1,
        STRING      = 2,
        RES_NONE    = 4
    };

    // Abstract Syntax Tree (AST) Nodes
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

    class ASTAssignmentNode : public ASTStatementNode {
    public:
        ASTAssignmentNode(std::string, ASTExprNode*, unsigned int);
        std::string identifier;
        ASTExprNode *expr;
        unsigned int line_number;
        void accept(visitor::Visitor*) override;
    };

    class ASTDeclarationNode : public ASTStatementNode {
    public:
        ASTDeclarationNode(TYPE, std::string, ASTExprNode*, unsigned int);
        TYPE type;
        std::string identifier;
        ASTExprNode *expr;
        unsigned int line_number;
        void accept(visitor::Visitor*) override;
    };

    class ASTIdentifierNode : public ASTExprNode {
    public:
        explicit ASTIdentifierNode(std::string, unsigned int);
        std::string identifier;
        unsigned int line_number;
        void accept(visitor::Visitor*) override;
    };

    // Expression Nodes
    template <typename T>
    class ASTLiteralNode : public ASTExprNode {
    public:
        ASTLiteralNode(T val, unsigned int code_line_number) : val(val), code_line_number(code_line_number) {};
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

    // Standard Output
    class ASTStdOutNode : public ASTStatementNode {
    public:
        ASTStdOutNode(ASTExprNode*, unsigned int);
        ASTExprNode *expr;
        unsigned int line_number;
        void accept(visitor::Visitor*) override;
    };
}


#endif //AST_H
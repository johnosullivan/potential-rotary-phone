#ifndef VISITOR_H
#define VISITOR_H

#include <string>

namespace core::parser {
    template <typename T> class ASTLiteralNode;

    class ASTProgramNode;
    class ASTBinaryExprNode;
    class ASTDeclarationNode;
    class ASTIdentifierNode;
}

namespace core::visitor {

    typedef struct valueT {
        valueT() : i(0), f(0), b(0), s("") {};
        int i;
        float f;
        bool b;
        std::string s;
    } value_t;

    class Visitor {
        public:
            virtual void visit(parser::ASTProgramNode*) = 0;
            virtual void visit(parser::ASTLiteralNode<int>*) = 0;
            virtual void visit(parser::ASTBinaryExprNode*) = 0;
            virtual void visit(parser::ASTDeclarationNode*) = 0;
            virtual void visit(parser::ASTIdentifierNode*) = 0;
    };
}

#endif
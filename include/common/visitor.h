#ifndef VISITOR_H
#define VISITOR_H

#include <string>

namespace core::parser {
    template <typename T> class ASTLiteralNode;

    class ASTProgramNode;
    class ASTStdOutNode;

    class ASTBinaryExprNode;
    class ASTDeclarationNode;
    class ASTIdentifierNode;
    
    class ASTAssignmentNode;

    class ASTFuncNode;
    class ASTBlockNode;
    class ASTReturnNode;
    class ASTExprFuncCallNode;
}

namespace core::visitor {

    typedef struct VT {
        VT() : i(0), f(0), b(0), s("") {};
        int i;
        float f;
        bool b;
        std::string s;
    } value_t;
    
    class Visitor {
        public:
            virtual void visit(parser::ASTProgramNode*) = 0;
            virtual void visit(parser::ASTStdOutNode*) = 0;

            /* literal node for int, float, string */
            virtual void visit(parser::ASTLiteralNode<int>*) = 0;
            virtual void visit(parser::ASTLiteralNode<float>*) = 0;
            virtual void visit(parser::ASTLiteralNode<std::string>*) = 0;
            virtual void visit(parser::ASTLiteralNode<bool>*) = 0;

            virtual void visit(parser::ASTBinaryExprNode*) = 0;
            virtual void visit(parser::ASTDeclarationNode*) = 0;
            virtual void visit(parser::ASTIdentifierNode*) = 0;
            virtual void visit(parser::ASTAssignmentNode*) = 0;

            /* function define */
            virtual void visit(parser::ASTFuncNode*) = 0;
            virtual void visit(parser::ASTBlockNode*) = 0;
            virtual void visit(parser::ASTReturnNode*) = 0;
            virtual void visit(parser::ASTExprFuncCallNode*) = 0;
    };
}

#endif
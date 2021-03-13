#ifndef MINILANG_VISITOR_H
#define MINILANG_VISITOR_H

#include <string>

namespace core::parser {

    class ASTProgramNode;

    template <typename T> class ASTLiteralNode;
    class ASTBinaryExprNode;
}

namespace core::visitor {
    typedef struct vT {
        vT() : i(0), r(0), b(0), s("") {};
        int i;
        float r;
        bool b;
        std::string s;
    } value_t;
    
    class Visitor {

    public:
        virtual void visit(parser::ASTProgramNode*) = 0;
        virtual void visit(parser::ASTLiteralNode<int>*) = 0;
        virtual void visit(parser::ASTBinaryExprNode*) = 0;
    };

}


#endif
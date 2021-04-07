#include "ast.h"
#include <utility>
#include <iostream>

using namespace core::parser;

// Program Node
ASTProgramNode::ASTProgramNode(std::vector<ASTNode*> statements) :
        statements(std::move(statements))
{}

// Expression Nodes
ASTBinaryExprNode::ASTBinaryExprNode(std::string op, ASTExprNode *left, ASTExprNode *right,
                                     unsigned int line_number) :
        op(std::move(op)),
        left(left),
        right(right),
        code_line_number(line_number)
{}

// Statement Nodes
ASTDeclarationNode::ASTDeclarationNode(TYPE type, std::string identifier, ASTExprNode *expr,
                                       unsigned int line_number) :
    type(type),
    identifier(std::move(identifier)),
    expr(expr),
    line_number(line_number)
{}

ASTIdentifierNode::ASTIdentifierNode(std::string identifier, unsigned int line_number) :
        identifier(std::move(identifier)),
        line_number(line_number)
{}


// Accept functions for visitors
void ASTBinaryExprNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

namespace core::parser {

    template<>
    void ASTLiteralNode<int>::accept(visitor::Visitor *v) {
        v->visit(this);
    }
}

void ASTProgramNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTDeclarationNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTIdentifierNode::accept(visitor::Visitor *v){
    v -> visit(this);
}
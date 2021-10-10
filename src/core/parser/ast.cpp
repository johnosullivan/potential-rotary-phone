#include "ast.h"
#include <utility>
#include <iostream>

using namespace core::parser;

ASTFuncNode::ASTFuncNode(std::string identifier,
                                                     std::vector<std::pair<std::string, TYPE>> parameters,
                                                     TYPE type, ASTBlockNode* block, unsigned int line_number) :
        identifier(std::move(identifier)),
        parameters(std::move(parameters)),
        type(type),
        block(block),
        line_number(line_number)
{
    // Generate signature
    this->signature = std::vector<TYPE>();
    for(auto param : this->parameters) {
        variable_names.push_back(param.first);
        signature.push_back(param.second);
    }
}

ASTBlockNode::ASTBlockNode(std::vector<ASTStatementNode*> statements, unsigned int line_number) :
        statements(std::move(statements)),
        line_number(line_number)
{}

// Program Node
ASTProgramNode::ASTProgramNode(std::vector<ASTNode*> statements) :
        statements(std::move(statements))
{}

// Expression Nodes
ASTBinaryExprNode::ASTBinaryExprNode(std::string op, ASTExprNode *left, ASTExprNode *right, unsigned int line_number) :
        op(std::move(op)),
        left(left),
        right(right),
        code_line_number(line_number)
{}

ASTAssignmentNode::ASTAssignmentNode(std::string identifier, ASTExprNode *expr, unsigned int line_number) :
        identifier(std::move(identifier)),
        expr(expr),
        line_number(line_number)
{}

// Statement Nodes
ASTDeclarationNode::ASTDeclarationNode(TYPE type, std::string identifier, ASTExprNode *expr, unsigned int line_number) :
    type(type),
    identifier(std::move(identifier)),
    expr(expr),
    line_number(line_number)
{}

ASTIdentifierNode::ASTIdentifierNode(std::string identifier, unsigned int line_number) :
        identifier(std::move(identifier)),
        line_number(line_number)
{}

ASTStdOutNode::ASTStdOutNode(ASTExprNode *expr, unsigned int line_number) :
        expr(expr),
        line_number(line_number)
{}

ASTReturnNode::ASTReturnNode(ASTExprNode *expr, unsigned int line_number) :
        expr(expr),
        line_number(line_number)
{}

ASTExprFuncCallNode::ASTExprFuncCallNode(std::string identifier, std::vector<ASTExprNode*> parameters,
                                         unsigned int line_number) :
    identifier(std::move(identifier)),
    parameters(std::move(parameters)),
    line_number(line_number)
{}

namespace core::parser {
    template<>
    void ASTLiteralNode<int>::accept(visitor::Visitor *v) {
        v->visit(this);
    }
    template<>
    void ASTLiteralNode<float>::accept(visitor::Visitor *v) {
        v->visit(this);
    }
    template<>
    void ASTLiteralNode<std::string>::accept(visitor::Visitor *v) {
        v->visit(this);
    }
}

void ASTBinaryExprNode::accept(visitor::Visitor *v){
    v -> visit(this);
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

void ASTAssignmentNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTStdOutNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTBlockNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTFuncNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTReturnNode::accept(visitor::Visitor *v){
    v -> visit(this);
}

void ASTExprFuncCallNode::accept(visitor::Visitor *v){
    v -> visit(this);
}
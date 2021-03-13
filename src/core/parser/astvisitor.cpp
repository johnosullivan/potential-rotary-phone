#include "astvisitor.h"

#include <iostream>

using namespace core::visitor;

ASTVisitor::ASTVisitor() : indentation_level(0) {

};

std::string ASTVisitor::indentation() {
    std::string tabs;

    for(unsigned int i = 0; i < indentation_level; i++)
        tabs += TAB;

    return tabs;
}

void ASTVisitor::visit(parser::ASTProgramNode *program) {
    std::cout << "<exec>" << std::endl;

    indentation_level++;

    for(auto &statement : program -> statements)
        statement -> accept(this);

    indentation_level--;


    std::cout << "</exec>" << std::endl;
}

void ASTVisitor::visit(parser::ASTLiteralNode<int> *lit) {
    std::cout << indentation()  << "<int>" << std::to_string(lit->val) << "</int>" << std::endl;
}

void ASTVisitor::visit(parser::ASTBinaryExprNode *bin) {
    std::cout << indentation() << "<bin op = \"" + safe_op(bin->op) +
                                "\">" << std::endl;

    // Indent
    indentation_level++;

    bin -> left -> accept(this);
    bin -> right -> accept(this);

    // Unindent
    indentation_level--;

    std::cout << indentation() << "</bin>" << std::endl;
}

std::string ASTVisitor::type_str(parser::TYPE t) {
    switch(t) {
        case parser::INT:
            return "int";
        default:
            throw std::runtime_error("invalid type encountered in syntax tree");
    }
}

std::string ASTVisitor::safe_op(std::string op) {
    return op;
}

ASTVisitor::~ASTVisitor(){}
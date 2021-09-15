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
    std::cout << "<program>" << std::endl;

    indentation_level++;

    for(auto &statement : program -> statements)
        statement -> accept(this);

    indentation_level--;

    std::cout << "</program>" << std::endl;
}

void ASTVisitor::visit(parser::ASTDeclarationNode *decl) {
    std::cout << indentation() << "<declaration>" << std::endl;

    indentation_level++;

    std::cout << indentation() << "<id type = \"" + type_str(decl -> type) + "\">"
                             << decl -> identifier << "</id>" << std::endl;

    decl -> expr -> accept(this);

    indentation_level--;

    std::cout << indentation() << "</declaration>" << std::endl;
}

void ASTVisitor::visit(parser::ASTAssignmentNode *assign) {

}

void ASTVisitor::visit(parser::ASTIdentifierNode *id) {
    std::cout << indentation() << "<id>";

    std::cout << id -> identifier;

    std::cout << "</id>" << std::endl;
}

void ASTVisitor::visit(parser::ASTLiteralNode<int> *lit) {
    std::cout << indentation()  << "<int>" << std::to_string(lit->val) << "</int>" << std::endl;
}

void ASTVisitor::visit(parser::ASTLiteralNode<std::string> *lit) {
    std::cout << indentation()  << "<string>" << lit->val << "</string>" << std::endl;
}

void ASTVisitor::visit(parser::ASTLiteralNode<float> *lit) {
    std::cout << indentation()  << "<float>" << std::to_string(lit->val) << "</float>" << std::endl;
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
        case parser::FLOAT:
            return "float";
        default:
            throw std::runtime_error("invalid type encountered in syntax tree");
    }
}

std::string ASTVisitor::safe_op(std::string op) {
    return op;
}

ASTVisitor::~ASTVisitor(){}
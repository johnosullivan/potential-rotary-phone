#include "astvisitor.h"

#include <iostream>

using namespace core::visitor;

ASTVisitor::ASTVisitor() : indentation_level(0) {

};

std::string ASTVisitor::indentation() {
    std::string tabs;

    for(unsigned int i = 0; i < indentation_level; i++) {
        tabs += TAB;
    }

    return tabs;
}

void ASTVisitor::visit(parser::ASTBlockNode *block) {
    std::cout << indentation() << "<block>" << std::endl;

    // Indent
    indentation_level++;

    // For each statement, accept
    for(auto &statement : block -> statements)
        statement -> accept(this);

    // Unindent
    indentation_level--;

    std::cout << indentation() << "</block>" << std::endl;
}

void ASTVisitor::visit(parser::ASTFuncNode *func) {

    std::cout << indentation() << "<fn type = \"" + type_str(func->type) +
                                "\">" << std::endl;

    // Indent
    indentation_level++;

    // Function identifier
    std::cout << indentation() << "<id>" + func->identifier + "</id>" << std::endl;

    // For each parameter
    for(auto &param : func -> parameters){
        std::cout << indentation() << "<param type = \"" + type_str(param.second) +
                                    "\">" + param.first + "</param>" << std::endl;
    }

    // Function body
    func -> block -> accept(this);

    // Unindent
    indentation_level--;

    std::cout << indentation() << "</fn>" << std::endl;
}

void ASTVisitor::visit(parser::ASTReturnNode *ret) {
    std::cout << indentation() << "<return>" << std::endl;

    // Indent
    indentation_level++;

    // Expression tags
    ret -> expr -> accept(this);

    // Unindent
    indentation_level--;

    std::cout << indentation() << "</return>" << std::endl;
}

void ASTVisitor::visit(parser::ASTExprFuncCallNode *func) {
    std::cout << indentation() << "<fnc>" << std::endl;

    // Indent
    indentation_level++;

    // Function identifier
    std::cout << indentation() << "<id>" + func->identifier + "</id>" << std::endl;

    // For each parameter
    for(auto &param : func -> parameters){
        std::cout << indentation() << "<args>" << std::endl;

        // Indent and Accept
        indentation_level++;
        param->accept(this);

        // Unindent
        indentation_level--;
        std::cout << indentation() << "</args>" << std::endl;
    }

    // Unindent
    indentation_level--;
    std::cout << indentation() << "</fnc>" << std::endl;
}

void ASTVisitor::visit(parser::ASTProgramNode *program) {
    std::cout << "\n<program>" << std::endl;

    indentation_level++;

    for(auto &statement : program -> statements) {
        statement -> accept(this);
    }

    indentation_level--;

    std::cout << "</program>\n" << std::endl;
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

void ASTVisitor::visit(parser::ASTStdOutNode *stdout){
    std::cout << indentation() << "<stdout>" << std::endl;

    indentation_level++;

    stdout -> expr -> accept(this);

    indentation_level--;

    std::cout << indentation() << "</stdout>" << std::endl;
}

std::string ASTVisitor::type_str(parser::TYPE t) {
    switch(t) {
        case parser::INT:
            return "int";
        case parser::FLOAT:
            return "float";
        case parser::STRING:
            return "string";
        case parser::RES_NONE:
            return "none";
        default:
            throw std::runtime_error("invalid type encountered in syntax tree");
    }
}

std::string ASTVisitor::safe_op(std::string op) {
    return op;
}

ASTVisitor::~ASTVisitor(){}
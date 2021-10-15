#include <iostream>

#include "common/common.h"
#include "core/parser/astvisitor.h"

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
    LOG_F(INFO, "%s<block>", indentation().c_str());

    // Indent
    indentation_level++;

    // For each statement, accept
    for(auto &statement : block -> statements)
        statement -> accept(this);

    // Unindent
    indentation_level--;

    LOG_F(INFO, "%s</block>", indentation().c_str());
}

void ASTVisitor::visit(parser::ASTFuncNode *func) {
    LOG_F(INFO, "%s<fn type='%s'>", indentation().c_str(), type_str(func->type).c_str());

    // Indent
    indentation_level++;

    // Function identifier
    LOG_F(INFO, "%s<id>%s</id>", indentation().c_str(), func->identifier.c_str());

    // For each parameter
    for(auto &param : func -> parameters){
        LOG_F(INFO, "%s<param type='%s'>%s</param>", indentation().c_str(), type_str(param.second).c_str(), param.first.c_str());
    }

    // Function body
    func -> block -> accept(this);

    // Unindent
    indentation_level--;

    LOG_F(INFO, "%s</fn>", indentation().c_str());
}

void ASTVisitor::visit(parser::ASTReturnNode *ret) {
    LOG_F(INFO, "%s<return>", indentation().c_str());

    // Indent
    indentation_level++;

    // Expression tags
    ret -> expr -> accept(this);

    // Unindent
    indentation_level--;

    LOG_F(INFO, "%s</return>", indentation().c_str());
}

void ASTVisitor::visit(parser::ASTExprFuncCallNode *func) {
    LOG_F(INFO, "%s<fnc>", indentation().c_str());

    // Indent
    indentation_level++;

    // Function identifier
    LOG_F(INFO, "%s<id>%s</id>", indentation().c_str(), func->identifier.c_str());

    // For each parameter
    for(auto &param : func -> parameters){
        LOG_F(INFO, "%s<args>", indentation().c_str());

        // Indent and Accept
        indentation_level++;
        param->accept(this);

        // Unindent
        indentation_level--;
        LOG_F(INFO, "%s</args>", indentation().c_str());
    }

    // Unindent
    indentation_level--;
    LOG_F(INFO, "%s</fnc>", indentation().c_str());
}

void ASTVisitor::visit(parser::ASTProgramNode *program) {
    LOG_F(INFO, "");
    LOG_F(INFO, "<program>");

    indentation_level++;

    for(auto &statement : program -> statements) {
        statement -> accept(this);
    }

    indentation_level--;

    LOG_F(INFO, "</program>");
}

void ASTVisitor::visit(parser::ASTDeclarationNode *decl) {
    LOG_F(INFO, "%s<declaration>", indentation().c_str());

    indentation_level++;

    LOG_F(INFO, "%s<id type ='%s'>%s</id>", indentation().c_str(), type_str(decl -> type).c_str(), decl->identifier.c_str());

    decl -> expr -> accept(this);

    indentation_level--;

    LOG_F(INFO, "%s</declaration>", indentation().c_str());

}

void ASTVisitor::visit(parser::ASTAssignmentNode *assign) {

}

void ASTVisitor::visit(parser::ASTIdentifierNode *id) {
    LOG_F(INFO, "%s<id>%s</id>", indentation().c_str(), id->identifier.c_str());
}

void ASTVisitor::visit(parser::ASTLiteralNode<int> *lit) {
    LOG_F(INFO, "%s<int>%s</int>", indentation().c_str(), std::to_string(lit->val).c_str());
}

void ASTVisitor::visit(parser::ASTLiteralNode<bool> *lit) {
    LOG_F(INFO, "%s<bool>%s</bool>", indentation().c_str(), (lit->val) ? "true" : "false");

}

void ASTVisitor::visit(parser::ASTLiteralNode<std::string> *lit) {
    LOG_F(INFO, "%s<string>%s</string>", indentation().c_str(), lit->val.c_str());
}

void ASTVisitor::visit(parser::ASTLiteralNode<float> *lit) {
    LOG_F(INFO, "%s<float>%s</float>", indentation().c_str(), std::to_string(lit->val).c_str());
}

void ASTVisitor::visit(parser::ASTBinaryExprNode *bin) {
    LOG_F(INFO, "%s<bin> op='%s'>", indentation().c_str(), safe_op(bin->op).c_str());

    // Indent
    indentation_level++;

    bin -> left -> accept(this);
    bin -> right -> accept(this);

    // Unindent
    indentation_level--;

    LOG_F(INFO, "%s</bin>", indentation().c_str());
}

void ASTVisitor::visit(parser::ASTStdOutNode *stdout){
    LOG_F(INFO, "%s<stdout>", indentation().c_str());
    indentation_level++;

    stdout -> expr -> accept(this);

    indentation_level--;

    LOG_F(INFO, "%s</stdout>", indentation().c_str());
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
        case parser::BOOL:
            return "bool";
        default:
            throw std::runtime_error("invalid type encountered in syntax tree");
    }
}

std::string ASTVisitor::safe_op(std::string op) {
    return op;
}

ASTVisitor::~ASTVisitor(){}
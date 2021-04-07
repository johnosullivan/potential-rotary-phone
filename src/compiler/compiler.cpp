#include "compiler.h"

#include <iostream>

using namespace core::compiler;
using namespace core::visitor;

Compiler::Compiler(COMPILER_ARCH_TYPE arch_type) {
    arch_type = arch_type;
    asm_source = "";
};

void Compiler::visit(parser::ASTProgramNode *program) {
    for(auto &statement : program -> statements) {
        statement -> accept(this);
    }
}

void Compiler::visit(parser::ASTLiteralNode<int> *lit) {
    value_t v;
    v.i = lit->val;
    current_expression_type = parser::INT;
    current_expression_value = std::move(v);
}

void Compiler::visit(parser::ASTBinaryExprNode *bin) {
    std::string op = bin -> op;

    bin -> left -> accept(this);
    parser::TYPE l_type = current_expression_type;
    value_t l_value = current_expression_value;

    bin -> right -> accept(this);
    parser::TYPE r_type = current_expression_type;
    value_t r_value = current_expression_value;    
}

void Compiler::visit(parser::ASTDeclarationNode *decl) { 

}

void Compiler::visit(parser::ASTIdentifierNode *id) {

}

Compiler::~Compiler(){}
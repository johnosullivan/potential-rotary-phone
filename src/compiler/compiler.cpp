#include <iostream>

#include "compiler/compiler.h"

using namespace core::compiler;
using namespace core::visitor;

Compiler::Compiler(COMPILER_ARCH_TYPE arch_type) {
    arch_type = arch_type;
    asm_source = "";

    scopes.push_back(new core::visitor::Scope());
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

    std::cout << "l_value: " << l_value.i << std::endl;

    bin -> right -> accept(this);
    parser::TYPE r_type = current_expression_type;
    value_t r_value = current_expression_value; 

    std::cout << "r_value: " << r_value.i << std::endl;   
}

void Compiler::visit(parser::ASTAssignmentNode *assign) {
    unsigned long i;
    for (i = scopes.size() - 1; !scopes[i] -> already_declared(assign->identifier); i--);

    assign -> expr -> accept(this);

    switch(scopes[i]->type_of(assign->identifier)){
        case parser::INT:
            scopes[i]->declare(assign->identifier, current_expression_value.i);
            break;
        default:
            break;
    }
}

void Compiler::visit(parser::ASTDeclarationNode *decl) { 
    decl -> expr -> accept(this);

    switch(decl -> type){
        case parser::INT:
            scopes.back()->declare(decl->identifier, current_expression_value.i);
            break;
        default:
            break;
    }
}

void Compiler::visit(parser::ASTIdentifierNode *id) {
    unsigned long i;
    for (i = scopes.size() - 1; !scopes[i] -> already_declared(id->identifier); i--);

    current_expression_type = scopes[i] -> type_of(id->identifier);
    current_expression_value = scopes[i] -> value_of(id->identifier);   
}

Compiler::~Compiler(){}
#include <iostream>
#include "interpreter.h"

using namespace core::visitor;

bool Scope::already_declared(std::string identifier) {
    return variable_symbol_table.find(identifier) != variable_symbol_table.end();
}

void Scope::declare(std::string identifier, int int_value) {
    value_t value;
    value.i = int_value;
    variable_symbol_table[identifier] = std::make_pair(parser::INT, value);
}

core::parser::TYPE Scope::type_of(std::string identifier) {
    return variable_symbol_table[identifier].first;
}

value_t Scope::value_of(std::string identifier) {
    return variable_symbol_table[identifier].second;
}


Interpreter::Interpreter(Scope* global_scope) {
    scopes.push_back(global_scope);
}

Interpreter::Interpreter(){ 
    scopes.push_back(new Scope());
}

Interpreter::~Interpreter() = default;

void Interpreter::visit(parser::ASTProgramNode *prog) {
    for(auto &statement : prog -> statements) {
        statement -> accept(this);
    }
}

void Interpreter::visit(parser::ASTLiteralNode<int> *lit) {
    value_t v;
    v.i = lit->val;
    current_expression_type = parser::INT;
    current_expression_value = std::move(v);
}

void Interpreter::visit(parser::ASTLiteralNode<float> *lit) {
    value_t v;
    v.f = lit->val;
    current_expression_type = parser::FLOAT;
    current_expression_value = std::move(v);
}

void Interpreter::visit(parser::ASTAssignmentNode *assign) {
    unsigned long i;
    for (i = scopes.size() - 1; !scopes[i] -> already_declared(assign->identifier); i--);

    assign -> expr -> accept(this);

    switch(scopes[i]->type_of(assign->identifier)){
        case parser::INT:
            scopes[i]->declare(assign->identifier, current_expression_value.i);
            break;
    }
}

void Interpreter::visit(parser::ASTBinaryExprNode *bin) {
    std::string op = bin -> op;

    bin -> left -> accept(this);
    parser::TYPE l_type = current_expression_type;
    value_t l_value = current_expression_value;

    bin -> right -> accept(this);
    parser::TYPE r_type = current_expression_type;
    value_t r_value = current_expression_value;

    value_t v;
    
    if(op == "+" || op == "-") {
        if(l_type == parser::INT && r_type == parser::INT){
            current_expression_type = parser::INT;
            if(op == "+") {
                v.i = l_value.i + r_value.i;
            } else if(op == "-") {
                v.i = l_value.i - r_value.i;
            }
        }
        if(l_type == parser::FLOAT && r_type == parser::INT){
            current_expression_type = parser::FLOAT;
            if(op == "+") {
                v.f = l_value.f + r_value.i;
            } else if(op == "-") {
                v.f = l_value.f - r_value.i;
            }
        }
        if(l_type == parser::INT && r_type == parser::FLOAT){
            current_expression_type = parser::FLOAT;
            if(op == "+") {
                v.f = l_value.i + r_value.f;
            } else if(op == "-") {
                v.f = l_value.i - r_value.f;
            }
        }
    }
    
    current_expression_value = v;
}

std::pair<core::parser::TYPE, value_t> Interpreter::current_expr(){
    return std::move(std::make_pair(current_expression_type, current_expression_value));
};

void Interpreter::visit(parser::ASTDeclarationNode *decl) { 
    decl -> expr -> accept(this);

    switch(decl -> type){
        case parser::INT:
            scopes.back()->declare(decl->identifier, current_expression_value.i);
            break;
    }
}

void Interpreter::visit(parser::ASTIdentifierNode *id) {
    //std::cout << "ASTIdentifierNode" << std::endl;

    unsigned long i;
    for (i = scopes.size() - 1; !scopes[i] -> already_declared(id->identifier); i--);

    current_expression_type = scopes[i] -> type_of(id->identifier);
    current_expression_value = scopes[i] -> value_of(id->identifier);
}

std::string type_str(core::parser::TYPE t) {

    switch(t){
        case core::parser::INT:
            return "int";
        default:
            throw std::runtime_error("invalid type encountered.");
    }
}


#include <iostream>
#include "interpreter.h"

using namespace core::visitor;

Interpreter::Interpreter(){
    
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
    }
    
    current_expression_value = v;
}

std::pair<core::parser::TYPE, value_t> Interpreter::current_expr(){
    return std::move(std::make_pair(current_expression_type,
                                    current_expression_value));
};


std::string type_str(core::parser::TYPE t) {

    switch(t){
        case core::parser::INT:
            return "int";
        default:
            throw std::runtime_error("invalid type encountered.");
    }
}


#include <iostream>
#include "interpreter.h"

using namespace core::visitor;

bool Scope::already_declared(std::string identifier) {
    return variable_symbol_table.find(identifier) != variable_symbol_table.end();
}

bool Scope::already_declared(std::string identifier, std::vector<parser::TYPE> signature) {

    auto funcs = function_symbol_table.equal_range(identifier);

    // If key is not present in multimap
    if(std::distance(funcs.first, funcs.second) == 0)
        return false;

    // Check signature for each function in multimap
    for (auto i = funcs.first; i != funcs.second; i++)
        if(std::get<0>(i->second) == signature)
            return true;

    // Function with matching signature not found
    return false;
}

void Scope::declare(std::string identifier, int int_value) {
    value_t value;
    value.i = int_value;
    variable_symbol_table[identifier] = std::make_pair(parser::INT, value);
}

void Scope::declare(std::string identifier, float float_value) {
    value_t value;
    value.f = float_value;
    variable_symbol_table[identifier] = std::make_pair(parser::FLOAT, value);
}

void Scope::declare(std::string identifier, std::string string_value) {
    value_t value;
    value.s = string_value;
    variable_symbol_table[identifier] = std::make_pair(parser::STRING, value);
}

void Scope::declare(std::string identifier, std::vector<parser::TYPE> signature, std::vector<std::string> variable_names, parser::ASTBlockNode* block) {
    function_symbol_table.insert(std::make_pair(identifier, std::make_tuple(signature,
                                                               variable_names,
                                                               block)));
}

core::parser::ASTBlockNode* Scope::block_of(std::string identifier, std::vector<parser::TYPE> signature) {

    auto funcs = function_symbol_table.equal_range(identifier);

    // Match given signature to function in multimap
    for (auto i = funcs.first; i != funcs.second; i++)
        if(std::get<0>(i->second) == signature) {
            return std::get<2>(i->second);
        }

    return nullptr;
}

std::vector<std::string> Scope::variable_names_of(std::string identifier, std::vector<parser::TYPE> signature) {
    auto funcs = function_symbol_table.equal_range(identifier);

    for (auto i = funcs.first; i != funcs.second; i++) {
        if(std::get<0>(i->second) == signature) {
            return std::get<1>(i->second);
        }
    }
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

void Interpreter::visit(parser::ASTStdOutNode *stdout){
    stdout -> expr -> accept(this);

    switch(current_expression_type) {
        case parser::INT:
            std::cout << current_expression_value.i << std::endl;
            break;
        case parser::FLOAT:
            std::cout << current_expression_value.f << std::endl;
            break;
        case parser::STRING:
            std::cout << current_expression_value.s << std::endl;
            break;
        default:
             break;
    }

    current_expression_type = parser::RES_NONE;
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

void Interpreter::visit(parser::ASTLiteralNode<std::string> *lit) {
    value_t v;
    v.s = lit->val;
    current_expression_type = parser::STRING;
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
        case parser::FLOAT:
            scopes[i]->declare(assign->identifier, current_expression_value.f);
            break;
        default:
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
        case parser::FLOAT:
            scopes.back()->declare(decl->identifier, current_expression_value.f);
            break;
        default:
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
        case core::parser::FLOAT:
            return "float";
        default:
            throw std::runtime_error("invalid type encountered.");
    }
}

void Interpreter::visit(parser::ASTFuncNode *func) {
    scopes.back() -> declare(func->identifier, func->signature, func->variable_names, func->block);
}

void Interpreter::visit(parser::ASTBlockNode *block) {
    // Create new scope
    scopes.push_back(new Scope());

    for(unsigned int i = 0; i < current_function_arguments.size(); i++){
        switch(current_function_arguments[i].first){
            case parser::INT:
                scopes.back() -> declare(current_function_parameters[i], current_function_arguments[i].second.i);
                break;
            case parser::FLOAT:
                scopes.back() -> declare(current_function_parameters[i], current_function_arguments[i].second.f);
                break;
            case parser::STRING:
                scopes.back() -> declare(current_function_parameters[i], current_function_arguments[i].second.s);
                break;
        }
    }

    current_function_parameters.clear();
    current_function_arguments.clear();

    for(auto &stmt : block -> statements) {
        stmt -> accept(this);
    }

    // Close scope
    scopes.pop_back();
}

void Interpreter::visit(parser::ASTReturnNode *ret) {
    // Update current expression
    ret -> expr -> accept(this);
}

void Interpreter::visit(parser::ASTExprFuncCallNode *func) {
    // Determine the signature of the function
    std::vector<parser::TYPE> signature;
    std::vector<std::pair<parser::TYPE, value_t>> current_function_arguments;

    // For each parameter,
    for (auto param : func -> parameters) {

        // visit to update current expr type
        param->accept(this);

        // add the type of current expr to signature
        signature.push_back(current_expression_type);

        // add the current expr to the local vector of function arguments, to be
        // used in the creation of the function scope
        current_function_arguments.emplace_back(current_expression_type, current_expression_value);
    }

    // Update the global vector current_function_arguments
    for(auto arg : current_function_arguments)
        this -> current_function_arguments.push_back(arg);

    // Determine in which scope the function is declared
    unsigned long i;
    for (i = scopes.size() - 1;
         !scopes[i]->already_declared(func->identifier, signature);
         i--);

    // Populate the global vector of function parameter names, to be used in creation of
    // function scope
    current_function_parameters = scopes[i] -> variable_names_of(func->identifier, signature);

    // Visit the corresponding function block
    scopes[i] -> block_of(func->identifier, signature) -> accept(this);

}

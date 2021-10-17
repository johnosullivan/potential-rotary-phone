#include <iostream>
#include <cmath> 

#include "compiler/compiler.h"

using namespace core::compiler;
using namespace core::visitor;


AsmLine::AsmLine(std::string _label, std::string _instruction, std::string _operand,std::string _comment) {
    label = _label;
    instruction = _instruction;
    operand = _operand;
    comment = _comment;
}

AsmLine::~AsmLine() {
    
}

std::string AsmLine::getLabel() {
    return label;
}

std::string AsmLine::getInstruction() {
    return instruction;
}

std::string AsmLine::getOperand() {
    return operand;
}

std::string AsmLine::getComment() {
    return comment;
}


Compiler::Compiler(COMPILER_ARCH_TYPE arch_type) : indentation_num(0) {
    arch_type = arch_type;
    asm_source = "";

    scopes.push_back(new core::visitor::Scope());
};

/* helper function */
std::string Compiler::indentation() {
    std::string tabs;

    for(unsigned int i = 0; i < indentation_num; i++) {
        tabs += TAB;
    }

    return tabs;
}

void Compiler::visit(parser::ASTProgramNode *program) {
    add_asm(AsmLine("", "global", "_main", ""));
    add_asm(AsmLine("", "", "", ""));


    for(auto &statement : program -> statements) {
        statement -> accept(this);
    }

    add_asm(AsmLine("", "", "", ""));
    add_asm(AsmLine("", "section", ".data", ""));
    add_asm(AsmLine("message:", "db", "\"Hello World!\", 10", ""));
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

    //std::cout << "l_value: " << l_value.i << std::endl;

    bin -> right -> accept(this);
    parser::TYPE r_type = current_expression_type;
    value_t r_value = current_expression_value; 

    //std::cout << "r_value: " << r_value.i << std::endl;   
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

void Compiler::visit(parser::ASTStdOutNode* std) {
    std -> expr -> accept(this);
}

void Compiler::visit(parser::ASTLiteralNode<float>* lit) {

}

void Compiler::visit(parser::ASTLiteralNode<bool>* lit) {

}

void Compiler::visit(parser::ASTLiteralNode<std::string>* lit) {

}

void Compiler::visit(parser::ASTFuncNode* fn) {
    add_asm(AsmLine(fn->identifier + ":", "", "", ""));
    add_asm(AsmLine("", "push", "rbp", ""));
    add_asm(AsmLine("", "mov", "edx, DWORD [rbp-4]", ""));

    
    // lists all the parameters
    for(auto &param : fn -> parameters){

    }

    // Function body
    fn -> block -> accept(this);
}

void Compiler::visit(parser::ASTBlockNode* block) {
    // visit all the block statements
    for(auto &statement : block -> statements) {
        statement -> accept(this);
    }
}

void Compiler::visit(parser::ASTReturnNode* ret) {
    // Expression tags
    ret -> expr -> accept(this);
}

void Compiler::visit(parser::ASTExprFuncCallNode* node) {
    for(auto &param : node -> parameters) {
        param->accept(this);
    }
}

std::string Compiler::padding(int j) {
    std::string tabs;

    for(unsigned int i = 0; i < j; i++) {
        tabs += " ";
    }

    return tabs;
}

void Compiler::add_asm(AsmLine a) {
    if (a.getLabel().length() > l0_padding_max) {
        l0_padding_max = a.getLabel().length();
    }

    if (a.getInstruction().length() > l1_padding_max) {
        l1_padding_max = a.getInstruction().length();
    }

    if (a.getOperand().length() > l2_padding_max) {
        l2_padding_max = a.getOperand().length();
    }

    asm_commands.push_back(a);
}

std::string Compiler::comment_char_arch() {
    switch(arch_type) {
        case ARCH_X86:
            return ";";
        case ARCH_X86_64:
            return ";";
        case ARCH_ARM:
            return ";";
    }
}

void Compiler::stdout_vector() {
    std::cout << "" << std::endl;

    int l0 = l0_padding_max + 1;
    int l1 = l1_padding_max + 1;
    int l2 = l2_padding_max + 1;

    /*std::cout << "l0_padding_max: " << l0_padding_max << std::endl;
    std::cout << "l1_padding_max: " << l1_padding_max << std::endl;
    std::cout << "l2_padding_max: " << l2_padding_max << std::endl;
    std::cout << "" << std::endl;*/

    for (auto &command : asm_commands)
    { 
        std::string temp = "";
        temp = temp + command.getLabel();
        int diffl0 = command.getLabel().length() - l0;
        if (diffl0 > 0) {
            temp.erase(temp.length() - (1 + diffl0), diffl0);
        } else if (diffl0 < 0) {
            temp = temp + padding(std::abs(diffl0));
        }

        temp = temp + command.getInstruction();
        int diffl1 = command.getInstruction().length() - l1;
        if (diffl1 > 0) {
            temp.erase(temp.length() - (1 + diffl1), diffl1);
        } else if (diffl1 < 0) {
            temp = temp + padding(std::abs(diffl1));
        }

        temp = temp + command.getOperand();
        int diffl3 = command.getOperand().length() - (l1 + l2);
        if (diffl3 > 0) {
            temp.erase(temp.length() - (1 + diffl3), diffl3);
        } else if (diffl3 < 0) {
            temp = temp + padding(std::abs(diffl3));
        }

        if (command.getComment().length() != 0) {
            temp = temp + padding(1) + comment_char_arch() + " " + command.getComment();
        }

        std::cout << temp << std::endl;
    }
    std::cout << "" << std::endl;
}

Compiler::~Compiler(){}
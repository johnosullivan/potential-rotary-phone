#include <iostream>
#include <sstream>  
#include <string>   
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

    current_registers.clear();

    Registers::setup_register(&current_registers, &current_preserved, arch_type);

    for(auto it = current_registers.cbegin(); it != current_registers.cend(); ++it) {
        //std::cout << it->first << " " << it->second << std::endl;
    }

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
        add_asm(AsmLine("", "", "", ""));
    }
}


void Compiler::visit(parser::ASTLiteralNode<int> *lit) {
    if (current_bit_stack == 0) {
        current_bit_stack = 4;
    } else {
        current_bit_stack = current_bit_stack + 4;
    }
    
    value_t v;
    v.i = lit->val;
    v.bit = current_bit_stack;
    current_expression_type = parser::INT;
    current_expression_value = std::move(v);

    /*char asmbuffer[30];
    sprintf(asmbuffer, "edx, DWORD [rbp-%d]", v.bit);
    add_asm(AsmLine("", "mov", asmbuffer, ""));*/
}

void Compiler::visit(parser::ASTBinaryExprNode *bin) {
    std::string op = bin -> op;

    bin -> left -> accept(this);
    parser::TYPE l_type = current_expression_type;
    value_t l_value = current_expression_value;

    bin -> right -> accept(this);
    parser::TYPE r_type = current_expression_type;
    value_t r_value = current_expression_value;

    /*char asmbuffer1[30];
    sprintf(asmbuffer1, "edx, DWORD [rbp-%d]", l_value.bit);
    add_asm(AsmLine("", "mov", asmbuffer1, ""));

    char asmbuffer2[30];
    sprintf(asmbuffer2, "eax, DWORD [rbp-%d]", r_value.bit);
    add_asm(AsmLine("", "mov", asmbuffer2, ""));*/

    /*std::cout << "ASTBinaryExprNode" << std::endl;
    std::cout << "l_value" << l_value.i << std::endl;
    std::cout << "r_value" << r_value.i << std::endl;*/

    /*if (l_value.bit != 0) {
        char asmbuffer1[30];
        sprintf(asmbuffer1, "edx, DWORD [rbp-%d]", l_value.bit);
        add_asm(AsmLine("", "mov", asmbuffer1, ""));
        stack_t.push(l_value);
    }

    if (r_value.bit != 0) {
        stack_t.push(r_value);
        char asmbuffer2[30];
        sprintf(asmbuffer2, "eax, DWORD [rbp-%d]", r_value.bit);
        add_asm(AsmLine("", "mov", asmbuffer2, ""));
    } */  

    //add_asm(AsmLine("", "add", "edx, eax", ""));
    //std::cout << stack_t.size() << std::endl;

    value_t v;
    current_expression_type = parser::INT;
    v.i = l_value.i + r_value.i;
    current_expression_value = v;
}

void Compiler::visit(parser::ASTAssignmentNode *assign) {
    unsigned long i;
    for (i = scopes.size() - 1; !scopes[i] -> already_declared(assign->identifier); i--);

    assign -> expr -> accept(this);

    switch(scopes[i]->type_of(assign->identifier)){
        case parser::INT:
            scopes[i]->declare(assign->identifier, current_expression_value.i, 0);
            break;
        default:
            break;
    }
}

void Compiler::visit(parser::ASTDeclarationNode *decl) { 
    decl -> expr -> accept(this);

    /*std::cout << decl->identifier << std::endl;
    std::cout << current_expression_value.i << std::endl;
    std::cout << current_expression_value.bit << std::endl;*/

    char asmbuffer[30];
    sprintf(asmbuffer, "DWORD [rbp-%d], %d", current_expression_value.bit, current_expression_value.i);
    add_asm(AsmLine("", "mov", asmbuffer, ""));
    
    switch(decl -> type){
        case parser::INT:
            scopes.back()->declare(decl->identifier, current_expression_value.i, current_expression_value.bit);
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

    /*char asmbuffer[30];
    sprintf(asmbuffer, "edx, DWORD [rbp-%d]", current_expression_value.bit);
    add_asm(AsmLine("", "mov", asmbuffer, ""));*/
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
    std::string id = fn->identifier;
    if (fn->identifier == "main") {
        id = "_" + id;
    }  

    add_asm(AsmLine(id + ":", "", "", ""));
    add_asm(AsmLine("", "push", "rbp", ""));
    add_asm(AsmLine("", "mov", "rbp, rsp", ""));

    
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

    // Take node data to build asm.
    if (current_expression_type == parser::INT) {
        char asmbuffer[30];
        sprintf(asmbuffer, "eax, %d", current_expression_value.i);
        add_asm(AsmLine("", "mov", asmbuffer, ""));
    }
    
    add_asm(AsmLine("", "pop", "rbp", ""));
    add_asm(AsmLine("", "ret", "", ""));
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
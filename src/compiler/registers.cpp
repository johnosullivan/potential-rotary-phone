#include <iostream>
#include <sstream>  
#include <string>   
#include <cmath> 
#include <vector>
#include <map>

#include "compiler/registers.h"

using namespace core::compiler;

void Registers::setup_register(std::map<std::string, int> *reg, std::vector<std::string> *pre, int arch_type) {
    switch(arch_type) {
        case 1: {
            /* x86_64 NASM Assembly */
            reg->insert(std::pair<std::string, int>("rax", 0)); // scratch
            reg->insert(std::pair<std::string, int>("rcx", 0)); // scratch
            reg->insert(std::pair<std::string, int>("rdx", 0)); // scratch
            reg->insert(std::pair<std::string, int>("rbx", 0)); // preserved
            reg->insert(std::pair<std::string, int>("rsp", 0)); // preserved
            reg->insert(std::pair<std::string, int>("rbp", 0)); // preserved
            reg->insert(std::pair<std::string, int>("rsi", 0)); // scratch
            reg->insert(std::pair<std::string, int>("rdi", 0)); // scratch
            reg->insert(std::pair<std::string, int>("r8", 0)); // scratch
            reg->insert(std::pair<std::string, int>("r9", 0)); // scratch
            reg->insert(std::pair<std::string, int>("r10", 0)); // scratch
            reg->insert(std::pair<std::string, int>("r11", 0)); // scratch
            reg->insert(std::pair<std::string, int>("r12", 0)); // preserved
            reg->insert(std::pair<std::string, int>("r13", 0)); // preserved
            reg->insert(std::pair<std::string, int>("r14", 0)); // preserved
            reg->insert(std::pair<std::string, int>("r15", 0)); // preserved
            /* preserved vector registers */
            pre->push_back("rbx");
            pre->push_back("rsp");
            pre->push_back("rbp");
            pre->push_back("r12");
            pre->push_back("r13");
            pre->push_back("r14");
            pre->push_back("r15");
        }
        default:
            break;
    }
}

Registers::Registers() {

}

Registers::~Registers() {

}
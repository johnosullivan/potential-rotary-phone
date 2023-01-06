#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <stack>

#include "common/visitor.h"
#include "core/parser/ast.h"
#include "compiler/registers.h"
#include "core/repl/interpreter.h"

namespace core::compiler {

    enum COMPILER_ARCH_TYPE {
        ARCH_X86        = 0,
        ARCH_X86_64     = 1, 
        ARCH_ARM        = 2 
    };

    class AsmLine {
        public:
            AsmLine(std::string _label, std::string _instruction, std::string _operand, std::string _command);
            ~AsmLine();
            std::string getLabel();
            std::string getInstruction();
            std::string getOperand();
            std::string getComment();
        private:
            std::string label;
            std::string instruction;
            std::string operand;
            std::string comment;
    };

    class Compiler : public core::visitor::Visitor {
        public:
            Compiler(COMPILER_ARCH_TYPE);
            ~Compiler();

            void visit(parser::ASTProgramNode*) override;
            void visit(parser::ASTLiteralNode<int>*) override;
            void visit(parser::ASTLiteralNode<float>*) override;
            void visit(parser::ASTLiteralNode<std::string>*) override;
            void visit(parser::ASTLiteralNode<bool>*)  override;
            void visit(parser::ASTBinaryExprNode*) override;

            void visit(parser::ASTDeclarationNode*) override;
            void visit(parser::ASTIdentifierNode*) override;
            void visit(parser::ASTAssignmentNode*) override;

            void visit(parser::ASTStdOutNode*) override;
            void visit(parser::ASTFuncNode*) override;
            void visit(parser::ASTBlockNode*) override;
            void visit(parser::ASTReturnNode*) override;
            void visit(parser::ASTExprFuncCallNode*) override;
            
            void stdout_vector();
            void add_asm(AsmLine);
        private:
            parser::TYPE current_expression_type;
            COMPILER_ARCH_TYPE arch_type;
            core::visitor::value_t current_expression_value;

            /* spacing indentation variables */
            unsigned int indentation_num;
            const std::string TAB = "    ";
            std::string indentation();
            std::string comment_char_arch();
            std::string padding(int);
            int l0_padding_max = 0;
            int l1_padding_max = 0;
            int l2_padding_max = 0;
            int current_bit_stack = 0;

            // type stack
            std::stack<visitor::value_t> stack_t;

            std::map<std::string, int> current_registers;
            std::vector<std::string> current_preserved;

            // asm code for cli nasm
            std::string asm_source; 
            // std::vector<std::string> asm_commands;
            std::vector<AsmLine> asm_commands;

            std::vector<core::visitor::Scope*> scopes;


    };

}

#endif //COMPILER_H
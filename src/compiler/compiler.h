#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>

#include "../common/visitor.h"
#include "../core/parser/ast.h"
#include "../core/repl/interpreter.h"

namespace core::compiler {
    enum COMPILER_ARCH_TYPE {
        ARCH_X86        = 0,
        ARCH_X86_64     = 1, 
        ARCH_ARM        = 2 
    };

    class Compiler : public core::visitor::Visitor {
        public:
            Compiler(COMPILER_ARCH_TYPE);
            ~Compiler();

            void visit(parser::ASTProgramNode*) override;
            void visit(parser::ASTLiteralNode<int>*) override;
            void visit(parser::ASTBinaryExprNode*) override;

            void visit(parser::ASTDeclarationNode*) override;
            void visit(parser::ASTIdentifierNode*) override;

            void visit(parser::ASTAssignmentNode*) override;
        private:
            parser::TYPE current_expression_type;
            COMPILER_ARCH_TYPE arch_type;
            core::visitor::value_t current_expression_value;

            // asm code for cli nasm
            std::string asm_source; 
            std::vector<std::string> asm_commands;

            std::vector<core::visitor::Scope*> scopes;
    };

}

#endif //COMPILER_H
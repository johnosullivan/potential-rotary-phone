#include <iostream>
#include <fstream>
#include <regex>

/* include core headers */
#include "core/lexer/lexer.h"
#include "core/parser/parser.h"

#include "core/parser/ast.h"
#include "core/parser/astvisitor.h"

#include "core/parser/interpreter.h"

#include "compiler/compiler.h"

int main() {
    for(;;){
        std::string input_line;
        std::string source;

        std::cout << "\n>>> _\b";
        std::getline(std::cin, source);

        if(source == ".q") {
            break;
        } else {
            std::cout << source << std::endl;

            core::parser::ASTProgramNode *prog;
            try {
                core::lexer::Lexer expr_lexer(source);
                core::parser::Parser parser = core::parser::Parser(&expr_lexer, 0);

                prog = new core::parser::ASTProgramNode(
                    std::vector<core::parser::ASTNode *>({parser.parse_expression()})
                );
            } catch(const std::exception &expr_e) {

            }

            std::cout << "====================================" << std::endl;

            /* Debugger AST */ 
            core::visitor::ASTVisitor ast_visitor;
            ast_visitor.visit(prog);

            core::visitor::Interpreter interpreter;
            interpreter.visit(prog);

            auto current = interpreter.current_expr();
            switch(current.first){
                case core::parser::INT:
                    std::cout << current.second.i;
                    break;
            }

            core::compiler::Compiler compiler(core::compiler::ARCH_X86_64);
            compiler.visit(prog);

            // clear IDLE commandline
            source = "";
        }
    }

    return 0;
}
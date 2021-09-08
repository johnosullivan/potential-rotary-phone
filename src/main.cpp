#include <iostream>
#include <fstream>
#include <regex>

#include <sstream>
#include <filesystem>

/* include core headers */
#include "core/lexer/lexer.h"
#include "core/parser/parser.h"
#include "core/parser/ast.h"
#include "core/parser/astvisitor.h"

#include "core/repl/interpreter.h"

#include "compiler/compiler.h"
/*
    var x:int = 0; var y:int = 0; x = 10; y = 20; var z:int = x + y;
*/

int main(int argc, char* argv[]) {
    std::string source;

    core::visitor::Scope global_scope;

    // check if a source file is defined
    if(argc == 2) {
            std::string file_path = argv[1];
            file_path = std::regex_replace(file_path, std::regex("^ +| +$"), "$1");

            std::ifstream file;
            file.open(file_path);
            if(!file) {
                std::cout << "Could not load: \"" + file_path + "\"" << std::endl;
                return 1;
            } else {
                std::string line;
                while(std::getline(file, line)) {
                    source.append(line + "\n");
                }
            }
            file.close();

            core::parser::ASTProgramNode *prog;

            core::lexer::Lexer expr_lexer(source);
            core::parser::Parser parser = core::parser::Parser(&expr_lexer);

            try {
                prog = parser.parse_program();
            } catch(const std::exception &e) {
                std::cout << "error 1: " << e.what() << std::endl;

                try {
                    core::lexer::Lexer expr_lexerz(source);
                    core::parser::Parser parserz = core::parser::Parser(&expr_lexerz, 0);

                    prog = new core::parser::ASTProgramNode(
                        std::vector<core::parser::ASTNode *>({parserz.parse_expression()})
                    );
                } catch(const std::exception &expr_e) {
                    std::cout << "error 2: " << expr_e.what() << std::endl;
                }
            }

            core::visitor::Interpreter interpreter;
            interpreter.visit(prog);
            auto current = interpreter.current_expr();
            switch(current.first){
                case core::parser::INT:
                    std::cout << current.second.i << std::endl;
                    break;
            }

    } else {
        for(;;){
                std::string input_line;

                std::cout << "\n>>> _\b";
                std::getline(std::cin, source);

                if(source == "") {
                    continue;
                }

                if(source == ".q") {
                    break;
                } else {
                    //std::cout << source << std::endl;

                    core::parser::ASTProgramNode *prog;

                    core::lexer::Lexer expr_lexer(source);   
                    core::parser::Parser parser = core::parser::Parser(&expr_lexer);

                    try {
                        prog = parser.parse_program();
                    } catch(const std::exception &e) {
                        //std::cout << "error 1: " << e.what() << std::endl;

                        try {
                            core::lexer::Lexer expr_lexerz(source);
                            core::parser::Parser parserz = core::parser::Parser(&expr_lexerz, 0);

                            prog = new core::parser::ASTProgramNode(
                                std::vector<core::parser::ASTNode *>({parserz.parse_expression()})
                            );
                        } catch(const std::exception &expr_e) {
                            //std::cout << "error 2: " << expr_e.what() << std::endl;
                        }
                    }

                    //std::cout << "====================================" << std::endl;

                    /* Debugger AST */
                    core::visitor::ASTVisitor ast_visitor;
                    ast_visitor.visit(prog);

                    core::visitor::Interpreter interpreter(&global_scope);
                    interpreter.visit(prog);

                    auto current = interpreter.current_expr();
                    switch(current.first){
                        case core::parser::INT:
                            std::cout << current.second.i << std::endl;
                            break;
                    }

                    /*core::compiler::Compiler compiler(core::compiler::ARCH_X86_64);
                    compiler.visit(prog);*/

                    // clear IDLE commandline
                    source = "";
                    input_line = "";
                }
            }
    }

    return 0;
}

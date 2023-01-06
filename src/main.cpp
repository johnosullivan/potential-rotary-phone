#include <iostream>
#include <fstream>
#include <regex>
#include <ctime>

#include <sstream>
#include <filesystem>

/* include core/common headers */
#include "common/common.h"
#include "common/table.h"
#include "core/lexer/lexer.h"
#include "core/parser/parser.h"
#include "core/parser/ast.h"
#include "core/parser/astvisitor.h"
#include "core/repl/interpreter.h"

#include "compiler/compiler.h"

const std::string current_datetime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

int main(int argc, char* argv[]) {
    /* initialized loguru logger */
    loguru::init(argc, argv);

    /* gets current time and converts to string */
    time_t now = time(0);
    tm *ltm = localtime(&now);

    bool file_flag = false;
    bool verbose_flag = false;

    std::string file_path;
    int verbose_level = 0;

    for (int i = 1; i < argc; i++) {  
        if (i + 1 != argc) {
            if (strcmp(argv[i], "-f") == 0) {                 
                file_path = argv[i + 1];
                file_flag = true;
                i++;
            }
            // Disable verbose flag - loguru
            /*if (strcmp(argv[i], "-v") == 0) {                 
                verbose_flag = true;
                verbose_level = atoi(argv[i + 1]);
                i++;
            }*/
        }
    }

    /* output the details header */ 
    if (!file_flag) {
        std::cout << "Facile (fx) Simple Programming, Version 0.0.0 (" << current_datetime() << ")." << std::endl;
        std::cout << "Type \"help\", \"copyright\", \"credits\" or \"license\" for more information." << std::endl;
    }

    std::string source;
    core::visitor::Scope global_scope;

    // check if a source file is defined
    if(file_flag) {
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

            /* Debugger AST mapping */
            core::visitor::ASTVisitor ast_visitor;
            ast_visitor.visit(prog);

            core::visitor::Interpreter interpreter;
            interpreter.visit(prog);
            auto current = interpreter.current_expr();
            switch(current.first){
                case core::parser::INT:
                    std::cout << current.second.i << std::endl;
                    break;
                case core::parser::FLOAT:
                    std::cout << current.second.f << std::endl;
                    break;
                case core::parser::STRING:
                    std::cout << current.second.s << std::endl;
                    break;
                case core::parser::BOOL:
                    std::cout << (current.second.b ? "true" : "false") << std::endl;
                    break;
                default:
                    break;
            }

            core::compiler::Compiler compiler(core::compiler::ARCH_X86_64);
            compiler.visit(prog);
            compiler.stdout_vector();
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
                } else if(source == ".q") {
                    break;
                } else if (source == ".s") {
                    TextTable vt('-', '|', 'o');
                    vt.add("Name");
                    vt.add("Type");
                    vt.add("Value");
                    vt.endOfRow();
                    for(auto var : global_scope.all_variable_list()) {
                        vt.add(std::get<0>(var));
                        vt.add(std::get<1>(var));
                        vt.add(std::get<2>(var));
                        vt.endOfRow();
                    }
                    vt.setAlignment(2, TextTable::Alignment::RIGHT);
                    std::cout << vt << std::endl;
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

                    /* Debugger AST mapping */
                    core::visitor::ASTVisitor ast_visitor;
                    ast_visitor.visit(prog);

                    core::visitor::Interpreter interpreter(&global_scope);
                    interpreter.visit(prog);

                    auto current = interpreter.current_expr();
                    switch(current.first){
                        case core::parser::INT:
                            std::cout << current.second.i << std::endl;
                            break;
                        case core::parser::FLOAT:
                            std::cout << current.second.f << std::endl;
                            break;
                        case core::parser::STRING:
                            std::cout << current.second.s << std::endl;
                            break;
                        case core::parser::BOOL:
                            std::cout << (current.second.b ? "true" : "false") << std::endl;
                            break;
                        default:
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

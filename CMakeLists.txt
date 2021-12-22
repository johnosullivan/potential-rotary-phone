cmake_minimum_required(VERSION 3.8)
project(facile)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(fx
        # logging class
        src/lib/loguru.cpp
        # main program
        src/main.cpp
        # lexer fx
        src/core/lexer/lexer.cpp
        src/core/lexer/token.cpp
        # ast / parser
        src/core/parser/ast.cpp
        src/core/parser/astvisitor.cpp
        src/core/parser/parser.cpp
        src/core/repl/interpreter.cpp
        src/compiler/registers.cpp
        src/compiler/compiler.cpp
)
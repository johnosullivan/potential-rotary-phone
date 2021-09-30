cmake_minimum_required(VERSION 3.8)
project(facile)

set(CMAKE_CXX_STANDARD 17)

add_executable(fx
        src/main.cpp

        src/core/lexer/lexer.cpp
        src/core/lexer/token.cpp

        src/core/parser/ast.cpp
        src/core/parser/astvisitor.cpp
        
        src/core/parser/parser.cpp

        src/core/repl/interpreter.cpp

        src/compiler/compiler.cpp
)
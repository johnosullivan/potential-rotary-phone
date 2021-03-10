cmake_minimum_required(VERSION 3.8)
project(mylang)

set(CMAKE_CXX_STANDARD 17)

add_executable(mylang
        src/main.cpp

        src/core/lexer/lexer.cpp
        src/core/lexer/token.cpp
)
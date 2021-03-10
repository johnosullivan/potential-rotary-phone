#include "token.h"

#include <iostream>

using namespace core::lexer;

Token::Token(int final_state, std::string value, unsigned int line_number) :
    type(token_type(final_state, value)),
    value(value),
    line_number(line_number)
{}

TOKEN Token::token_type(int final_state, std::string &value) {
    std::cout << "value " << value << std::endl;
    std::cout << "final_state " << final_state << std::endl;

    switch(final_state) {
        case 1:
            return TK_INT;
        case 2:
            return TK_ADDITIVE_OP;
        case 22:
            return TK_EOF;
        default:
            return TK_ERROR;
    }
}

Token::Token() = default;
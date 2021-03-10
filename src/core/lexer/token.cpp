#include "token.h"

#include <iostream>

using namespace core::lexer;

Token::Token(int final_state, std::string value, unsigned int line_number):
    type(token_type(final_state, value)),
    value(value),
    line_number(line_number)
{}

TOKEN Token::token_type(int final_state, std::string &value) {
    /* EOF 100 */
    switch(final_state) {
        case 1:
            return TK_INT;
        case 2:
            return TK_ADDITIVE_OP;
        case 100:
            return TK_EOF;
        default:
            return TK_ERROR;
    }
}

std::string Token::get_tk_type_as_string() {
    switch(type) {
        case TK_INT:
            return "TK_INT";
        case TK_ADDITIVE_OP:
            return "TK_ADDITIVE_OP";
        case TK_EOF:
            return "TK_EOF";
        default:
            return "TK_ERROR";
    }
}

Token::Token() = default;
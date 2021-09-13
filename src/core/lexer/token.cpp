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

    std::cout << "Final State:" << final_state << std::endl;

    switch(final_state) {
        case 1:
            return TK_INT;
        case 2:
            return TK_ADDITIVE_OP;
        case 4:
            if(value == "var")
                return TK_VAR;
            if(value == "int")
                return TK_INT_TYPE;
            else return TK_IDENTIFIER;
        case 5:
            return TK_EQUALS;
        case 6:
            if(value == ":")
                return TK_COLON;
            if(value == ";")
                return TK_SEMICOLON;
        case 100:
            return TK_EOF;
        default:
            return TK_ERROR;
    }
}

std::string Token::get_tk_type_as_string() {
    return ENUM_STR[type];
}

Token::Token() = default;
#include <iostream>

#include "common/common.h"
#include "core/lexer/token.h"

using namespace core::lexer;

Token::Token(int final_state, std::string value, unsigned int line_number):
    type(token_type(final_state, value)),
    value(value),
    line_number(line_number)
{}

TOKEN Token::token_type(int final_state, std::string &value) {
    /* EOF 100 */

    LOG_F(INFO, "%s %d", value.c_str(), final_state);

    switch(final_state) {
        case 1:
            return TK_INT;
        case 3:
            return TK_FLOAT;
        case 4:
            return TK_ADDITIVE_OP;
        case 5:
            return TK_MULTIPLICATIVE; 
        case 7:
            return TK_RELATIONAL_OP;
        case 8:
            return TK_EQUALS;
        case 10:
            if(value == "fn") {
                return TK_FUNC;
            }
            if(value == "return") {
                return TK_RETURN;
            }
            if(value == "print") { // NOTE: souljaboytellem
                return TK_PRINT;
            }
            if(value == "var") {
                return TK_VAR;
            }
            if(value == "val") {
                return TK_VAL_TYPE;
            }
            if(value == "int") {
                return TK_INT_TYPE;
            }
            if(value == "bool") {
                return TK_BOOL_TYPE;
            }
            if(value == "true" || value == "false") {
                return TK_BOOL;
            }
            if(value == "float") {
                return TK_FLOAT_TYPE;
            } 
            return TK_IDENTIFIER;
        case 11:
            return TK_MULTIPLICATIVE; 
        case 14:
        case 16:
            return TK_COMMENT_LINE;
        case 21:
            if(value == ":")
                return TK_COLON;
            if(value == ";")
                return TK_SEMICOLON;
            if(value == "{")
                return TK_LEFT_CURLY;
            if(value == "}")
                return TK_RIGHT_CURLY;
            if(value == "(")
                return TK_LEFT_PARENTHESES;
            if(value == ")")
                return TK_RIGHT_PARENTHESES;
            if(value == ",")
                return TK_COMMA;
        case 20:
            return TK_STRING;
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
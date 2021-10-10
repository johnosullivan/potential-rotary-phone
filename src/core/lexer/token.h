#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace core::lexer {

    enum TOKEN {
        TK_INT = 0,                   
        TK_ADDITIVE_OP = 1, 
        TK_MULTIPLICATIVE_OP = 2, 
        TK_RELATIONAL_OP = 3,  
        TK_ERROR = 4,  
        TK_EOF = 5,
        TK_INT_TYPE = 6,
        TK_VAR = 7,
        TK_IDENTIFIER = 8,
        TK_COLON = 9,
        TK_SEMICOLON = 10,
        TK_EQUALS = 11,
        TK_FLOAT_TYPE = 12,
        TK_FLOAT = 13,
        TK_STRING_TYPE = 14,
        TK_STRING = 15,
        TK_VAL_TYPE = 16,
        TK_LEFT_CURLY = 17,
        TK_RIGHT_CURLY = 18,
        TK_LEFT_PARENTHESES = 19,
        TK_RIGHT_PARENTHESES = 20,
        TK_PRINT = 21,
        TK_FUNC = 22,
        TK_RETURN = 23,
        TK_COMMA = 24
    };

    static const char *ENUM_STR[] = {
        "TK_INT",                   
        "TK_ADDITIVE_OP", 
        "TK_MULTIPLICATIVE_OP", 
        "TK_RELATIONAL_OP",  
        "TK_ERROR",  
        "TK_EOF",
        "TK_INT_TYPE",
        "TK_VAR",
        "TK_IDENTIFIER",
        "TK_COLON",
        "TK_SEMICOLON",
        "TK_EQUALS",
        "TK_FLOAT_TYPE",
        "TK_FLOAT",
        "TK_STRING_TYPE",
        "TK_STRING",
        "TK_VAL_TYPE",
        "TK_LEFT_CURLY",
        "TK_RIGHT_CURLY",
        "TK_LEFT_PARENTHESES",
        "TK_RIGHT_PARENTHESES",
        "TK_PRINT",
        "TK_FUNC",
        "TK_RETURN",
        "TK_COMMA"
    };

    class Token {
        public:
            Token();
            Token(int, std::string, unsigned int line_number = 0);
            TOKEN type;
            std::string value;
            unsigned int line_number;

            /* debugging tools */
            std::string get_tk_type_as_string();
        private:
            TOKEN token_type(int, std::string&);
    };
};


#endif //TOKEN_H

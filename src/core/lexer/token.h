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
        "TK_EQUALS"
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

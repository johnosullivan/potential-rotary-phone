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

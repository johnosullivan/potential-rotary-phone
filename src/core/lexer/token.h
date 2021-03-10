#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace core::lexer {

    enum TOKEN {
        TK_INT,                   
        TK_ADDITIVE_OP, 
        TK_MULTIPLICATIVE_OP, 
        TK_RELATIONAL_OP,  
        TK_ERROR,  
        TK_EOF 
    };

    class Token {

        public:
            Token();
            Token(int, std::string, unsigned int line_number = 0);
            TOKEN type;
            std::string value;
            unsigned int line_number;

        private:
            TOKEN token_type(int, std::string&);
    };
};


#endif //TOKEN_H

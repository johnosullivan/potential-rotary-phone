#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "token.h"

namespace core::lexer {

    enum TRANSITION_TYPE {
        DIGIT           = 0,
        ADDITIVE_OP     = 1, 
        ENDOFFILE       = 2, 
        OTHER           = 3,
        LETTER          = 4,
        EQUALS          = 5,  
        PUNCTUATION     = 6,
    };

    class Lexer {
        public:
            Lexer(std::string&);

            Token next_token();

            /**
             * Default deconstructor.
             */
            ~Lexer();
        private:
            /**
             * Default Error Code 
            */
            const unsigned int e = 94;
            
            const bool in_final_state[7] = { 0, 1,  1, 0, 1, 1, 1 };

            /* EOF 100 */
            const unsigned int transitions[7][7] = {
                /* 0 T_DIGIT          */ {   1,  1,  e,  e,  e, e, e },
                /* 1 T_ADDITIVE_OP    */ {   2,  e,  e,  e,  e, e, e },
                /* 2 T_END_OF_FILE    */ {   e,  e,  e,  e,  e, e, e },
                /* 3 T_OTHER          */ {   e,  e,  e,  e,  e, e, e },
                /* 4 T_LETTER         */ {   4,  e,  e,  e,  4, e, e },
                /* 5 T_EQUALS         */ {   5,  e,  e,  e,  e, e, e },
                /* 6 T_PUNCTUATION    */ {   6,  e,  e,  e,  e, e, e },
            };

            unsigned int current_token = 0;

            std::vector<Token> tokens;

            int find_transition(int, char);

            Token next_token(std::string&, unsigned int&);

            unsigned int get_source_line_number(std::string&, unsigned int);
    };
};


#endif //LEXER_H
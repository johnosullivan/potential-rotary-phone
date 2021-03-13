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
        OTHER           = 3  
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
            const unsigned int e = 94;

            const bool in_final_state[6] = { 0,  1,  1 };

            /* EOF 100 */
            const unsigned int transitions[4][5] = {
                /* T_DIGIT          */ {   1,  e,  e,  e,  e },
                /* T_ADDITIVE_OP    */ {   2,  e,  e,  e,  e },
                /* T_ENDOFFILE      */ {   e,  e,  e,  e,  e },
                /* T_OTHER          */ {   e,  e,  e,  e,  e }
            };

            unsigned int current_token = 0;

            std::vector<Token> tokens;

            int find_transition(int, char);

            Token next_token(std::string&, unsigned int&);

            unsigned int get_source_line_number(std::string&, unsigned int);
    };
};


#endif //LEXER_H
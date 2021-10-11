#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "common/common.h"
#include "token.h"

namespace core::lexer {

    enum TRANSITION_TYPE {
        TT_DIGIT           =  0,
        TT_PERIOD          =  1,
        TT_ADDITIVE_OP     =  2,
        TT_ASTERISK        =  3,
        TT_EXCL_MARK       =  4,
        TT_ORDER_REL       =  5,
        TT_EQUALS          =  6,
        TT_UNDERSCORE      =  7,
        TT_FORWARDSLASH    =  8,
        TT_BACKSLASH       =  9,
        TT_QUOTATION_MARK  = 10,
        TT_PUNCTUATION     = 11,
        TT_NEWLINE         = 12,
        TT_ENDOFFILE       = 13,
        TT_LETTER          = 14,
        TT_PRINTABLE       = 15,
        TT_OTHER           = 16
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
            
            //const bool in_final_state[9] = { 0, 1,  1, 0, 1, 1, 1, 1, 1 };
            /* 
                EOF 100 
                https://en.wikipedia.org/wiki/Finite-state_machine
            */
            //const unsigned int transitions[9][9] = {
                /* 0 T_DIGIT          */ //{  1,  1,  e,  e,  e, e, e, 7, e },
                /* 1 T_ADDITIVE_OP    */ //{  2,  e,  e,  e,  e, e, e, e, e },
                /* 2 T_END_OF_FILE    */ //{  e,  e,  e,  e,  e, e, e, e, e },
                /* 3 T_OTHER          */ //{  3,  e,  e,  3,  e, e, e, e, e }, 
                /* 4 T_LETTER         */ //{  4,  e,  e,  4,  4, e, e, e, e },
                /* 5 T_EQUALS         */ //{  5,  e,  e,  e,  e, e, e, e, e },
                /* 6 T_PUNCTUATION    */ //{  6,  7,  e,  e,  6, e, 6, e, e },
                /* 7 T_FLOAT          */ //{  7,  7,  7,  e,  e, e, e, e, e },
                /* 8 T_STRING         */ //{  e,  e,  e,  e,  e, e, e, e, e }
            //};

            const bool in_final_state[24] = { 0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  0,  0,  1,  0,  1,  0,  0,  0,  1,  1,  1, 0 };

            const unsigned int transitions[17][23] = {
                                     /* S0  S1  S2  S3  S4  S5  S6  S7  S8  S9 S10 S11 S12 S13 S14 S15 S16 S17 S18 S19 S20 S21 S22 */
                /* TT_DIGIT          */ {   1,  1,  3,  3,  e,  e,  e,  e,  e,  e, 10,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_PERIOD         */ {   2,  3,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_ADDITIVE_OP    */ {   4,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_ASTERISK       */ {   5,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 13, 12, 15,  e, 15,  e, 17, 17, 17,  e,  e,  e},
                /* TT_EXCL_MARK      */ {   6,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_ORDER_REL      */ {   7,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_EQUALS         */ {   8,  e,  e,  e,  e,  e,  9,  9,  9,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_UNDERSCORE     */ {  10,  e,  e,  e,  e,  e,  e,  e,  e,  e, 10,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_FORWARDSLASH   */ {  11,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 12, 13,  e, 16,  e, 17, 17, 17,  e,  e,  e},
                /* TT_BACKSLASH      */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 18, 18, 18,  e,  e,  e},
                /* TT_QUOTATION_MARK */ {  17,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 20, 19, 20,  e,  e,  e},
                /* TT_PUNCTUATION    */ {  21,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_NEWLINE        */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 14, 13,  e, 13,  e,  e,  e,  e,  e,  e,  e},
                /* TT_ENDOFFILE      */ {  22,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e},
                /* TT_LETTER         */ {  10,  e,  e,  e,  e,  e,  e,  e,  e,  e, 10,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_PRINTABLE      */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e, 12, 13,  e, 13,  e, 17, 17, 17,  e,  e,  e},
                /* TT_OTHER          */ {   e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e,  e}
            };

            unsigned int current_token = 0;

            std::vector<Token> tokens;

            int find_transition(int, int, char);

            Token next_token(std::string&, unsigned int&);

            unsigned int get_source_line_number(std::string&, unsigned int);
    };
};


#endif //LEXER_H
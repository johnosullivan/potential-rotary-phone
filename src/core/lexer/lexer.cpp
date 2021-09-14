#include <stack>
#include <stdexcept>
#include <iostream>
#include "lexer.h"

using namespace core::lexer;

Lexer::Lexer(std::string& program) {
    unsigned int current_index = 0;

    Token t;
    while(current_index <= program.length()) {
        t = next_token(program, current_index);

        //std::cout << "L: " << t.line_number << std::endl;
        
        std::cout << "V: " << t.value << std::endl;
        std::cout << "T: " << t.get_tk_type_as_string() << std::endl;
        std::cout << "======================" << std::endl;

        tokens.push_back(t);
    }
}

Token Lexer::next_token() {
    if(current_token < tokens.size())
        return tokens[current_token++];
    else{
        return Token(TK_ERROR, "");
    }
}

int Lexer::find_transition(int state, char symbol) {

    //std::cout << "state: " << state << " symbol: " << symbol << std::endl;

    switch(symbol){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            //std::cout << "DIGIT -> " << state << std::endl;
            return transitions[DIGIT][state];
        }
        case '+':
        case '-':
            return transitions[ADDITIVE_OP][state];
        case '=': {
            //std::cout << "EQUALS -> " << state << std::endl;
            return transitions[EQUALS][state];
        }
        case ':':
        case ';':
        case '.': {
            //std::cout << "PUNCTUATION -> " << state << std::endl;
            return transitions[PUNCTUATION][state];
        }
        case EOF:
            return transitions[ENDOFFILE][state];
        default:
            auto ascii = (int)symbol;

            if (((0x41 <= ascii) && (ascii <= 0x5A)) || ((0x61 <= ascii) && (ascii <= 0x7A))) {
                //std::cout << "LETTER -> " << state << std::endl;
                return transitions[LETTER][state];
            }

            return transitions[OTHER][state];
    }
}

Token Lexer::next_token(std::string &program, unsigned int &current_index) {
    int current_state = 0;
    std::stack<int> stack;
    char current_symbol;
    std::string lexeme;

    stack.push(-1);

    while(current_index < program.length() && (program[current_index] == ' ' || program[current_index] == '\n')) {
        current_index++;
    }

    if(current_index == program.length()){
        lexeme = (char)EOF;
        current_index++;
        return Token(100, lexeme, get_source_line_number(program, current_index));
    }

    while(current_state != e){
        current_symbol = program[current_index];
        lexeme += current_symbol;
       
        bool is_final_value = in_final_state[current_state];

        if (is_final_value) {
            while(!stack.empty()) {
                stack.pop();
            }
        }

        stack.push(current_state);

        current_state = find_transition(current_state, current_symbol);

        //std::cout << "current_symbol " << current_symbol << std::endl;
        //std::cout << "current_state " << current_state << std::endl;

        current_index++;
    }

    while(!in_final_state[current_state] && current_state != -1){
        current_state = stack.top();
        stack.pop();
        lexeme.pop_back();
        current_index--;
    }

    if(current_state == -1)
        throw std::runtime_error("lexical error");


    if(in_final_state[current_state]) {
        
        return Token(current_state, std::move(lexeme), get_source_line_number(program, current_index));
    } else {
        throw std::runtime_error("lexical error: line = " + std::to_string(get_source_line_number(program, current_index)) + ".");
    }
}

unsigned int Lexer::get_source_line_number(std::string &program, unsigned int index) {
    unsigned int line = 1;
    for(int i = 0; i < index; i++)
        if(program[i] == '\n')
            line++;
    return line;
}


Lexer::~Lexer() = default;
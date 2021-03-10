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

        std::cout << "L: " << t.line_number << std::endl;
        std::cout << "V: " << t.value << std::endl;
        std::cout << "T: " << t.type << std::endl;
    }
}

Token Lexer::next_token() {
    if(current_token < tokens.size())
        return tokens[current_token++];
    else{
        return Token(TK_ERROR, "");
    }
}

int Lexer::transition(int s, char sigma) {
    switch(sigma){
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return transitions[DIGIT][s];
        case '+':
        case '-':
            return transitions[ADDITIVE_OP][s];
        case EOF:
            return transitions[ENDOFFILE][s];

        default:
            return transitions[OTHER][s];
    }
}

Token Lexer::next_token(std::string &program, unsigned int &current_index) {
    int current_state = 0;
    std::stack<int> state_stack;
    char current_symbol;
    std::string lexeme;

    state_stack.push(-1);

    std::cout << "=========================================================" << std::endl;

    while(current_index < program.length() &&
          (program[current_index] == ' ' || program[current_index] == '\n'))
        current_index++;

    if(current_index == program.length()){
        lexeme = (char) EOF;
        current_index++;
        return Token(eof_int, lexeme, get_source_line_number(program, current_index));
    }

    while(current_state != e){
        current_symbol = program[current_index];
        lexeme += current_symbol;
       
        bool is_final_value = in_final_state[current_state];
        std::cout << "is_final_value " << is_final_value << std::endl;

        if (is_final_value)
            while(!state_stack.empty())
                state_stack.pop();

        state_stack.push(current_state);

        current_state = transition(current_state, current_symbol);

        std::cout << "transition_value " << current_state << std::endl;

        current_index++;
    }

    while(!in_final_state[current_state] && current_state != -1){
        current_state = state_stack.top();
        state_stack.pop();
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
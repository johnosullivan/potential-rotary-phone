#include <iostream>
#include <fstream>
#include <regex>

/* include core headers */
#include "core/lexer/lexer.h"

int main() {
    for(;;){
        std::string input_line;
        std::string source;

        std::cout << "\n>>> _\b";
        std::getline(std::cin, source);

        // input_line = std::regex_replace(input_line, std::regex("^ +| +$"), "$1");

        if(source == ".quit") {
            break;
        } else {
            source += input_line;

            std::cout << input_line << std::endl;

            core::lexer::Lexer lexer(source);
        }
    }

    return 0;
}
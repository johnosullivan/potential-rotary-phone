#include <iostream>
#include <fstream>
#include <regex>

int main() {
    for(;;){
        std::string input_line;
        std::string program;

        std::cout << "\n>>> _\b";
        std::getline(std::cin, input_line);

        input_line = std::regex_replace(input_line, std::regex("^ +| +$"), "$1");

        if(input_line == ".quit") {
            break;
        }

        std::cout << program << std::endl;
    }

    return 0;
}
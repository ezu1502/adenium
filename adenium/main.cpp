#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "tokens.hpp"
#include "lexer.hpp"
#include "parser.hpp"

std::string get_sourcecode(int c, char* v[]){
    std::string sourcecode;
    if (c < 2 || std::string(v[1]) == "-write"){
        // INFO  write mode
        std::string line;
        std::cout << "Type your code:\n";

        while(true){
            std::getline(std::cin, line);

            if (line == "end"){
                break;
            }

            sourcecode += line + "\n";
        }
    }
    else if (c == 2){
        std::string fileName = v[1];

        if (!fileName.ends_with(".adn")){
            std::cout << "Adenium can only compile .adn files!\n";
            return "error";
        }

        std::ifstream file(fileName);

        if (!file){
            std::cout << "File couldn't be opened\n";
            return "error";
        }

        sourcecode = std::string(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }
    else {
        std::cout << "\nOnly one command line argument supported\n";
        return "error";
    }


    return sourcecode;
}

int main(int argc, char* argv[]){
    std::string sourcecode = get_sourcecode(argc, argv);

    if(sourcecode == "error"){
        return 1;
    }
  
    Lexer lexer(sourcecode);
    Parser parser(lexer);

    NodePtr tree = parser.parseExpression();
}
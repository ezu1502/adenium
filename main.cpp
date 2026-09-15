#include <iostream>

#include "lexer.hpp"
#include "tokens.hpp"

int main(){
    std::string sourcecode;

    std::cout << "Type your code:\n> ";
    std::cin >> sourcecode;
    
    Lexer lexer(sourcecode);
}
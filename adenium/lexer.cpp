#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <stdexcept>
#include <vector>

#include "tokens.hpp"

std::set<std::string> keywords = {
    "if",
    "else",
    "while",
    "try",
    "except",
    "match",
    "case",
    "for",
    "import",
    "from",
    "in",
    "and",
    "asm",
    "goto",
    "Null"
};

std::set<char> operators = {
    '+',
    '-',
    '*',
    '/',
    '>',
    '<',
    '=',
    '%',
    
};

std::set<std::string> doubleOperators {
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "//",
    "**",
    "++",
    "==",
    ">=",
    "<=",
    ">>",
    "<<",
};

std::set<char> delimiters = {
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    ':',
    ',',
    '.'
};

class Lexer{
public:
    Lexer(std::string sourcecode);
    Token nextToken();

private:
    std::string sourcecode;
    int codeSize;

    int cursor;
    bool advance();

    int indentSize = 0;
    std::vector<int> indentStack = {0};
    int pendingDedents = 0;
    bool lineStart = true;
};

Lexer::Lexer(std::string sourcecode){
    this->sourcecode = sourcecode;
    this->codeSize = sourcecode.size();
    this->cursor = 0;
}

bool Lexer::advance(){
    if (cursor < codeSize){
        cursor++;
        return true;
    }
    return false;
}

Token Lexer::nextToken(){

    if(pendingDedents > 0){
        lineStart = false;
        pendingDedents--;

        return Token(TokenType::Dedent);
    }
    
    if (lineStart){
        
        int spaces = 0;
        while (cursor < codeSize && sourcecode[cursor] == ' '){
            spaces++;
            advance();
        }

        if (spaces > indentStack.back()){
            if (indentSize == 0){
                indentSize = spaces;
            }

            if (spaces % indentSize != 0){
                throw std::runtime_error("IndentationError");
            }

            indentStack.push_back(spaces);
            lineStart = false;

            

            return Token(TokenType::Indent);
        }

        if (spaces < indentStack.back()){
            
            while (spaces < indentStack.back()){
                if (spaces % indentSize != 0){
                    throw std::runtime_error("Indentation error");
                }
                indentStack.pop_back();
                pendingDedents++;
            }

            if (spaces != indentStack.back()){
                throw std::runtime_error("IndentationError");
            }
            
            lineStart = false;
            pendingDedents--;

            return Token(TokenType::Dedent);
        }

        

    }
    
    while (cursor < codeSize && std::isspace(sourcecode[cursor]) && sourcecode[cursor] != '\n'){
        advance();
    }
    // ! CHECAR ISSO AQUI
    lineStart = false;


    Token thisToken;
    char current;

    int alpha = 0;
    int digits = 0;
    int underscores = 0;
    int dots = 0;

    while (cursor < codeSize){
        current = sourcecode[cursor];

        if (std::isspace(current) && sourcecode[cursor] != '\n'){ // * FIM DO TOKEN     
            break;
        }
        
        if (current == '\n' || current == ';'){
            if (thisToken.value.empty()){
                thisToken.type = TokenType::NewLine;
                if (current == '\n'){
                    thisToken.value = "\\n";
                }
                else {
                    thisToken.value = ";";
                }
                advance();
                lineStart = true;
                return thisToken;
            }
            else {
                break;
            }
        }

        if (current == '#'){
            if (cursor + 1 < codeSize && sourcecode[cursor+1] == '#'){
                // Comentário que só fecha quando encontrar outro ##
                
                advance();
                advance();

                while (cursor < codeSize){
                    if (cursor + 1 < codeSize && sourcecode[cursor] == '#' && sourcecode[cursor+1] == '#'){
                        advance();
                        advance();
                        break;
                    }
                    advance();

                }
            }
            else {
                while (true){
                    advance();
                    
                    if (cursor + 1 < codeSize && sourcecode[cursor+1] == '\n'){
                        advance();
                        break;
                    }

                    if (cursor + 1 == codeSize){
                        return Token(TokenType::Comment);
                    }
                }
            }
            
            return Token(TokenType::Comment);
        }

        if (current == '\"'){
            advance();
            while (cursor < codeSize && sourcecode[cursor] != '\"'){
                thisToken.value += sourcecode[cursor];
                if (!advance()){
                    // ! syntax error!
                    throw std::runtime_error("Unclosed quotes");
                }
            }

            if (cursor >= codeSize){
                throw std::runtime_error("Unclosed quotes");
            }

            thisToken.type = TokenType::String;
            advance();
            return thisToken;
        }
        if (current == '\''){
            advance();
            while (cursor < codeSize && sourcecode[cursor] != '\''){
                thisToken.value += sourcecode[cursor];
                if (!advance()){
                    // ! syntax error!
                    throw std::runtime_error("Unclosed quotes");
                }
            }
            
            if (cursor >= codeSize){
                throw std::runtime_error("Unclosed quotes");
            }
            
            thisToken.type = TokenType::String;
            advance();
            return thisToken;
        }


        if (operators.contains(current)){
            if (thisToken.value.empty()){

                Token operatorToken;
                operatorToken.type = TokenType::Operator;

                operatorToken.value += current;

                if (advance() && operators.contains(sourcecode[cursor])){
                    std::string candidate = operatorToken.value;
                    candidate += sourcecode[cursor];
                    
                    
                    if (doubleOperators.contains(candidate)){
                        operatorToken.value = candidate;
                        advance();
                    }
                    else {
                        // ! syntax error
                    }
                    
                }

                return operatorToken;
                
            }
            else {
                break;
            }
        }

        if (delimiters.contains(current)){
            if (current == '.'){
                bool decimal = (
                    cursor > 0 &&
                    cursor + 1 < codeSize &&
                    std::isdigit(sourcecode[cursor - 1]) &&
                    std::isdigit(sourcecode[cursor + 1])
                );
                
                if (!decimal){
                    if (thisToken.value.empty()){
                        thisToken.value = current;
                        thisToken.type = TokenType::Delimiter;
                        advance();
                        return thisToken;
                    }
                    else{
                        break;
                    }
                }
            }
            else {
                if (thisToken.value.empty()){
                    thisToken.value = current;
                    thisToken.type = TokenType::Delimiter;
                    advance();
                    return thisToken;
                }
                else{
                    break;
                }
            }
            
        }


        if (std::isdigit(current)){
            digits++;
        }
        else if (std::isalpha(current)){
            alpha++;
        }
        else if(current == '_'){
            underscores++;
        }
        else if(current == '.'){
            dots++;
        }



        thisToken.value += current;
        advance();
    }

    if (keywords.contains(thisToken.value)){
        thisToken.type = TokenType::Keyword;
    }
    else if (alpha == 0 && digits > 0){
        thisToken.type = TokenType::Number;

        thisToken.value.erase(
            std::remove(thisToken.value.begin(), thisToken.value.end(), '_'),
            thisToken.value.end()
        );

        if (dots > 1){
            throw std::runtime_error("Syntax error");
        }
    }
    else if (alpha != 0  || (underscores > 0 && digits == 0)){
        thisToken.type = TokenType::Identifier;
    }

    if (thisToken.value == ""){
        thisToken.type = TokenType::End;
        thisToken.value = "UGA";
    }
    return thisToken;
}

int main(int argc, char* argv[]){
    std::string sourcecode;
    if (argc < 2 || std::string(argv[1]) == "-write"){
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
    else if (argc == 2){
        std::string fileName = argv[1];

        if (!fileName.ends_with(".adn")){
            std::cout << "Adenium can only compile .adn files!\n";
            return 1;
        }

        std::ifstream file(fileName);

        if (!file){
            std::cout << "File couldn't be opened\n";
            return 1;
        }

        sourcecode = std::string(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }
    else {
        std::cout << "\nOnly one command line argument supported\n";
        return 1;
    }
  
    Lexer lexer(sourcecode);
    
    Token n;
    
    while (true){
        n = lexer.nextToken();
        
        std::cout << tokenTypeName(n.type) << ", \"" << n.value << "\"\n";

        if (n.type == TokenType::End){
            break;
        }
    }

}
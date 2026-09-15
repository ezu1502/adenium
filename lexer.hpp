#include <string>
#include "tokens.hpp"

class Lexer{
public:
    Lexer(std::string sourcecode);
    Token nextToken();

private:
    std::string sourcecode;

    int cursor;
    bool advance();
};

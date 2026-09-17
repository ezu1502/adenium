#pragma once

#include <string>
#include "tokens.hpp"
#include <vector>

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
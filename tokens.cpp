#include "tokens.hpp"
#include <string>


std::string tokenTypeName(TokenType type){
    switch (type){
        case TokenType::Delimiter:
            return "Delimiter";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Keyword:
            return "Keyword";
        case TokenType::Number:
            return "Number";
        case TokenType::Operator:
            return "Operator";
        case TokenType::String:
            return "String";
        case TokenType::Comment:
            return "Comment";
        case TokenType::NewLine:
            return "Newline";
        case TokenType::End:
            return "End";
    }
    return "Unknown";
}
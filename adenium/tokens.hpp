#include <string>

enum class TokenType {
    Identifier,
    Number,
    Operator,
    Delimiter,
    Keyword,
    String,
    Comment,
    
    NewLine,
    
    Indent,
    Dedent,

    End,
};

class Token {
public:
    TokenType type;
    std::string value;
};

std::string tokenTypeName(TokenType type);
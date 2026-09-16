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
    
    JumpTag,

    End,
};

std::string tokenTypeName(TokenType type);

class Token {
public:
    Token() = default;

    Token(TokenType type) : type(type), value(tokenTypeName(type)) {}


    Token(TokenType type, std::string value) : type(type), value(value) {}

    TokenType type;
    std::string value;
};


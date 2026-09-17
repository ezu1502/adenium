#pragma once

#include <memory>
#include <string>

#include "tokens.hpp"
#include "lexer.hpp"

class Node {
public:
    Node() = default;
    virtual ~Node() = default;
};

using NodePtr = std::unique_ptr<Node>;

class IdentifierNode : public Node {
public:
    IdentifierNode() = default;

    IdentifierNode(const Token& token);

private:
    std::string name;
};

class NumberNode : public Node {
public:
    NumberNode() = default;

    NumberNode(const Token& token);

private:
    int value;
};

class AssignmentNode : public Node {
public:
    AssignmentNode() = default;

    AssignmentNode(NodePtr target, NodePtr value);

private:
    NodePtr target;
    NodePtr value;
};

class BinaryNode : public Node {
public:
    BinaryNode() = default;

    BinaryNode(NodePtr left, NodePtr right, std::string op);

private:
    NodePtr left;
    NodePtr right;

    std::string op;
};

class Parser {
public:
    Parser(Lexer& lexer);
    NodePtr parseExpression();

private:
    using enum TokenType;

    Lexer& lexer;
    Token currentToken;

    void walk();
    bool check(TokenType type);

    NodePtr parsePrimary();

    NodePtr parseNumber();
    NodePtr parseIdentifier();
    NodePtr parseBinary();

};

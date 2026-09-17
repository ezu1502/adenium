#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "tokens.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void raise(const std::string& message = "Something went wrong"){
    throw std::runtime_error(message);
}
using NodePtr = std::unique_ptr<Node>;

IdentifierNode::IdentifierNode(const Token& token) : name(token.value){}

NumberNode::NumberNode(const Token& token) : value(std::stoi(token.value)) {}

AssignmentNode::AssignmentNode(NodePtr target, NodePtr value) : target(std::move(target)), value(std::move(value)){}

BinaryNode::BinaryNode(NodePtr left, NodePtr right, std::string op) : left(std::move(left)), right(std::move(right)), op(std::move(op)) {}



Parser::Parser(Lexer& lexer) : lexer(lexer){
    walk();
    // anda pro primeiro token
}

void Parser::walk(){
    currentToken = lexer.nextToken();
}

bool Parser::check(TokenType type){
    return currentToken.type == type;
}

NodePtr Parser::parsePrimary(){
    NodePtr node;

    switch (currentToken.type){
        case Number:
            node = parseNumber();
            break;
        case Identifier:
            node = parseIdentifier();
            break;

        default:
            raise("Unexpected Token");
    }

    walk();

    return node;
}


NodePtr Parser::parseNumber(){
    return std::make_unique<NumberNode>(currentToken);
}

NodePtr Parser::parseIdentifier(){
    return std::make_unique<IdentifierNode>(currentToken);
}

NodePtr Parser::parseBinary(){
    NodePtr left = parsePrimary();


    while (check(Operator)){
        std::string op = currentToken.value;
        walk(); 

        NodePtr right = parsePrimary();


        left = std::make_unique<BinaryNode>(
            std::move(left),
            std::move(right),
            std::move(op)
        );
    }

    return left;

    // TODO adicionar precedência
    // TODO checar bugs no código
    
}


NodePtr Parser::parseExpression(){
    return parseBinary();
}

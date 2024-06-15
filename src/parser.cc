#include <parser.hh>
#include <unordered_map>
#include <functional>

parser::Parser::Parser(std::vector<token::Token> &tokens) {
    this->tokens = tokens;
}

void parser::Parser::consume() {
    pos++;
}

token::TokenType parser::Parser::current() {
    return tokens.at(pos).type;
}

std::string parser::Parser::lcurrent() {
    return tokens.at(pos).lexeme;
}

parser::Node parser::Parser::parseIf() {
    parser::If node;
    consume();

}

parser::Node parser::Parser::parseStatements() {
    Node node;
    token::TokenType type = current();

    switch (type) {
        case token::TokenType::TK_DATATYPE:
            node = parseVar();
            break;
        case token::TokenType::TK_KEYWORD:
            static std::unordered_map<std::string, std::function<parser::Node>> map = {

            };
            
    }
}

parser::Node parser::Parser::parseVar() {

}
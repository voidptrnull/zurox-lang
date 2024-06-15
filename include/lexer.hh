#ifndef LEXER_HH
#define LEXER_HH

#include <string>
#include <vector>
#include <token.hh>
#include <print.hh>

namespace lexer
{
    class Lexer
    {
    public:
        Lexer(const std::string &file_name, util::PrintGlobalState &print);
        std::vector<token::Token> lex();

    private:
        size_t line, col;
        std::vector<token::Token> tokens;
        std::string file;
        std::string file_name;
        util::PrintGlobalState &print;

        inline char current() const;
        inline char peek() const;
        inline char previous() const;
        inline void advance();

        void keywordOrDatatypeOrIdentifier();
        void number();
        bool isSeperator(char c) const;
        bool isOperator(char c) const;
        void handleOperator();
        void handleStringLiteral();
        void handleCharLiteral();
        void handleComment();
    };
}

#endif
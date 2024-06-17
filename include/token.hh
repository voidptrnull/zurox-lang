#ifndef TOKEN_HH
#define TOKEN_HH

typedef unsigned long long int_t;

enum TokenType
{
    TK_DATATYPE,
    TK_KEYWORD,
    TK_SEPARATOR,
    TK_OPERATOR,
    TK_ID,

    TKL_INT,
    TKL_FLOAT,
    TKL_CHAR,
    TKL_STR,

    TK_ERR,
    __EOF,
};

#include <string>

struct Token
{
    TokenType type;
    int_t line;
    int_t col;
    std::string lexeme;
    Token(TokenType _type, int_t _line, int_t _col, const std::string &_lexeme)
        : type(_type), line(_line), col(_col), lexeme(_lexeme) {}
    Token(TokenType _type, int_t _line, int_t _col, const char &_lexeme)
        : type(_type), line(_line), col(_col)
    {
        lexeme.push_back(_lexeme);
    }
    Token() {}
};

#endif
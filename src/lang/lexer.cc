/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */

#include <array>
#include <algorithm>
#include <definitions.hh>
#include <lexer.hh>
#include <version.hh>

constexpr std::array<bool, 256> createAlphaTable()
{
    std::array<bool, 256> table = {};
    for (char c = 'a'; c <= 'z'; ++c)
        table[static_cast<unsigned char>(c)] = true;
    for (char c = 'A'; c <= 'Z'; ++c)
        table[static_cast<unsigned char>(c)] = true;
    table[static_cast<unsigned char>('_')] = true;
    return table;
}

constexpr std::array<bool, 256> createAlnumTable()
{
    std::array<bool, 256> table = createAlphaTable();
    for (char c = '0'; c <= '9'; ++c)
        table[static_cast<unsigned char>(c)] = true;
    return table;
}

constexpr std::array<bool, 256> createDigitTable()
{
    std::array<bool, 256> table = {};
    for (char c = '0'; c <= '9'; ++c)
        table[static_cast<unsigned char>(c)] = true;
    return table;
}

constexpr std::array<bool, 256> createXdigitTable()
{
    std::array<bool, 256> table = createDigitTable();
    for (char c = 'a'; c <= 'f'; ++c)
        table[static_cast<unsigned char>(c)] = true;
    for (char c = 'A'; c <= 'F'; ++c)
        table[static_cast<unsigned char>(c)] = true;
    return table;
}

static constexpr std::array<bool, 256> is_alpha_table = createAlphaTable();
static constexpr std::array<bool, 256> is_alnum_table = createAlnumTable();
static constexpr std::array<bool, 256> is_digit_table = createDigitTable();
static constexpr std::array<bool, 256> is_xdigit_table = createXdigitTable();

constexpr inline bool __is_alpha(unsigned char c)
{
    return is_alpha_table[c];
}

constexpr inline bool __is_alnum(unsigned char c)
{
    return is_alnum_table[c];
}

constexpr inline bool __is_digit(unsigned char c)
{
    return is_digit_table[c];
}

constexpr inline bool __is_xdigit(unsigned char c)
{
    return is_xdigit_table[c];
}

Lexer::Lexer(const std::string &file, const std::string &file_name, PrintGlobalState &print)
    : line(1), col(0), file(file), file_name(file_name), print(print)
{
    tokens.reserve(file.length() / LEXER_SIZE_APPROX_FACTOR);
}

std::vector<Token> Lexer::lex()
{
    while (col < file.length())
    {
        char c = current();
        if (__is_alpha(c))
        {
            keywordOrDatatypeOrIdentifier();
        }
        else if (__is_digit(c))
        {
            number();
        }
        else if (isSeperator(c))
        {
            tokens.emplace_back(TokenType::TK_SEPARATOR, line, col, std::string(1, current()));
            advance();
        }
        else if (isOperator(c))
        {
            handleOperator();
        }
        else if (isspace(c))
        {
            if (c == '\n')
            {
                line++;
            }
            advance();
        }
        else if (c == '"')
        {
            handleStringLiteral();
        }
        else if (c == '\'')
        {
            handleCharLiteral();
        }
        else
        {
            print.error("Unexpected character found.", line, col + 1, file);
            tokens.emplace_back(TokenType::TK_ERR, line, col, std::string(1, current()));
            advance();
        }
    }
    tokens.emplace_back(TokenType::__EOF, line, col, "");
    return tokens;
}

char Lexer::current() const
{
    if (col < file.length())
    {
        return file.at(col);
    }
    return '\0'; // Return null character if out of bounds
}

char Lexer::peek() const
{
    return col + 1 < file.size() ? file[col + 1] : '\0';
}

char Lexer::previous() const
{
    return col > 0 ? file.at(col - 1) : '\0'; // Return null character if at beginning
}

void Lexer::advance()
{
    col++;
}

void Lexer::keywordOrDatatypeOrIdentifier()
{
    std::string str;
    str.reserve(8);

    while (__is_alnum(current()) || current() == '_')
    {
        str.push_back(current());
        advance();
    }

    std::string_view sv(str);
    if (auto dt = find_dt(sv); dt)
    {
        tokens.emplace_back(TokenType::TK_DATATYPE, line, col - str.length(), str);
    }
    else if (auto keyword = find_keyword(sv); keyword)
    {
        tokens.emplace_back(TokenType::TK_KEYWORD, line, col - str.length(), str);
    }
    else
    {
        tokens.emplace_back(TokenType::TK_ID, line, col - str.length(), str);
    }
}

void Lexer::number()
{
    std::string str;
    str.reserve(8);

    if (current() == '0')
    {
        advance();
        if (current() == 'x')
        { // Hexadecimal integer or float
            str.push_back('0');
            str.push_back('x');
            advance();
            while (__is_xdigit(current()))
            {
                str.push_back(current());
                advance();
            }
            tokens.emplace_back(TokenType::TKL_INT, line, col - str.length(), str);
            return;
        }
        else if (current() == 'o')
        { // Octal integer
            str.push_back('0');
            str.push_back('o');
            advance();
            while (current() >= '0' && current() <= '7')
            {
                str.push_back(current());
                advance();
            }
            tokens.emplace_back(TokenType::TKL_INT, line, col - str.length(), str);
            return;
        }
        else if (current() == 'b')
        { // Binary integer
            str.push_back('0');
            str.push_back('b');
            advance();
            while (current() == '0' || current() == '1')
            {
                str.push_back(current());
                advance();
            }
            tokens.emplace_back(TokenType::TKL_INT, line, col - str.length(), str);
            return;
        }
    }

    // Decimal integer or float
    while (__is_digit(current()))
    {
        str.push_back(current());
        advance();
    }

    bool __float = 0;

    if (current() == '.')
    {
        str.push_back(current());
        advance();
        while (__is_digit(current()))
        {
            str.push_back(current());
            advance();
        }
        __float = true;
    }

    if (tolower(current()) == 'e')
    {
        __float = true;
        str.push_back(current());
        advance();
        if (current() == '+' || current() == '-')
        {
            str.push_back(current());
            advance();
        }
        while (__is_digit(current()))
        {
            str.push_back(current());
            advance();
        }

        tokens.emplace_back(TokenType::TKL_FLOAT, line, col - str.length(), str);
        return;
    }

    if (__float)
        tokens.emplace_back(TokenType::TKL_FLOAT, line, col - str.length(), str);
    else
        tokens.emplace_back(TokenType::TKL_INT, line, col - str.length(), str);
}

inline bool Lexer::isSeperator(char c) const
{
    constexpr std::array<char, 8> SEPARATORS = {';', ',', '{', '}', '[', ']', '(', ')'};
    return std::find(SEPARATORS.begin(), SEPARATORS.end(), c) != SEPARATORS.end();
}

inline bool Lexer::isOperator(char c) const
{
    constexpr std::array<char, 13> OPERATORS = {'>', '<', '=', '!', '^', '|', '&', '~', '+', '-', '*', '/', '%'};
    return std::find(OPERATORS.begin(), OPERATORS.end(), c) != OPERATORS.end();
}

void Lexer::handleOperator()
{
    std::string op;
    op.reserve(3);

    if (current() == '/' && (peek() == '/' || peek() == '*'))
    {
        handleComment();
        return;
    }
    else
    {
        op.push_back(current());

        if ((current() == '>' && peek() == '>') || (current() == '<' && peek() == '<') || peek() == '=' || (current() == '&' && peek() == '&') || (current() == '|' && peek() == '|') || (current() == '+' && peek() == '+') || (current() == '-' && peek() == '-'))
        {
            op.push_back(peek());
            advance();
        }
        tokens.emplace_back(TokenType::TK_OPERATOR, line, col, op);
        advance();
    }
}

void Lexer::handleStringLiteral()
{
    advance();
    std::string str;
    str.reserve(16);

    while (current() != '"' && current() != '\0')
    {
        if (current() == '\\')
        {
            advance();
            switch (current())
            {
            case 'u':
            {
                advance();
                std::string unicode_seq;
                for (int i = 0; i < 4; ++i)
                {
                    if (!__is_xdigit(current()))
                    {
                        print.error("Invalid Unicode escape sequence.", line, col + 1, file);
                        tokens.emplace_back(TokenType::TK_ERR, line, col - unicode_seq.length(), unicode_seq);
                        return;
                    }
                    unicode_seq.push_back(current());
                    advance();
                }
                str.push_back(static_cast<char>(std::stoi(unicode_seq, nullptr, 16)));
                break;
            }
            case 'n':
                str.push_back('\n');
                advance();
                break;
            case 't':
                str.push_back('\t');
                advance();
                break;
            case '"':
                str.push_back('"');
                advance();
                break;
            case '\\':
                str.push_back('\\');
                advance();
                break;
            default:
                print.warn("Escape sequence is not recognised.", line, col + 1, file);
                str.push_back(current());
                advance();
                break;
            }
        }
        else
        {
            str.push_back(current());
            advance();
        }
    }

    if (current() == '"')
    {
        advance();
    }
    else
    {
        print.error("Unterminated string literal.", line, col + 1, file);
        tokens.emplace_back(TokenType::TK_ERR, line, col - str.length(), str);
        return;
    }

    tokens.emplace_back(TokenType::TKL_STR, line, col - str.length(), str);
}

void Lexer::handleCharLiteral()
{
    advance();

    if (current() == '\\')
    {
        advance();
        std::string escape_seq;
        escape_seq.reserve(6);

        if (current() == 'u')
        {
            advance();
            for (int i = 0; i < 4; ++i)
            {
                escape_seq.push_back(current());
                advance();
            }
            tokens.emplace_back(TokenType::TKL_CHAR, line, col - 6, static_cast<char>(std::stoi(escape_seq, nullptr, 16)));
        }
        else
        {
            escape_seq.push_back(current());
            tokens.emplace_back(TokenType::TKL_CHAR, line, col - 3, escape_seq[0]);
            advance();
        }
    }
    else
    {
        char c = current();
        advance();
        tokens.emplace_back(TokenType::TKL_CHAR, line, col - 2, c);
    }
}

void Lexer::handleComment()
{
    if (peek() == '*')
    {
        while (current() != '/' && previous() != '*')
        {
            advance();
        }
    }
    else
    {
        while (current() != '\n')
        {
            advance();
        }
    }
}
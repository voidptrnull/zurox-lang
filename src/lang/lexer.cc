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

Lexer::Lexer(const std::string &file, const std::string &file_name, PrintGlobalState &print)
    : line(1), col(0), file(file), file_name(file_name), print(print) {}

std::vector<Token> Lexer::lex()
{
    while (col < file.length())
    {
        char c = current();
        if (isalpha(c) || c == '_')
        {
            keywordOrDatatypeOrIdentifier();
        }
        else if (isdigit(c))
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
    while (isalnum(current()) || current() == '_')
    {
        str.push_back(current());
        advance();
    }

    if (auto dt = find_dt(str); dt)
    {
        tokens.emplace_back(TokenType::TK_DATATYPE, line, col - str.length(), str);
    }
    else if (auto keyword = find_keyword(str); keyword)
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

    if (current() == '0')
    {
        advance();
        if (current() == 'x')
        { // Hexadecimal integer or float
            str.push_back('0');
            str.push_back('x');
            advance();
            while (isxdigit(current()))
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
    while (isdigit(current()))
    {
        str.push_back(current());
        advance();
    }
    if (current() == '.')
    {
        str.push_back(current());
        advance();
        while (isdigit(current()))
        {
            str.push_back(current());
            advance();
        }
        if (tolower(current()) == 'e')
        {
            str.push_back(current());
            advance();
            if (current() == '+' || current() == '-')
            {
                str.push_back(current());
                advance();
            }
            while (isdigit(current()))
            {
                str.push_back(current());
                advance();
            }
        }
        tokens.emplace_back(TokenType::TKL_FLOAT, line, col - str.length(), str);
    }
    else
    {
        tokens.emplace_back(TokenType::TKL_INT, line, col - str.length(), str);
    }
}

bool Lexer::isSeperator(char c) const
{
    constexpr std::array<char, 8> SEPARATORS = {';', ',', '{', '}', '[', ']', '(', ')'};
    return std::find(SEPARATORS.begin(), SEPARATORS.end(), c) != SEPARATORS.end();
}

bool Lexer::isOperator(char c) const
{
    constexpr std::array<char, 13> OPERATORS = {'>', '<', '=', '!', '^', '|', '&','~', '+', '-', '*', '/', '%'};
    return std::find(OPERATORS.begin(), OPERATORS.end(), c) != OPERATORS.end();
}

void Lexer::handleOperator()
{
    std::string op;
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
                    if (!isxdigit(current()))
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
#include <array>
#include <algorithm>
#include <memory>

#include <definitions.hh>
#include <lexer.hh>
#include <version.hh>

Lexer::Lexer(const std::string &file, const std::string &file_name, PrintGlobalState &print) : line(1), col(0), file_name(file_name), print(print)
{
    this->file = file;
}

llvm::StringRef Lexer::getFile()
{
    return this->file;
};

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
            tokens.emplace_back(TokenType::TK_SEPERATOR, line, col, std::string(1, current()));
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
            tokens.emplace_back(TokenType::TK_SEPERATOR, line, col, std::string(1, current()));
            advance();
        }
    }
    tokens.emplace_back(TokenType::__EOF, line, col, "");
    return tokens;
}

inline char Lexer::current() const
{
    return file[col];
}

inline char Lexer::peek() const
{
    return col + 1 < file.size() ? file[col + 1] : '\0';
}

inline char Lexer::previous() const
{
    return file.at(col - 1);
}

inline void Lexer::advance()
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
    print.info(str);

    if (find_dt(str).found)
    {
        tokens.emplace_back(TokenType::TK_DATATYPE, line, col - str.length(), str);
    }
    else if (find_keyword(str).found)
    {
        tokens.emplace_back(TokenType::TK_KEYWORD, line, col - str.length(), str);
    }
    else if (find_archdt(str).found)
    {
        print.error("Found '" + str + "' which is not supported for " + _ARCH + ".", line, col - str.length(), file);
        tokens.emplace_back(TokenType::TK_KEYWORD, line, col - str.length(), str); // It can be parsed and checked so add it
    }
    else
    {
        tokens.emplace_back(TokenType::TK_ID, line, col - str.length(), str);

    }
}

void Lexer::number()
{
    std::string str;
    while (isdigit(current()) || current() == '.' || current() == 'x' || current() == 'e' || current() == 'E' || current() == '+' || current() == '-')
    {
        str.push_back(current());
        advance();
    }

    try
    {
        if (str.find('.') != std::string::npos || str.find('e') != std::string::npos || str.find('E') != std::string::npos)
        {
            std::stod(str);
            tokens.emplace_back(TokenType::TKL_FLOAT, line, col - str.length(), str);
        }
        else
        {
            std::stoll(str);
            tokens.emplace_back(TokenType::TKL_INT, line, col - str.length(), str);
        }
    }
    catch (const std::exception &)
    {
        print.error("Invalid number format.", line, col - str.length(), file);
    }
}

bool Lexer::isSeperator(char c) const
{
    constexpr std::array<char, 8> SEPARATORS = {';', ',', '{', '}', '[', ']', '(', ')'};
    return std::find(SEPARATORS.begin(), SEPARATORS.end(), c) != SEPARATORS.end();
}

bool Lexer::isOperator(char c) const
{
    constexpr std::array<char, 12> OPERATORS = {'>', '<', '=', '!', '^', '|', '&', '+', '-', '*', '/', '%'};
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
            if (current() == 'u')
            {
                advance();
                std::string unicode_seq;
                for (int i = 0; i < 4; ++i)
                {
                    unicode_seq.push_back(current());
                    advance();
                }
                str.push_back(static_cast<char>(std::stoi(unicode_seq, nullptr, 16)));
            }
            else
            {
                str.push_back(current());
                advance();
            }
        }
        else
        {
            str.push_back(current());
            advance();
        }
    }
    advance();
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
        return;
    }
}
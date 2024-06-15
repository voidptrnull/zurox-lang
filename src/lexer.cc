#include <lexer.hh>
#include <version.hh>

#include <sstream>
#include <fstream>
#include <cctype>
#include <unordered_set>

lexer::Lexer::Lexer(const std::string &file_name, util::PrintGlobalState &print) : line(1), col(0), file_name(file_name), print(print)
{
    std::ifstream file_stream(file_name);
    if (!file_stream)
    {
        print.error("File '" + file_name + "' not found !!!");
        exit(127);
    }
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    file = buffer.str();
}

std::vector<token::Token> lexer::Lexer::lex()
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
            tokens.emplace_back(token::TokenType::TK_SEPERATOR, line, col, std::string(1, current()));
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
            tokens.emplace_back(token::TokenType::TK_SEPERATOR, line, col, std::string(1, current()));
            advance();
        }
    }
    tokens.emplace_back(token::TokenType::__EOF, line, col, "");
    return tokens;
}

inline char lexer::Lexer::current() const
{
    return file[col];
}

inline char lexer::Lexer::peek() const
{
    return col + 1 < file.size() ? file[col + 1] : '\0';
}

inline char lexer::Lexer::previous() const
{
    return file.at(col - 1);
}

inline void lexer::Lexer::advance()
{
    col++;
}

void lexer::Lexer::keywordOrDatatypeOrIdentifier()
{
    std::string str;
    while (isalnum(current()) || current() == '_')
    {
        str.push_back(current());
        advance();
    }

    static const std::unordered_set<std::string> DATA_TYPES = {
        "u8", "u16", "u32", "u64", "i8", "i16", "i32", "i64",
        "f32", "f64",
#if defined(__x86_64__) || defined(__aarch64__) || defined(__powerpc64__) // Target specific features, usage is not recommended or avaialable by LLVM
        "u128", "i128",                                                   // Dont support x86 even though it is avaialable for LLVM
#endif
#ifdef __x86_64__ // x86_64 specific features, can support x86 too but will not be implemented
        "f80", "f128"
#endif
               "char",
        "bool"};

    static const std::unordered_set<std::string> ARCH_SPECIFIC_TYPES = {
        "u128", "i128", "f80", "f128"};

    static const std::unordered_set<std::string> KEYWORDS = {
        "if", "elif", "else", "while", "fn", "as", "ret", "true", "false", "ref", "deref",
        "struct", "sync", "enum", "void", "volatile", "import", "break", "continue", "match"};

    if (DATA_TYPES.find(str) != DATA_TYPES.end())
    {
        tokens.emplace_back(token::TokenType::TK_DATATYPE, line, col - str.length(), str);
    }
    else if (KEYWORDS.find(str) != KEYWORDS.end())
    {
        tokens.emplace_back(token::TokenType::TK_KEYWORD, line, col - str.length(), str);
    }
    else if (ARCH_SPECIFIC_TYPES.find(str) != ARCH_SPECIFIC_TYPES.end())
    {
        print.error("Found '" + str + "' which is not supported for " + _ARCH + ".", line, col - str.length(), file);
        tokens.emplace_back(token::TokenType::TK_KEYWORD, line, col - str.length(), str); // It can be parsed and checked so add it
    }
    else
    {
        tokens.emplace_back(token::TokenType::TK_ID, line, col - str.length(), str);
    }
}

void lexer::Lexer::number()
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
            tokens.emplace_back(token::TokenType::TKL_FLOAT, line, col - str.length(), str);
        }
        else
        {
            std::stoll(str);
            tokens.emplace_back(token::TokenType::TKL_INT, line, col - str.length(), str);
        }
    }
    catch (const std::exception &)
    {
        print.error("Invalid number format.", line, col - str.length(), file);
    }
}

bool lexer::Lexer::isSeperator(char c) const
{
    static const std::unordered_set<char> SEPARATORS = {';', ',', '{', '}', '[', ']', '(', ')'};
    return SEPARATORS.find(c) != SEPARATORS.end();
}

bool lexer::Lexer::isOperator(char c) const
{
    static const std::unordered_set<char> OPERATORS = {'>', '<', '=', '!', '^', '|', '&', '+', '-', '*', '/', '%'};
    return OPERATORS.find(c) != OPERATORS.end();
}

void lexer::Lexer::handleOperator()
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
        tokens.emplace_back(token::TokenType::TK_OPERATOR, line, col, op);
        advance();
    }
}

void lexer::Lexer::handleStringLiteral()
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
    tokens.emplace_back(token::TokenType::TKL_STR, line, col - str.length(), str);
}

void lexer::Lexer::handleCharLiteral()
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
            tokens.emplace_back(token::TokenType::TKL_CHAR, line, col - 6, static_cast<char>(std::stoi(escape_seq, nullptr, 16)));
        }
        else
        {
            escape_seq.push_back(current());
            tokens.emplace_back(token::TokenType::TKL_CHAR, line, col - 3, escape_seq[0]);
            advance();
        }
    }
    else
    {
        char c = current();
        advance();
        tokens.emplace_back(token::TokenType::TKL_CHAR, line, col - 2, c);
    }
}

void lexer::Lexer::handleComment()
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
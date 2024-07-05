#include <parser.hh>

Parser::Parser(std::vector<ASTNode> &declarations, std::vector<Token> &tokens, PrintGlobalState &print, const std::string &file)
    : declarations(declarations), tokens(tokens), print(print), file(file), pos(0) {}

inline bool Parser::isnend()
{
    return pos < tokens.size();
}

inline bool Parser::expect(TokenType type, const std::string &err_msg)
{
    if (type != tokens[pos].type)
    {
        print.error(err_msg, tokens[pos].line, tokens[pos].col, file);
        return false;
    }
    return true;
}

inline std::shared_ptr<ASTNode> Parser::parseLiteral()
{
    TokenType t = tokens[pos].type;
    if (t != TKL_CHAR && t != TKL_INT && t != TKL_FLOAT && t != TKL_STR)
    {
        print.error("Expected a valid literal, found '" + tokens[pos].lexeme + "'");
        return nullptr;
    }

    return std::make_shared<LiteralASTNode>(tokens[pos]);
}

inline std::shared_ptr<UnaryOp> Parser::parseUnaryOp()
{
    if (!expect(TK_OPERATOR, "Expected a '+','-','!' or '~', found '" + tokens[pos].lexeme + "'"))
        return nullptr;

    if (tokens[pos].lexeme.length() != 1)
    {
        print.error("Expected a '+','-','!' or '~', found '" + tokens[pos].lexeme + "'");
        return nullptr;
    }

    switch (tokens[pos].lexeme[0])
    {
    case '+':
    case '-':
    case '!':
    case '~':
        return std::make_shared<UnaryOp>(tokens[pos].lexeme[0]);
    default:
        print.error("Expected a '+','-','!' or '~', found '" + tokens[pos].lexeme + "'");
        return nullptr;
    }
}

inline std::shared_ptr<ASTNode> Parser::parseType()
{
}

inline std::shared_ptr<ASTNode> Parser::parsePrimary()
{
}

inline std::shared_ptr<ASTNode> Parser::parseUnaryExpr()
{
}

inline std::shared_ptr<ASTNode> Parser::parseFactor()
{
}

inline std::shared_ptr<ASTNode> Parser::parseTerm()
{
}

inline std::shared_ptr<ASTNode> Parser::parseExpression()
{
}

inline std::shared_ptr<ASTNode> Parser::parseBinaryExpr()
{
}

inline std::shared_ptr<ASTNode> Parser::parseParameter()
{
}

inline std::shared_ptr<ASTNode> Parser::parseParameters()
{
}

inline std::shared_ptr<ASTNode> Parser::parseReturnType()
{
}

inline std::shared_ptr<BlockASTNode> Parser::parseBlock()
{
}

inline std::shared_ptr<ASTNode> Parser::parseStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseAsmBlock()
{
}

inline std::shared_ptr<ASTNode> Parser::parseAsmStatements()
{
}

inline std::shared_ptr<ASTNode> Parser::parseAsmStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseRetStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseIfStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseLoopStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseVarDeclaration()
{
}

inline std::shared_ptr<ASTNode> Parser::parseExpressionStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseMatchStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseCaseClause()
{
}

inline std::shared_ptr<ASTNode> Parser::parseBreakStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseContinueStatement()
{
}

inline std::shared_ptr<ASTNode> Parser::parseEnumFields()
{
}

inline std::shared_ptr<ASTNode> Parser::parseStructFields()
{
}

inline std::shared_ptr<ASTNode> Parser::parseFunctionDeclaration()
{
}

inline std::shared_ptr<ASTNode> Parser::parseEnumDeclaration()
{
}

inline std::shared_ptr<ASTNode> Parser::parseStructDeclaration()
{
}

inline std::shared_ptr<ASTNode> Parser::parseDeclaration()
{
}

void Parser::parse()
{
    while (isnend())
    {
        parseDeclaration();
    }
}
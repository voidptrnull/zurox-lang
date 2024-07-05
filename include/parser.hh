#ifndef PARSER_HH
#define PARSER_HH

#include <ast.hh>
#include <print.hh>

class Parser
{
public:
    Parser(std::vector<ASTNode> &declarations, std::vector<Token> &tokens, PrintGlobalState &print, const std::string &file);
    void parse(); // parseProgram()

private:
    std::vector<ASTNode> declarations;
    std::vector<Token> tokens;
    PrintGlobalState &print;
    const std::string &file;
    u_int pos;

    inline bool isnend();
    inline bool expect(TokenType type, const std::string &err_msg);

    inline std::shared_ptr<ASTNode> parseLiteral();
    inline std::shared_ptr<UnaryOp> parseUnaryOp();
    inline std::shared_ptr<ASTNode> parseType();
    inline std::shared_ptr<ASTNode> parsePrimary();
    inline std::shared_ptr<ASTNode> parseUnaryExpr();
    inline std::shared_ptr<ASTNode> parseFactor();
    inline std::shared_ptr<ASTNode> parseTerm();
    inline std::shared_ptr<ASTNode> parseExpression();
    inline std::shared_ptr<ASTNode> parseBinaryExpr();
    inline std::shared_ptr<ASTNode> parseParameter();
    inline std::shared_ptr<ASTNode> parseParameters();
    inline std::shared_ptr<ASTNode> parseReturnType();
    inline std::shared_ptr<BlockASTNode> parseBlock();
    inline std::shared_ptr<ASTNode> parseStatement();
    inline std::shared_ptr<ASTNode> parseAsmBlock();
    inline std::shared_ptr<ASTNode> parseAsmStatements();
    inline std::shared_ptr<ASTNode> parseAsmStatement();
    inline std::shared_ptr<ASTNode> parseRetStatement();
    inline std::shared_ptr<ASTNode> parseIfStatement();
    inline std::shared_ptr<ASTNode> parseLoopStatement();
    inline std::shared_ptr<ASTNode> parseVarDeclaration();
    inline std::shared_ptr<ASTNode> parseExpressionStatement();
    inline std::shared_ptr<ASTNode> parseMatchStatement();
    inline std::shared_ptr<ASTNode> parseCaseClause();
    inline std::shared_ptr<ASTNode> parseBreakStatement();
    inline std::shared_ptr<ASTNode> parseContinueStatement();
    inline std::shared_ptr<ASTNode> parseEnumFields();
    inline std::shared_ptr<ASTNode> parseStructFields();
    inline std::shared_ptr<ASTNode> parseFunctionDeclaration();
    inline std::shared_ptr<ASTNode> parseEnumDeclaration();
    inline std::shared_ptr<ASTNode> parseStructDeclaration();
    inline std::shared_ptr<ASTNode> parseDeclaration();
};

#endif
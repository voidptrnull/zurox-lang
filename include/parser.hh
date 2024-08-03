/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */
#ifndef PARSER_HH
#define PARSER_HH

#include <ast.hh>
#include <token.hh>
#include <print.hh>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

class Parser
{
public:
    Parser(std::vector<std::shared_ptr<ASTNode>> &nodes, const std::vector<Token> &tokens, PrintGlobalState &print, const std::string &file);

    void parse();

private:
    Token currentToken();
    Token peekToken(int n);
    void advance();
    bool match(TokenType type);
    bool expect(TokenType type, const std::string &err_msg);

    std::shared_ptr<LiteralASTNode> parseLiteral();
    std::shared_ptr<ASTNode> parsePrimary();
    std::shared_ptr<ASTNode> parseUnaryExpr();
    std::shared_ptr<ASTNode> parseFactor();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseShiftExpression();
    std::shared_ptr<ASTNode> parseAndExpression();
    std::shared_ptr<ASTNode> parseXorExpression();
    std::shared_ptr<ASTNode> parseOrExpression();
    std::shared_ptr<ASTNode> parseComparison();
    std::shared_ptr<ASTNode> parseLAndExpression();
    std::shared_ptr<ASTNode> parseLOrExpression();
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<BlockASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<IfStmtASTNode> parseIfStatement();
    std::shared_ptr<LoopStmtASTNode> parseLoopStatement();
    std::shared_ptr<ASTNode> parseRetStatement();
    std::shared_ptr<AsmBlockASTNode> parseAsmBlock();
    std::shared_ptr<AsmBlockStmntASTNode> parseAsmStatement();
    std::shared_ptr<LLVMBlockASTNode> parseLLVMBlock();
    std::shared_ptr<LLVMBlockStmntASTNode> parseLLVMStatement();
    std::shared_ptr<ASTNode> parseExpressionStatement();
    std::shared_ptr<MatchStmtASTNode> parseMatchStatement();
    std::shared_ptr<CaseClauseASTNode> parseCaseClause();
    std::shared_ptr<JumpASTNode> parseBreakStatement();
    std::shared_ptr<JumpASTNode> parseContinueStatement();
    std::shared_ptr<EnumDeclASTNode> parseEnumDeclaration();
    std::shared_ptr<StructDeclASTNode> parseStructDeclaration();
    std::shared_ptr<FunctionDeclASTNode> parseFunctionDeclaration();
    std::shared_ptr<ASTNode> parseParameter();
    std::shared_ptr<ASTNode> parseReturnType();
    std::shared_ptr<ASTNode> parseDeclaration();

    std::vector<std::shared_ptr<ASTNode>> &nodes;
    const std::vector<Token> &tokens;
    PrintGlobalState &print;
    const std::string &file;
    int_t pos = 0;
};

#endif // PARSER_HH
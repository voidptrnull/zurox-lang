/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */
#include "parser.hh"
#include <iostream>
#include <unordered_map>
#include <functional>

Parser::Parser(std::vector<std::shared_ptr<ASTNode>> &nodes, const std::vector<Token> &tokens, PrintGlobalState &print, const std::string &file) : nodes(nodes), tokens(tokens), print(print), file(file) {}

Token Parser::currentToken()
{
    if (pos < tokens.size() - 1)
    {
        return tokens[pos];
    }
    return Token(__EOF, 0, 0, "");
}

Token Parser::peekToken(int n)
{
    if (pos + n < tokens.size() - 1)
    {
        return tokens[pos + n];
    }
    return Token(__EOF, 0, 0, "");
}

void Parser::advance()
{
    if (pos < tokens.size() - 1)
    {
        pos++;
        return;
    }
    print.error("Reached unexpected end of file while parsing.", tokens[pos].line, tokens[pos].col, file);
}

bool Parser::match(TokenType type)
{
    if (currentToken().type == type)
    {
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type, const std::string &err_msg)
{
    if (!match(type))
    {
        print.error(err_msg + currentToken().lexeme, currentToken().line, currentToken().col, file);
        return false;
    }
    return true;
}

std::shared_ptr<LiteralASTNode> Parser::parseLiteral()
{
    Token token = currentToken();
    if (token.type == TKL_INT || token.type == TKL_FLOAT || token.type == TKL_CHAR || token.type == TKL_STR)
    {
        advance();
        return std::make_shared<LiteralASTNode>(token);
    }
    print.error("Expected a literal", token.line, token.col, file);
    return nullptr;
}

std::shared_ptr<ASTNode> Parser::parsePrimary()
{
    Token token = currentToken();
    if (token.type == TK_ID)
    {
        advance();
        return std::make_shared<IdentifierASTNode>(token);
    }
    else if (token.type == TKL_INT || token.type == TKL_FLOAT || token.type == TKL_CHAR || token.type == TKL_STR)
    {
        return parseLiteral();
    }
    else if (match(TK_SEPARATOR) && token.lexeme == "(")
    {
        std::shared_ptr<ASTNode> expr = parseExpression();
        expect(TK_SEPARATOR, "Expected ')'");
        return expr;
    }
    else
    {
        print.error("Expected a primary expression", token.line, token.col, file);
        return nullptr;
    }
}

std::shared_ptr<ASTNode> Parser::parseUnaryExpr()
{
    Token token = currentToken();
    if (match(TK_OPERATOR) && (token.lexeme == "-" || token.lexeme == "!" || token.lexeme == "~"))
    {
        std::shared_ptr<ASTNode> operand = parseUnaryExpr();
        return std::make_shared<UnaryOp>(token, operand);
    }
    return parsePrimary();
}

std::shared_ptr<ASTNode> Parser::parseFactor()
{
    std::shared_ptr<ASTNode> node = parseUnaryExpr();
    while (currentToken().type == TK_OPERATOR && (currentToken().lexeme == "*" || currentToken().lexeme == "/" || currentToken().lexeme == "%"))
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseUnaryExpr();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseTerm()
{
    std::shared_ptr<ASTNode> node = parseFactor();
    while (currentToken().type == TK_OPERATOR && (currentToken().lexeme == "+" || currentToken().lexeme == "-"))
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseFactor();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseShiftExpression()
{
    std::shared_ptr<ASTNode> node = parseTerm();
    while (currentToken().type == TK_OPERATOR && (currentToken().lexeme == "<<" || currentToken().lexeme == ">>"))
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseTerm();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseAndExpression()
{
    std::shared_ptr<ASTNode> node = parseShiftExpression();
    while (currentToken().type == TK_OPERATOR && currentToken().lexeme == "&")
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseShiftExpression();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseXorExpression()
{
    std::shared_ptr<ASTNode> node = parseAndExpression();
    while (currentToken().type == TK_OPERATOR && currentToken().lexeme == "^")
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseAndExpression();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseOrExpression()
{
    std::shared_ptr<ASTNode> node = parseXorExpression();
    while (currentToken().type == TK_OPERATOR && currentToken().lexeme == "|")
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseXorExpression();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseComparison()
{
    std::shared_ptr<ASTNode> node = parseOrExpression();
    while (currentToken().type == TK_OPERATOR && (currentToken().lexeme == "<" || currentToken().lexeme == ">" ||
                                                  currentToken().lexeme == "<=" || currentToken().lexeme == ">=" ||
                                                  currentToken().lexeme == "==" || currentToken().lexeme == "!="))
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseOrExpression();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseLAndExpression()
{
    std::shared_ptr<ASTNode> node = parseComparison();
    while (currentToken().type == TK_OPERATOR && currentToken().lexeme == "&&")
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseComparison();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseLOrExpression()
{
    std::shared_ptr<ASTNode> node = parseLAndExpression();
    while (currentToken().type == TK_OPERATOR && currentToken().lexeme == "||")
    {
        Token op = currentToken();
        advance();
        std::shared_ptr<ASTNode> right = parseLAndExpression();
        node = std::make_shared<BinaryOp>(node, op, right);
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseExpression()
{
    return parseLOrExpression();
}

std::shared_ptr<BlockASTNode> Parser::parseBlock()
{
    auto block = std::make_shared<BlockASTNode>();
    expect(TK_SEPARATOR, "Expected '{', found ");
    while (!match(TK_SEPARATOR) || currentToken().lexeme != "}")
    {
        block->statements.push_back(parseStatement());
    }
    return block;
}

std::shared_ptr<ASTNode> Parser::parseStatement()
{
    Token token = currentToken();
    if (token.type == TK_KEYWORD)
    {
        const std::unordered_map<std::string, std::function<std::shared_ptr<ASTNode>(Parser*)>> parseFunctionMap = {
            {"if", &Parser::parseIfStatement},
            {"loop", &Parser::parseLoopStatement},
            {"ret", &Parser::parseRetStatement},
            {"asm", &Parser::parseAsmStatement},
            {"llvm", &Parser::parseLLVMStatement},
            {"match", &Parser::parseMatchStatement},
            {"break", &Parser::parseBreakStatement},
            {"continue", &Parser::parseContinueStatement},
            {"enum", &Parser::parseEnumDeclaration},
            {"struct", &Parser::parseStructDeclaration},
            {"fn", &Parser::parseFunctionDeclaration}
        };

        auto it = parseFunctionMap.find(token.lexeme);
        if (it != parseFunctionMap.end())
        {
            return (it->second)(this);
        }
    }

    return parseExpressionStatement();
}

std::shared_ptr<IfStmtASTNode> Parser::parseIfStatement()
{
    expect(TK_KEYWORD, "Expected 'if', found ");
    auto condition = parseExpression();
    auto thenBranch = parseBlock();
    std::shared_ptr<BlockASTNode> elseBranch;
    std::vector<std::pair<ASTNodePtr, std::shared_ptr<BlockASTNode>>> elifBranches;
    while (match(TK_KEYWORD) && currentToken().lexeme == "elif")
    {
        auto condition = parseExpression();
        auto elifBranch = parseBlock();
        elifBranches.push_back(std::make_pair(condition, std::make_shared<BlockASTNode>(elifBranch)));
    }
    if (match(TK_KEYWORD) && currentToken().lexeme == "else")
    {
        elseBranch = parseBlock();
    }
    return std::make_shared<IfStmtASTNode>(condition, thenBranch, elifBranches, elseBranch);
}

std::shared_ptr<LoopStmtASTNode> Parser::parseLoopStatement()
{
    expect(TK_KEYWORD, "Expected 'loop'");
    return std::make_shared<LoopStmtASTNode>(parseBlock());
}

std::shared_ptr<ASTNode> Parser::parseRetStatement()
{
    expect(TK_KEYWORD, "Expected 'ret'");
    auto expr = parseExpression();
    return std::make_shared<ReturnStmtASTNode>(expr);
}

std::shared_ptr<AsmBlockASTNode> Parser::parseAsmBlock()
{
    expect(TK_KEYWORD, "Expected 'asm'");
    expect(TK_SEPARATOR, "Expected '{'");
    auto block = std::make_shared<AsmBlockASTNode>();
    while (currentToken().lexeme != "}")
    {
        std::vector<Token> inst;
        while (currentToken().lexeme != ",")
        {
            if (currentToken().lexeme == "+")
            {
                advance();
            }

            if (!match(TK_ID) && !match(TKL_STR))
            {
                print.error("Expected an identifier or string, found `" + currentToken().lexeme, currentToken().line, currentToken().col, file);
            }
            inst.push_back(currentToken());
        }
        block->instructions.push_back(inst);
        inst.clear();
        advance();
    }
    return block;
}

std::shared_ptr<AsmBlockStmntASTNode> Parser::parseAsmStatement()
{
    return std::make_shared<AsmBlockStmntASTNode>(parseAsmBlock());
}

std::shared_ptr<LLVMBlockASTNode> Parser::parseLLVMBlock()
{
    expect(TK_KEYWORD, "Expected 'llvm'");
    expect(TK_SEPARATOR, "Expected '{'");
    auto block = std::make_shared<LLVMBlockASTNode>();
    while (currentToken().lexeme != "}")
    {
        std::vector<Token> inst;
        while (currentToken().lexeme != ",")
        {
            if (!match(TK_ID) && !match(TKL_STR))
            {
                print.error("Expected an identifier or string, found `" + currentToken().lexeme + "`", currentToken().line, currentToken().col, file);
                return nullptr;
            }

            inst.push_back(currentToken());
            advance();

            while (currentToken().lexeme == "+")
            {
                inst.push_back(currentToken());
                advance();

                if (!match(TK_ID) && !match(TKL_STR))
                {
                    print.error("Expected an identifier or string after '+', found `" + currentToken().lexeme + "`", currentToken().line, currentToken().col, file);
                    return nullptr;
                }

                inst.push_back(currentToken());
                advance();
            }
        }

        block->instructions.push_back(inst);
        inst.clear();
        advance();
    }
    return block;
}

std::shared_ptr<LLVMBlockStmntASTNode> Parser::parseLLVMStatement()
{
    return std::make_shared<LLVMBlockStmntASTNode>(parseLLVMBlock());
}

std::shared_ptr<ASTNode> Parser::parseExpressionStatement()
{
    auto expr = parseExpression();
    expect(TK_SEPARATOR, "Expected ';'");
    return expr;
}

std::shared_ptr<MatchStmtASTNode> Parser::parseMatchStatement()
{
    expect(TK_KEYWORD, "Expected 'match'");
    auto expr = parseExpression();
    expect(TK_SEPARATOR, "Expected '{'");
    auto matchStmt = std::make_shared<MatchStmtASTNode>(expr);
    while (!match(TK_SEPARATOR) || currentToken().lexeme != "}")
    {
        matchStmt->cases.push_back(parseCaseClause());
    }
    return matchStmt;
}

std::shared_ptr<CaseClauseASTNode> Parser::parseCaseClause()
{
    auto value = parseExpression();
    expect(TK_SEPARATOR, "Expected ':'");
    auto body = parseBlock();
    return std::make_shared<CaseClauseASTNode>(value, body);
}

std::shared_ptr<JumpASTNode> Parser::parseBreakStatement()
{
    expect(TK_KEYWORD, "Expected 'break'");
    return std::make_shared<JumpASTNode>(JumpType::BREAK);
}

std::shared_ptr<JumpASTNode> Parser::parseContinueStatement()
{
    expect(TK_KEYWORD, "Expected 'continue'");
    return std::make_shared<JumpASTNode>(JumpType::CONTINUE);
}

std::shared_ptr<EnumDeclASTNode> Parser::parseEnumDeclaration()
{
    expect(TK_KEYWORD, "Expected 'enum'");
    auto name = currentToken();
    expect(TK_ID, "Expected identifier");
    expect(TK_SEPARATOR, "Expected '{'");
    auto enumDecl = std::make_shared<EnumDeclASTNode>(name);
    while (currentToken().lexeme != "}")
    {
        auto value = currentToken();
        expect(TK_ID, "Expected identifier");
        enumDecl->values.push_back(value);
        if (!match(TK_SEPARATOR) || (currentToken().lexeme != "," && currentToken().lexeme != "}"))
        {
            print.error("Expected ',' or '}', found " + currentToken().lexeme, currentToken().line, currentToken().col, file);
            break;
        }
    }
    return enumDecl;
}

std::shared_ptr<StructDeclASTNode> Parser::parseStructDeclaration()
{
    expect(TK_KEYWORD, "Expected 'struct', ");
    auto name = currentToken();
    expect(TK_ID, "Expected identifier");
    expect(TK_SEPARATOR, "Expected '{'");
    auto structDecl = std::make_shared<StructDeclASTNode>(name);
    while ((currentToken().lexeme != ";" && currentToken().type != TK_SEPARATOR) || currentToken().lexeme != "}")
    {
        auto type = currentToken();
        expect(TK_ID, "Expected a data type.");
        auto member = currentToken();
        expect(TK_ID, "Expected identifier");
        structDecl->members.push_back(std::make_pair(type.lexeme, member));
        if (!match(TK_SEPARATOR) || currentToken().lexeme != ";")
        {
            break;
        }
    }
    return structDecl;
}

std::shared_ptr<FunctionDeclASTNode> Parser::parseFunctionDeclaration()
{
    expect(TK_KEYWORD, "Expected 'fn'");
    auto name = currentToken();
    expect(TK_ID, "Expected identifier");
    expect(TK_SEPARATOR, "Expected '('");
    auto funcDecl = std::make_shared<FunctionDeclASTNode>(name);
    while (!match(TK_SEPARATOR) || currentToken().lexeme != ")")
    {
        funcDecl->params.push_back(parseParameter());
        if (!match(TK_SEPARATOR) || currentToken().lexeme != ",")
        {
            break;
        }
    }
    funcDecl->returnType = parseReturnType();
    funcDecl->body = parseBlock();
    return funcDecl;
}

std::shared_ptr<ASTNode> Parser::parseParameter()
{
    auto type = currentToken();
    expect(TK_ID, "Expected type, found");
    auto name = currentToken();
    expect(TK_ID, "Expected identifier");
    return std::make_shared<ParameterASTNode>(type, name);
}

std::shared_ptr<ASTNode> Parser::parseReturnType()
{
    if (match(TK_SEPARATOR) && currentToken().lexeme == "->")
    {
        auto type = currentToken();
        expect(TK_ID, "Expected type");
        return std::make_shared<ReturnTypeASTNode>(type);
    }
    return nullptr;
}

std::shared_ptr<ASTNode> Parser::parseDeclaration()
{
    if (!match(TK_KEYWORD))
    {
        print.error("Expected a 'fn', 'struct', 'enum', 'asm' or 'llvm' keyword, found " + currentToken().lexeme, currentToken().line, currentToken().line, file);
        return nullptr;
    }

    if (currentToken().lexeme == "fn")
    {
        return parseFunctionDeclaration();
    }
    else if (currentToken().lexeme == "struct")
    {
        return parseStructDeclaration();
    }
    else if (currentToken().lexeme == "enum")
    {
        return parseEnumDeclaration();
    }
    else if (currentToken().lexeme == "asm")
    {
        return parseAsmBlock();
    }
    else if (currentToken().lexeme == "llvm")
    {
        return parseLLVMBlock();
    }
    else
    {
        print.error("Expected a fn, struct, enum, asm or llvm keyword, found " + currentToken().lexeme, currentToken().line, currentToken().line, file);
        return nullptr;
    }
}

void Parser::parse()
{
    while (pos < tokens.size())
    {
        auto decl = parseDeclaration();
        if (decl)
        {
            nodes.push_back(decl);
        }
        else
        {
            print.error("Unexpected token", currentToken().line, currentToken().col, file);
            advance();
        }
    }
}
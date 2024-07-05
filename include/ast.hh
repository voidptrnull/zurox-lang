#ifndef AST_HH
#define AST_HH

#include <token.hh>
#include <memory>
#include <vector>

class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

class LiteralASTNode : public ASTNode
{
public:
    Token token;
    LiteralASTNode(const Token &_token) : token(_token) {}
};

class UnaryOp
{
public:
    const char l;
    UnaryOp(const char &l) : l(l) {}
};

class UnaryExprASTNode : public ASTNode
{
public:
    std::shared_ptr<UnaryOp> op;

    UnaryExprASTNode(std::shared_ptr<UnaryOp> _op)
        : op(_op) {}
};

class BinaryExprASTNode : public ASTNode
{
public:
    Token op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;

    BinaryExprASTNode(const Token &_op, std::shared_ptr<ASTNode> _left, std::shared_ptr<ASTNode> _right)
        : op(_op), left(_left), right(_right) {}
};

class VarASTNode : public ASTNode
{
public:
    Token identifier;
};

class VarDeclASTNode : public ASTNode
{
public:
    Token type;
    Token identifier;
    std::shared_ptr<ASTNode> initializer; // Optional initializer expression

    VarDeclASTNode(const Token &_type, const Token &_identifier, std::shared_ptr<ASTNode> _initializer = nullptr)
        : type(_type), identifier(_identifier), initializer(_initializer) {}
};

class BlockASTNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<ASTNode>> statements;

    BlockASTNode(const std::vector<std::shared_ptr<ASTNode>> &_statements)
        : statements(_statements) {}
};

class IfStmtASTNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<BlockASTNode> ifBlock;
    std::vector<std::pair<std::shared_ptr<ASTNode>, std::shared_ptr<BlockASTNode>>> elifBlocks;
    std::shared_ptr<BlockASTNode> elseBlock; // Optional

    IfStmtASTNode(std::shared_ptr<ASTNode> _condition, std::shared_ptr<BlockASTNode> _ifBlock,
                  const std::vector<std::pair<std::shared_ptr<ASTNode>, std::shared_ptr<BlockASTNode>>> &_elifBlocks,
                  std::shared_ptr<BlockASTNode> _elseBlock)
        : condition(_condition), ifBlock(_ifBlock), elifBlocks(_elifBlocks), elseBlock(_elseBlock) {}
};

class LoopStmtASTNode : public ASTNode
{
public:
    std::shared_ptr<BlockASTNode> loopBlock;

    LoopStmtASTNode(std::shared_ptr<BlockASTNode> _loopBlock)
        : loopBlock(_loopBlock) {}
};

class ReturnStmtASTNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> expression; // Optional return expression

    ReturnStmtASTNode(std::shared_ptr<ASTNode> _expression)
        : expression(_expression) {}
};

class FunctionDeclASTNode : public ASTNode
{
public:
    Token identifier;
    std::vector<std::shared_ptr<ASTNode>> parameters;
    Token returnType;
    std::shared_ptr<BlockASTNode> body;

    FunctionDeclASTNode(const Token &_identifier, const std::vector<std::shared_ptr<ASTNode>> &_parameters, const Token &_returnType, std::shared_ptr<BlockASTNode> _body)
        : identifier(_identifier), parameters(_parameters), returnType(_returnType), body(_body) {}
};

class StructDeclASTNode : public ASTNode
{
public:
    Token identifier;
    std::vector<std::pair<Token, Token>> fields; // Type and identifier pairs

    StructDeclASTNode(const Token &_identifier, const std::vector<std::pair<Token, Token>> &_fields)
        : identifier(_identifier), fields(_fields) {}
};

class EnumDeclASTNode : public ASTNode
{
public:
    Token identifier;
    std::vector<Token> fields;

    EnumDeclASTNode(const Token &_identifier, const std::vector<Token> &_fields)
        : identifier(_identifier), fields(_fields) {}
};

class AsmBlockStmntASTNode : public ASTNode
{
public:
    std::string left;
    std::shared_ptr<VarASTNode> identifier;
    std::string right;

    AsmBlockStmntASTNode(const std::string &_left, const VarASTNode &_identifier, const std::string &_right)
        : left(_left), identifier(std::make_shared<VarASTNode>(_identifier)), right(_right) {}
};

class AsmBlockASTNode : public ASTNode
{
public:
    std::vector<AsmBlockStmntASTNode> statements;

    AsmBlockASTNode(const std::vector<AsmBlockStmntASTNode> &_statements)
        : statements(_statements) {}
};

class CaseClauseASTNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> pattern;
    std::shared_ptr<BlockASTNode> block;

    CaseClauseASTNode(std::shared_ptr<ASTNode> _pattern, std::shared_ptr<BlockASTNode> _block)
        : pattern(_pattern), block(_block) {}
};

class MatchStmtASTNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> expression;
    std::vector<std::shared_ptr<CaseClauseASTNode>> cases;
    std::shared_ptr<BlockASTNode> defaultCase; // Optional default case

    MatchStmtASTNode(std::shared_ptr<ASTNode> _expression, const std::vector<std::shared_ptr<CaseClauseASTNode>> &_cases, std::shared_ptr<BlockASTNode> _defaultCase)
        : expression(_expression), cases(_cases), defaultCase(_defaultCase) {}
};

#endif
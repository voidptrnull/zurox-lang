/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */
#ifndef AST_HH
#define AST_HH

#include <token.hh>
#include <memory>
#include <vector>
#include <string>

class ASTVisitor;

// Base class for all AST nodes
class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

using ASTNodePtr = std::shared_ptr<ASTNode>;

// Class for literal values
class LiteralASTNode : public ASTNode
{
public:
    Token token;

    LiteralASTNode(const Token &tok) : token(tok) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for identifiers
class IdentifierASTNode : public ASTNode
{
public:
    Token token;

    IdentifierASTNode(const Token &tok) : token(tok) {}
    void accept(ASTVisitor &visitor) override;
};

class VarDeclASTNode : public ASTNode
{
public:
bool reference;

void accept(ASTVisitor &visitor) override;
};

// Class for binary operations
class BinaryOp : public ASTNode
{
public:
    ASTNodePtr left;
    Token op;
    ASTNodePtr right;

    BinaryOp(ASTNodePtr l, const Token &o, ASTNodePtr r) : left(l), op(o), right(r) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for unary operations
class UnaryOp : public ASTNode
{
public:
    std::string op;
    ASTNodePtr operand;

    UnaryOp(const Token &o, ASTNodePtr opd) : op(o.lexeme), operand(opd) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for blocks of statements
class BlockASTNode : public ASTNode
{
public:
    std::vector<ASTNodePtr> statements;
    BlockASTNode() = default;
    BlockASTNode(std::shared_ptr<BlockASTNode> ptr) : statements(ptr.get()->statements) {}
    BlockASTNode(std::vector<ASTNodePtr> &statements) : statements(statements) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for if statements
class IfStmtASTNode : public ASTNode
{
public:
    ASTNodePtr condition;
    std::shared_ptr<BlockASTNode> thenBranch;
    std::vector<std::pair<ASTNodePtr, std::shared_ptr<BlockASTNode>>> elifBranches;
    std::shared_ptr<BlockASTNode> elseBranch;

    IfStmtASTNode(ASTNodePtr cond, std::shared_ptr<BlockASTNode> thenBr, std::vector<std::pair<ASTNodePtr, std::shared_ptr<BlockASTNode>>> elifBrs, std::shared_ptr<BlockASTNode> elseBr = nullptr)
        : condition(cond), thenBranch(thenBr), elifBranches(elifBrs), elseBranch(elseBr) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for loop statements
class LoopStmtASTNode : public ASTNode
{
public:
    std::shared_ptr<BlockASTNode> body;

    LoopStmtASTNode(std::shared_ptr<BlockASTNode> b) : body(b) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for return statements
class ReturnStmtASTNode : public ASTNode
{
public:
    ASTNodePtr expression;

    ReturnStmtASTNode(ASTNodePtr expr) : expression(expr) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for asm blocks
class AsmBlockASTNode : public ASTNode
{
public:
    std::vector<std::vector<Token>> instructions;

    AsmBlockASTNode() = default;
    void accept(ASTVisitor &visitor) override;
};

// Class for asm block statements
class AsmBlockStmntASTNode : public ASTNode
{
public:
    std::shared_ptr<AsmBlockASTNode> block;

    AsmBlockStmntASTNode(std::shared_ptr<AsmBlockASTNode> blk) : block(blk) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for asm blocks
class LLVMBlockASTNode : public ASTNode
{
public:
    std::vector<std::vector<Token>> instructions;

    LLVMBlockASTNode() = default;
    void accept(ASTVisitor &visitor) override;
};

// Class for asm block statements
class LLVMBlockStmntASTNode : public ASTNode
{
public:
    std::shared_ptr<LLVMBlockASTNode> block;

    LLVMBlockStmntASTNode(std::shared_ptr<LLVMBlockASTNode> blk) : block(blk) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for match statements
class MatchStmtASTNode : public ASTNode
{
public:
    ASTNodePtr expression;
    std::vector<std::shared_ptr<ASTNode>> cases;

    MatchStmtASTNode(ASTNodePtr expr) : expression(expr) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for case clauses in match statements
class CaseClauseASTNode : public ASTNode
{
public:
    ASTNodePtr value;
    std::shared_ptr<BlockASTNode> body;

    CaseClauseASTNode(ASTNodePtr val, std::shared_ptr<BlockASTNode> bdy) : value(val), body(bdy) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for jump statements (break and continue)
enum class JumpType
{
    BREAK,
    CONTINUE
};

class JumpASTNode : public ASTNode
{
public:
    JumpType type;

    JumpASTNode(JumpType t) : type(t) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for enum declarations
class EnumDeclASTNode : public ASTNode
{
public:
    Token name;
    std::vector<Token> values;

    EnumDeclASTNode(const Token &nm) : name(nm) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for struct declarations
class StructDeclASTNode : public ASTNode
{
public:
    Token name;
    std::vector<std::pair<std::string, Token>> members;

    StructDeclASTNode(const Token &nm) : name(nm) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for function declarations
class FunctionDeclASTNode : public ASTNode
{
public:
    Token name;
    std::vector<ASTNodePtr> params;
    ASTNodePtr returnType;
    std::shared_ptr<BlockASTNode> body;

    FunctionDeclASTNode(const Token &nm) : name(nm) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for function parameters
class ParameterASTNode : public ASTNode
{
public:
    Token type;
    Token name;

    ParameterASTNode(const Token &tp, const Token &nm) : type(tp), name(nm) {}
    void accept(ASTVisitor &visitor) override;
};

// Class for return types
class ReturnTypeASTNode : public ASTNode
{
public:
    Token type;

    ReturnTypeASTNode(const Token &tp) : type(tp) {}
    void accept(ASTVisitor &visitor) override;
};

class ASTVisitor {
public:
    virtual void visit(LiteralASTNode &node) = 0;
    virtual void visit(IdentifierASTNode &node) = 0;
    virtual void visit(VarDeclASTNode &node) = 0;
    virtual void visit(BinaryOp &node) = 0;
    virtual void visit(UnaryOp &node) = 0;
    virtual void visit(BlockASTNode &node) = 0;
    virtual void visit(IfStmtASTNode &node) = 0;
    virtual void visit(LoopStmtASTNode &node) = 0;
    virtual void visit(ReturnStmtASTNode &node) = 0;
    virtual void visit(AsmBlockASTNode &node) = 0;
    virtual void visit(AsmBlockStmntASTNode &node) = 0;
    virtual void visit(LLVMBlockASTNode &node) = 0;
    virtual void visit(LLVMBlockStmntASTNode &node) = 0;
    virtual void visit(MatchStmtASTNode &node) = 0;
    virtual void visit(CaseClauseASTNode &node) = 0;
    virtual void visit(JumpASTNode &node) = 0;
    virtual void visit(EnumDeclASTNode &node) = 0;
    virtual void visit(StructDeclASTNode &node) = 0;
    virtual void visit(FunctionDeclASTNode &node) = 0;
    virtual void visit(ParameterASTNode &node) = 0;
    virtual void visit(ReturnTypeASTNode &node) = 0;
};


#endif // AST_HH
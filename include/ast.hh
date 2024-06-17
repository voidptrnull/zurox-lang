#ifndef AST_HH
#define AST_HH

#include <memory>
#include <vector>
#include <string>
#include "token.hh"

// Forward declaration of AST classes
class ASTNode;
class ProgramNode;
class DeclarationNode;
class FunctionDeclarationNode;
class EnumDeclarationNode;
class StructDeclarationNode;
class ParameterNode;
class BlockNode;
class StatementNode;
class IfStatementNode;
class LoopStatementNode;
class VarDeclarationNode;
class ExpressionStatementNode;
class MatchStatementNode;
class CaseClauseNode;
class BreakStatementNode;
class ContinueStatementNode;
class EnumFieldsNode;
class StructFieldsNode;
class ExpressionNode;
class BinaryExprNode;
class UnaryExprNode;
class PrimaryExprNode;
class LiteralNode;
class TypeNode;
class IdentifierNode;

// Base AST
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// Program node representing the entire program
class ProgramNode : public ASTNode {
public:
    ProgramNode(std::vector<std::shared_ptr<DeclarationNode>> declarations)
        : declarations(std::move(declarations)) {}
    std::vector<std::shared_ptr<DeclarationNode>> declarations;
};

// Base class for declarations
class DeclarationNode : public ASTNode {};

// Function declaration node
class FunctionDeclarationNode : public DeclarationNode {
public:
    FunctionDeclarationNode(std::string name, std::vector<std::shared_ptr<ParameterNode>> parameters,
                            std::shared_ptr<TypeNode> return_type, std::shared_ptr<BlockNode> body)
        : name(std::move(name)), parameters(std::move(parameters)), return_type(std::move(return_type)), body(std::move(body)) {}
    std::string name;
    std::vector<std::shared_ptr<ParameterNode>> parameters;
    std::shared_ptr<TypeNode> return_type;
    std::shared_ptr<BlockNode> body;
};


// Parameter node
class ParameterNode : public ASTNode {
public:
    ParameterNode(std::shared_ptr<TypeNode> type, std::string name)
        : type(std::move(type)), name(std::move(name)) {}

    std::shared_ptr<TypeNode> type;
    std::string name;
};

// Base class for statements
class StatementNode : public ASTNode {};

// Block node
class BlockNode : public StatementNode {
public:
    BlockNode(std::vector<std::shared_ptr<StatementNode>> statements)
        : statements(std::move(statements)) {}

    std::vector<std::shared_ptr<StatementNode>> statements;
};

// If statement node
class IfStatementNode : public StatementNode {
public:
    IfStatementNode(std::shared_ptr<ExpressionNode> condition, std::shared_ptr<BlockNode> then_block,
                    std::vector<std::shared_ptr<IfStatementNode>> elif_statements,
                    std::shared_ptr<BlockNode> else_block)
        : condition(std::move(condition)), then_block(std::move(then_block)),
          elif_statements(std::move(elif_statements)), else_block(std::move(else_block)) {}

    std::shared_ptr<ExpressionNode> condition;
    std::shared_ptr<BlockNode> then_block;
    std::vector<std::shared_ptr<IfStatementNode>> elif_statements;
    std::shared_ptr<BlockNode> else_block;
};

// Loop statement node
class LoopStatementNode : public StatementNode {
public:
    LoopStatementNode(std::shared_ptr<BlockNode> body)
        : body(std::move(body)) {}


    std::shared_ptr<BlockNode> body;
};

// Variable declaration node
class VarDeclarationNode : public StatementNode {
public:
    VarDeclarationNode(std::shared_ptr<TypeNode> type, std::string name, std::shared_ptr<ExpressionNode> initializer)
        : type(std::move(type)), name(std::move(name)), initializer(std::move(initializer)) {}


    std::shared_ptr<TypeNode> type;
    std::string name;
    std::shared_ptr<ExpressionNode> initializer;
};

// Expression statement node
class ExpressionStatementNode : public StatementNode {
public:
    ExpressionStatementNode(std::shared_ptr<ExpressionNode> expression)
        : expression(std::move(expression)) {}


    std::shared_ptr<ExpressionNode> expression;
};

// Match statement node
class MatchStatementNode : public StatementNode {
public:
    MatchStatementNode(std::vector<std::shared_ptr<CaseClauseNode>> cases, std::shared_ptr<BlockNode> default_block)
        : cases(std::move(cases)), default_block(std::move(default_block)) {}


    std::vector<std::shared_ptr<CaseClauseNode>> cases;
    std::shared_ptr<BlockNode> default_block;
};

// Case clause node
class CaseClauseNode : public ASTNode {
public:
    CaseClauseNode(std::shared_ptr<LiteralNode> literal, std::shared_ptr<BlockNode> block)
        : literal(std::move(literal)), block(std::move(block)) {}


    std::shared_ptr<LiteralNode> literal;
    std::shared_ptr<BlockNode> block;
};

// Break statement node
class BreakStatementNode : public StatementNode {};

// Continue statement node
class ContinueStatementNode : public StatementNode {};

// Enum declaration node
class EnumDeclarationNode : public DeclarationNode {
public:
    EnumDeclarationNode(std::string name, std::vector<std::string> fields)
        : name(std::move(name)), fields(std::move(fields)) {}


    std::string name;
    std::vector<std::string> fields;
};

// Struct declaration node
class StructDeclarationNode : public DeclarationNode {
public:
    StructDeclarationNode(std::string name, std::vector<std::pair<std::shared_ptr<TypeNode>, std::string>> fields)
        : name(std::move(name)), fields(std::move(fields)) {}


    std::string name;
    std::vector<std::pair<std::shared_ptr<TypeNode>, std::string>> fields;
};

// Expression node base class
class ExpressionNode : public ASTNode {};

// Binary expression node
class BinaryExprNode : public ExpressionNode {
public:
    BinaryExprNode(std::shared_ptr<ExpressionNode> left, std::string op, std::shared_ptr<ExpressionNode> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}


    std::shared_ptr<ExpressionNode> left;
    std::string op;
    std::shared_ptr<ExpressionNode> right;
};

// Unary expression node
class UnaryExprNode : public ExpressionNode {
public:
    UnaryExprNode(std::string op, std::shared_ptr<ExpressionNode> operand)
        : op(std::move(op)), operand(std::move(operand)) {}


    std::string op;
    std::shared_ptr<ExpressionNode> operand;
};

// Primary expression node
class PrimaryExprNode : public ExpressionNode {
public:
    PrimaryExprNode(std::string value)
        : value(std::move(value)) {}


    std::string value;
};

// Literal node
class LiteralNode : public ExpressionNode {
public:
    LiteralNode(const std::string &value, TokenType type)
        : value(value), type(type) {}

    std::string value;
    TokenType type;
};

// Type node
class TypeNode : public ASTNode {
public:
    TypeNode(std::string name)
        : name(std::move(name)) {}


    std::string name;
};

// Identifier node
class IdentifierNode : public ASTNode {
public:
    IdentifierNode(std::string name)
        : name(std::move(name)) {}


    std::string name;
};

#endif
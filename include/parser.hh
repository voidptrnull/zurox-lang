#ifndef PARSER_HH
#define PARSER_HH

#include <print.hh>
#include <token.hh>
#include <vector>
#include <string>
#include <table.hh>
#include <type_traits>
#include <memory>

class Node
{
public:
    Node() = default;
    virtual ~Node() = default;
    virtual void accept(Visitor &visitor) = 0;
};

class Body : public Node
{
public:
    Body() = default;
    ~Body();

    void accept(Visitor &visitor) override;

    std::vector<std::shared_ptr<Node>> statements;
};

class Expression : public Node
{
public:
    Expression() = default;
    ~Expression() = default;

    void accept(Visitor &visitor) override;

    Token op;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};

class Elif : public Node
{
public:
    Elif() = default;
    ~Elif() = default;

    void accept(Visitor &visitor) override;

    Body body;
    Expression condition;
};

class If : public Node
{
public:
    If() = default;
    ~If();

    void accept(Visitor &visitor) override;

    std::vector<std::unique_ptr<Elif>> elifBranches;
    std::shared_ptr<Body> elseBranch;
    Expression condition;
    Body body;
};

class Continue : public Node
{
public:
    void accept(Visitor &visitor) override;
};

class Break : public Node
{
public:
    void accept(Visitor &visitor) override;
};

class Loop : public Node
{
public:
    Loop() = default;
    ~Loop() = default;

    void accept(Visitor &visitor) override;

    std::shared_ptr<Body> body;
};

class CaseClause : public Node
{
public:
    CaseClause() = default;
    ~CaseClause() = default;

    void accept(Visitor &visitor) override;

    std::shared_ptr<Body> body;
    Token literal;
};

class Match : public Node
{
public:
    Match() = default;
    ~Match() = default;

    void accept(Visitor &visitor) override;

    std::vector<std::shared_ptr<CaseClause>> caseClauses;
    std::shared_ptr<Body> nullClause;
};

class RootNode : public Node
{
public:
    RootNode() = default;
    ~RootNode() = default;

    void accept(Visitor &visitor) override;

    std::vector<std::shared_ptr<Node>> nodes;
};

class Variable : public Node
{
public:
    Variable() = default;
    ~Variable() = default;

    void accept(Visitor &visitor) override;

    VarType type;
    bool reference;
    Token variable;
};

class Parser : public Visitor
{
public:
    Parser(std::vector<Token> &tokens, PrintGlobalState &print, std::string &file);
    RootNode parse();

private:
    int_t pos;
    PrintGlobalState print;
    std::vector<Token> tokens;
    TokenType current();
    std::string lcurrent();
    int_t line_current();
    int_t ccurrent();
    std::string file;
    RootNode node;
    void consume();
    void visit(Body &node);
    void visit(Expression &node);
    void visit(Elif &node);
    void visit(If &node);
    void visit(Continue &node);
    void visit(Break &node);
    void visit(Loop &node);
    void visit(CaseClause &node);
    void visit(Match &node);
    void visit(RootNode &node);
    void visit(Variable &node);
};

#endif
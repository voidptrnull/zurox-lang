#ifndef VISITOR_HH
#define VISITOR_HH

class Body;
class Expression;
class Elif;
class If;
class Continue;
class Break;
class Loop;
class CaseClause;
class Match;
class RootNode;
class Variable;

class Visitor {
public:
    virtual void visit(Body& node) = 0;
    virtual void visit(Expression& node) = 0;
    virtual void visit(Elif& node) = 0;
    virtual void visit(If& node) = 0;
    virtual void visit(Continue& node) = 0;
    virtual void visit(Break& node) = 0;
    virtual void visit(Loop& node) = 0;
    virtual void visit(CaseClause& node) = 0;
    virtual void visit(Match& node) = 0;
    virtual void visit(RootNode& node) = 0;
    virtual void visit(Variable& node) = 0;

    virtual ~Visitor() = default;
};

enum VarType
{
    V_I8,
    V_I16,
    V_I32,
    V_I64,
    V_I128,

    V_U8,
    V_U16,
    V_U32,
    V_U64,
    V_U128,

    V_F32,
    V_F64,
    V_80,
    V_F128,
    V_FN,
    V_STRUCT,
    V_ENUM,
    ERR
};

#endif
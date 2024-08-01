#include <ast.hh>

void LiteralASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void IdentifierASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void VarDeclASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void BinaryOp::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void UnaryOp::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void BlockASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void IfStmtASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void LoopStmtASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void ReturnStmtASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void AsmBlockASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void AsmBlockStmntASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void LLVMBlockASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void LLVMBlockStmntASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void MatchStmtASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void CaseClauseASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void JumpASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void EnumDeclASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void StructDeclASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void FunctionDeclASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void ParameterASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

void ReturnTypeASTNode::accept(ASTVisitor &visitor)
{
    visitor.visit(*this);
}

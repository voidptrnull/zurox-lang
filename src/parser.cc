#include <vector>
#include <memory>
#include <string>
#include <token.hh>
#include <visitor.hh>
#include <parser.hh>

Body::~Body() {
    statements.clear();
}

void Body::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Elif::accept(Visitor& visitor) {
    visitor.visit(*this);
}

If::~If() {
    elifBranches.clear();
}

void If::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Continue::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Break::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Loop::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void CaseClause::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Match::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void RootNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Variable::accept(Visitor& visitor) {
    visitor.visit(*this);
}
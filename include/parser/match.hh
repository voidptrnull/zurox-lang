#ifndef MATCH_HH
#define MATCH_HH

#include <node.hh>
#include <body.hh>
#include <vector>
#include <memory>

namespace parser
{
    class CaseClause : public Node
    {
    public:
        Body *body;
        token::Token literal;
        ~CaseClause() {
            delete body;
        }
    };

    class Match : public Node
    {
    public:
        std::vector<std::shared_ptr<CaseClause>> caseClauses;
        Body *nullClause;
        ~Match() {
            delete nullClause;
        }
    };
}

#endif
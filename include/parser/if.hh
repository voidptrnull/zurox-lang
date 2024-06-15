#ifndef IF_HH
#define IF_HH

#include <node.hh>
#include <expression.hh>
#include <body.hh>
#include <vector>
#include <memory>

namespace parser
{

    class Elif : public Node
    {
    public:
        Body body;
        Expression condition;
    };

    class If : public Node
    {
    public:
        std::vector<std::unique_ptr<Elif>> elifBranches;
        std::unique_ptr<Body> elseBranch;
        Expression condition;
        Body body;
    };
}

#endif
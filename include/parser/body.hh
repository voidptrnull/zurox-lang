#ifndef BODY_HH
#define BODY_HH

#include <node.hh>
#include <vector>
#include <memory>

namespace parser
{
    class Body : public Node
    {
    public:
        std::vector<std::shared_ptr<Node>> statements;
    };
}

#endif
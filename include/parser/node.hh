#ifndef NODE_HH
#define NODE_HH

#include <string>
#include <token.hh>

namespace parser
{
    class Node
    {
    public:
        virtual ~Node() = default;
    };
}

#endif
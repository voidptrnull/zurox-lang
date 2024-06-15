#ifndef EXPRESSION_HH
#define EXPRESSION_HH

#include <node.hh>

namespace parser
{
    class Expression : public Node
    {
    public:
        token::Token op;
        Node *left;
        Node *right;
        ~Expression() {
            delete left;
            delete right;
        }
    };
}

#endif
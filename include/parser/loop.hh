#ifndef LOOP_HH
#define LOOP_HH

#include <node.hh>
#include <expression.hh>
#include <body.hh>

namespace parser {
    class While : public Node {
    public:
    Expression *condition;
    Body *body;
    ~While() {
            delete condition;
            delete body;
        }
    };
}

#endif
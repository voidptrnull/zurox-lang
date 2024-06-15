#ifndef ROOT_HH
#define ROOT_HH

#include <vector>
#include <memory>
#include <node.hh>

namespace parser {
    class RootNode : public Node {
        std::vector<std::shared_ptr<Node>> nodes;
        ~RootNode() {
            nodes.clear();
        }
    };
}

#endif
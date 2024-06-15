#ifndef PARSER_HH
#define PARSER_HH

#include <parser/node.hh>
#include <parser/body.hh>
#include <parser/expression.hh>
#include <parser/var.hh>
#include <parser/if.hh>
#include <parser/match.hh>
#include <parser/loop.hh>
#include <parser/jump.hh>
#include <parser/table.hh>
#include <parser/root.hh>

namespace parser
{
    class Parser
    {
    public:
        Parser(std::vector<token::Token> &tokens);
        Node parse();

    private:
        int_t pos;
        std::vector<token::Token> tokens;
        token::TokenType current();
        std::string lcurrent();
        void consume();
        Node root;
        Node parseStatements();
        Node parseBody();
        Node parseExpr();
        Node parseIf();
        Node parseJump();
        Node parseLoop();
        Node parseMatch();
        Node parseVar();
    
    };
}
#endif
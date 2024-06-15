#ifndef TABLE_HH
#define TABLE_HH

#include <string>
#include <vector>
#include <unordered_map>
#include <token.hh>
#include <print.hh>
#include <var.hh>

namespace parser
{
    // Reuse definitions
    typedef parser::VarType SymbolType;

    struct Symbol
    {
        std::string name;
        SymbolType type;
        int_t level;
        int_t space;
    };

    class SymbolTable
    {
    public:
        SymbolTable(util::PrintGlobalState &state);
        void insert(const std::string &name, const Symbol &symbol);
        bool lookup(const std::string &name, Symbol &symbol) const;
        Symbol get(const std::string &name) const;
        SymbolType getType(const std::string &name) const;
        bool check(const std::string &name) const;
        void enterScope();
        void exitScope();

    private:
        std::vector<std::unordered_map<std::string, Symbol>> scopes;
        util::PrintGlobalState state;
    };
}

#endif
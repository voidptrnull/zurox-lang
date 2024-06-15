#include <table.hh>

parser::SymbolTable::SymbolTable(util::PrintGlobalState &state)
{
    this->state = state;
}

void parser::SymbolTable::insert(const std::string &name, const Symbol &symbol)
{
    if (!scopes.empty())
    {
        if (scopes.back().find(name) == scopes.back().end())
        {
            scopes.back()[name] = symbol;
        }
        else
        {
            state.error("Redeclaration of identifier: " + name);
        }
    }
    else
    {
        state.error("No scope to insert into");
    }
}

bool parser::SymbolTable::lookup(const std::string &name, Symbol &symbol) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            symbol = found->second;
            return true;
        }
    }
    return false;
}

parser::SymbolType parser::SymbolTable::getType(const std::string &name) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            return found->second.type;
        }
    }
    this->state.error("Symbol not found: " + name);
    return SymbolType::ERR;
}

parser::Symbol parser::SymbolTable::get(const std::string &name) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            return found->second;
        }
    }
    this->state.error("Symbol not found: " + name);
    return {"", SymbolType::ERR, 0};
}

bool parser::SymbolTable::check(const std::string &name) const
{
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
    {
        if (it->find(name) != it->end())
        {
            return true;
        }
    }
    return false;
}

void parser::SymbolTable::enterScope()
{
    scopes.push_back({});
}

void parser::SymbolTable::exitScope()
{
    if (!scopes.empty())
    {
        scopes.pop_back();
    }
    else
    {
        state.error("No scope to exit from");
    }
}
#ifndef PRINT_HH
#define PRINT_HH

#include <token.hh>
#include <iostream>
#include <sstream>

class PrintGlobalState
{
public:
    PrintGlobalState();
    void reset();
    bool hasEncounteredError() const;
    void error(const std::string &message, int_t line, int_t col, const std::string &file) const;
    void error(const std::string &message) const;
    void warn(const std::string &message, int_t line, int_t col, const std::string &file);
    void warn(const std::string &message) const;
    void info(const std::string &message, int_t line, int_t col, const std::string &file);
    void info(const std::string &message) const;
    void flush() const;

private:
    mutable bool erroneous;
    mutable std::stringstream buffer;

    void printFile(int_t line, int_t col, const std::string &file) const;
};

#endif
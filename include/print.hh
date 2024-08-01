/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */
#ifndef PRINT_HH
#define PRINT_HH

#include <token.hh>
#include <sstream>

class PrintGlobalState
{
public:
    PrintGlobalState();
    void reset();
    bool hasEncounteredError() const;
    void printFilename(const std::string& name);
    void error(const std::string &message, int_t line, int_t col, const std::string &file) const;
    void error(const std::string &message) const;
    void warn(const std::string &message, int_t line, int_t col, const std::string &file);
    void warn(const std::string &message) const;
    void info(const std::string &message, int_t line, int_t col, const std::string &file);
    void info(const std::string &message) const;
    void flush() const;
    inline void increment(bool error);
    int_t getError() const;
    int_t getWarns() const;

private:
    mutable bool erroneous;
    mutable std::stringstream buffer;
    mutable int_t errors, warns;

    void printFile(int_t line, int_t col, const std::string &file) const;
};

#endif

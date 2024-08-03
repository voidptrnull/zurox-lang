/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */

#include <print.hh>
#include <iostream>

PrintGlobalState::PrintGlobalState() : erroneous(false) {}

void PrintGlobalState::reset()
{
    erroneous = false;
    buffer.str("");
    buffer.clear();
}

bool PrintGlobalState::hasEncounteredError() const
{
    return erroneous;
}

void PrintGlobalState::printFilename(const std::string &name)
{
    buffer << "\x1b[37mIn file \x1b[37;1m" << name << "\x1b[0m:";
}

void PrintGlobalState::error(const std::string &message, int_t line, int_t col, const std::string &file) const
{
    erroneous = true;
    buffer << "\x1b[31;1merror:\x1b[0m " << message << "\n";
    printFile(line, col, file);
}

void PrintGlobalState::error(const std::string &message) const
{
    erroneous = true;
    buffer << "\x1b[31;1merror:\x1b[0m " << message << "\n";
}

void PrintGlobalState::warn(const std::string &message, int_t line, int_t col, const std::string &file)
{
    buffer << "\x1b[33;1mwarn:\x1b[0m " << message << "\n";
    printFile(line, col, file);
}

void PrintGlobalState::warn(const std::string &message) const
{
    buffer << "\x1b[33;1mwarn:\x1b[0m " << message << "\n";
}

void PrintGlobalState::info(const std::string &message, int_t line, int_t col, const std::string &file)
{
    buffer << "\x1b[36;1minfo:\x1b[0m " << message << "\n";
    printFile(line, col, file);
}

void PrintGlobalState::info(const std::string &message) const
{
    buffer << "\x1b[36;1minfo:\x1b[0m " << message << "\n";
}

void PrintGlobalState::printFile(int_t line, int_t col, const std::string &file) const
{
    if (line <= 0 || col <= 0 || static_cast<int_t>(line) > file.size())
    {
        buffer << "Invalid line or column number.\n";
        return;
    }

    int_t index = 0;
    int_t current_line = 1;
    int_t start_of_line = 0;

    while (index < file.size())
    {
        if (current_line == line)
        {
            start_of_line = index;
            break;
        }

        if (file[index] == '\n')
        {
            current_line++;
        }
        index++;
    }

    int_t end_of_line = file.find('\n', start_of_line);
    if (end_of_line == std::string::npos)
    {
        end_of_line = file.size();
    }

    std::string line_str = file.substr(start_of_line, end_of_line - start_of_line);
    std::string marker(line_str.size() + 1, ' ');

    if (col - start_of_line < line_str.size())
    {
        marker[col - start_of_line] = '^';
    }

    int_t line_num_width = std::to_string(line).size();

    buffer << std::string(line_num_width, ' ') << " | " << "\n";
    buffer << line << " | " << line_str << '\n';
    buffer << std::string(line_num_width, ' ') << " | " << marker << "\n";
}

void PrintGlobalState::flush() const
{
    std::cout << buffer.str();
    buffer.str("");
    buffer.clear();
}

int_t PrintGlobalState::getError() const
{
    return errors;
}

int_t PrintGlobalState::getWarns() const
{
    return warns;
}

void PrintGlobalState::increment(bool error)
{
    (error) ? errors++ : warns++;
}
#include <iostream>
#include <string>
#include "token.hh"
#include "print.hh"

PrintGlobalState::PrintGlobalState() : erroneous(false) {}

void PrintGlobalState::reset()
{
    erroneous = false;
}

bool PrintGlobalState::hasEncounteredError() const
{
    return erroneous;
}

void PrintGlobalState::error(const std::string &message, int_t line, int_t col, const std::string &file)
{
    erroneous = true;
    std::cerr << "\x1b[31;1merror:\x1b[0m " << message << std::endl;
    printFile(line, col, file);
}

void PrintGlobalState::error(const std::string &message) const
{
    erroneous = true;
    std::cerr << "\x1b[31;1merror:\x1b[0m " << message << std::endl;
}

void PrintGlobalState::warn(const std::string &message, int_t line, int_t col, const std::string &file)
{
    std::cerr << "\x1b[33;1mwarn:\x1b[0m " << message << std::endl;
    printFile(line, col, file);
}

void PrintGlobalState::warn(const std::string &message) const
{
    std::cerr << "\x1b[33;1mwarn:\x1b[0m " << message << std::endl;
}

void PrintGlobalState::info(const std::string &message, int_t line, int_t col, const std::string &file)
{
    std::cerr << "\x1b[36;1minfo:\x1b[0m " << message << std::endl;
    printFile(line, col, file);
}

void PrintGlobalState::info(const std::string &message) const
{
    std::cerr << "\x1b[36;1minfo:\x1b[0m " << message << std::endl;
}

void PrintGlobalState::printFile(int_t line, int_t col, const std::string &file) const {
    if (line <= 0 || col <= 0 || static_cast<int_t>(line) > file.size()) {
        std::cerr << "Invalid line or column number." << std::endl;
        return;
    }

    int_t index = 0;
    int_t current_line = 1;
    int_t start_of_line = 0;

    while (index < file.size()) {
        if (current_line == line) {
            start_of_line = index;
            break;
        }

        if (file[index] == '\n') {
            current_line++;
        }
        index++;
    }

    int_t end_of_line = file.find('\n', start_of_line);
    if (end_of_line == std::string::npos) {
        end_of_line = file.size();
    }

    std::string line_str = file.substr(start_of_line, end_of_line - start_of_line);
    std::string marker(line_str.size(), ' ');

    if (col - start_of_line <= line_str.size()) {
        marker[col - start_of_line - 1] = '^';
    }

    int_t line_num_width = std::to_string(line).size();

    std::cout << line << " | " << line_str << '\n';
    std::cout << std::string(line_num_width, ' ') << " | " << marker << "\n";
}
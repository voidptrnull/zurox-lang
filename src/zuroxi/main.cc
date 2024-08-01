/**
 * This file is part of the Zurox project.
 * Licensed under the BSD 3-Clause License. See LICENSE file for details.
 * (C) 2024 Subhadip Roy Chowdhury
 */
#include <iostream>
#include <string>
#include <version.hh>
#include <print.hh>
#include <lexer.hh>
#include <parser.hh>
#include <fstream>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/TargetParser/Triple.h>

void read_file(std::string &file, const std::string &file_name, const PrintGlobalState &print)
{
    std::ifstream file_stream(file_name);
    if (!file_stream)
    {
        print.error("File '" + file_name + "' not found !!!");
        throw std::runtime_error("Unable to continue execution.");
    }

    file_stream.seekg(0, std::ios::end);
    std::streamsize size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file_stream.read(buffer.data(), size))
    {
        print.error("Error reading file '" + file_name + "' !!!");
        throw std::runtime_error("Error reading file: " + file_name);
    }

    file.assign(buffer.begin(), buffer.end());
}

int main(int argc, char **argv)
{
    std::vector<std::string> files;
    PrintGlobalState print;
    llvm::cl::SetVersionPrinter([](llvm::raw_ostream &O)
                                { O << "Zurox Interpreter " << get_version() << "\n"; });

    llvm::StringMap<llvm::cl::Option *> &Map = llvm::cl::getRegisteredOptions();

    for (auto &OptionPair : Map)
    {
        if (OptionPair.getKey() != "version" &&
            OptionPair.getKey() != "help")
        {
            OptionPair.getValue()->setHiddenFlag(llvm::cl::ReallyHidden);
        }
    }

    llvm::cl::list<std::string> InputFiles(llvm::cl::Positional, llvm::cl::desc("<input files>"), llvm::cl::OneOrMore, llvm::cl::Required);
    llvm::cl::ParseCommandLineOptions(argc, argv, "Zurox Interpreter\n", nullptr, nullptr, false);

    for (std::string file_name : files) {
        std::string file;
        read_file(file, file_name, print);
        Lexer lexer(file, file_name, print);
        std::vector<std::shared_ptr<ASTNode>> nodes;
        Parser parser(nodes, lexer.lex(), print, file);
    }

    return 0;
}

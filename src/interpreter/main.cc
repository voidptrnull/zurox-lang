#include <iostream>
#include <string>
#include <version.hh>
#include <print.hh>
#include <sstream>
#include <fstream>
#include <cctype>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/TargetParser/Triple.h>

void read_file(std::string &file, const std::string &file_name, const PrintGlobalState &print)
{
    std::ifstream file_stream(file_name, std::ios::binary | std::ios::ate);
    if (!file_stream)
    {
        print.error("File '" + file_name + "' not found !!!");
        exit(127);
    }
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    file = buffer.str();
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

    // Process other options and logic here
    //

    return 0;
}

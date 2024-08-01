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
#include <sstream>
#include <fstream>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/TargetParser/Triple.h>

enum OptimizationLevel
{
    g,
    O0,
    O1,
    O2,
    O3,
    Os,
    Oz
};

enum StageType
{
    c,
    S,
    B,
    C
};

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
                                { O << "Zurox Compiler " << get_version() << "\n"; });

    llvm::StringMap<llvm::cl::Option *> &Map = llvm::cl::getRegisteredOptions();

    for (auto &OptionPair : Map)
    {
        if (OptionPair.getKey() != "B" &&
            OptionPair.getKey() != "c" &&
            OptionPair.getKey() != "C" &&
            OptionPair.getKey() != "o" &&
            OptionPair.getKey() != "S" &&
            OptionPair.getKey() != "version" &&
            OptionPair.getKey() != "help")
        {
            // Make sure they dont get shown anyhow.
            OptionPair.getValue()->setHiddenFlag(llvm::cl::ReallyHidden);
        }
    }

    llvm::cl::opt<std::string> Output("o", llvm::cl::desc("Specify the name of the output file."), llvm::cl::value_desc("<path to file>"));
    llvm::cl::opt<StageType> Stage(llvm::cl::desc("Stage types.:"),
                                   llvm::cl::values(
                                       clEnumVal(c, "Run all stages except linking."),
                                       clEnumVal(S, "Specify to only compile files to provide assembly."),
                                       clEnumVal(B, "Specify to output the LLVM IR."),
                                       clEnumVal(C, "Check if the code compiles, do not produce any files.")));

    llvm::cl::opt<OptimizationLevel> OptimizationLevel(llvm::cl::desc("Choose optimization level:"),
                                                       llvm::cl::values(
                                                           clEnumVal(g, "No optimizations, enable debugging"),
                                                           clEnumVal(O0, "Perform very little optimizations."),
                                                           clEnumVal(O1, "Enable trivial optimizations for fast execution and debugging."),
                                                           clEnumVal(O2, "Enable default optimizations for performance."),
                                                           clEnumVal(O3, "Enable expensive optimizations for performance."),
                                                           clEnumVal(Os, "Enable default optimizations but for binary size."),
                                                           clEnumVal(Oz, "Enable expensive optimizations for binary size. Produce the smallest possible code.")));
    llvm::cl::opt<std::string> March(llvm::cl::desc("Choose target architecture."), llvm::cl::value_desc("architecture name"));
    llvm::cl::list<std::string> InputFiles(llvm::cl::Positional, llvm::cl::desc("<input files>"), llvm::cl::OneOrMore, llvm::cl::Required);

    llvm::cl::ParseCommandLineOptions(argc, argv, "Zurox Programming Language Compiler\n", nullptr, nullptr, false);
    if (!March.empty())
    {
        llvm::Triple triple;
        auto type = triple.getArchTypeForLLVMName(March);
        if (type == llvm::Triple::ArchType::UnknownArch)
        {
            print.error("Unrecognised option `" + March + "`. Error is fatal. Can not continue execution. Exiting....");
            print.info("You are supposed to pass the architecture name like `skylake` for `x86-64`. See LLVM triplets for more information.");
            print.flush();
            return 255;
        }
    }

    for (std::string ref : InputFiles)
    {
        std::string file;
        read_file(file, ref, print);
        Lexer lexer(file, ref, print);
        auto tokens = lexer.lex();
        std::vector<std::shared_ptr<ASTNode>> nodes;
        Parser parser(nodes, tokens, print, file);
        parser.parse();
    }

    return 0;
}

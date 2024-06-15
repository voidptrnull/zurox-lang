#include <iostream>
#include <string>
#include <vector>
#include "version.hh"
#include "lexer.hh"
#include "parser.hh"
#include "print.hh"

const char* HELP_TEXT =
    "Flags              Description\n"
    "   -o,--output     The name of the output files.\n"
    "   -c,--compile    *Specify to only compile files and produce object files, not link them.\n"
    "   -S,--asm        *Specify to only compile files.\n"
    "   -O,--opt-level  Specify the optimization level, to be followed with an integer like 0,1,2,3 or a character like 's'.\n"
    "   -B,--bitcode    *Specify to output the LLVM IR.\n"
    "   -C,--check      *Specify to check if the code works, won't produce anything.\n"
    "   -h,--help       Shows this message.\n"
    "   -v,--version    Shows the version.\n"
    "Flags whose description starts with '*' cannot be used in combination with one another and only 1 can be used at a time.\n";

void show_version()
{
    std::cout << "Zurox Programming Language " << get_version() << "\n";
}

void show_help(const char* name)
{
    show_version();
    std::cout << "Usage: " << name << " <flags> <file names>" << "\n"
              << HELP_TEXT;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        show_help(argv[0]);
        return 0;
    }

    bool oF = false, OF = false, SF = false, BF = false, cF = false, CF = false;
    std::string opt_level = "0";
    std::string output = "a.out";
    std::vector<std::string> files;
    util::PrintGlobalState print;

    for (int i = 1; i < argc; ++i)
    {
        std::string x = std::string(argv[i]);
        if (x == "-o" || x == "--output")
        {
            if (oF)
            {
                print.error("Output flag can be set only once.");
                exit(255);
            }
            oF = true;

            if (i + 1 < argc)
            {
                output = std::string(argv[++i]);
            }
            else
            {
                print.error("Output flag requires an argument.");
                exit(255);
            }
        }
        else if (x == "-c" || x == "--compile")
        {
            if (cF || SF || BF || CF)
            {
                print.error("Only one of the compile, asm, bitcode, or check flags can be set.");
                exit(254);
            }
            cF = true;
        }
        else if (x == "-S" || x == "--asm")
        {
            if (cF || SF || BF || CF)
            {
                print.error("Only one of the compile, asm, bitcode, or check flags can be set.");
                exit(253);
            }
            SF = true;
        }
        else if (x == "-O" || x == "--opt-level")
        {
            if (OF)
            {
                print.error("Optimize flag can be set only once.");
                exit(252);
            }
            OF = true;

            if (i + 1 < argc)
            {
                opt_level = std::string(argv[++i]);
                if (!opt_level.compare("0") && !opt_level.compare("1") && !opt_level.compare("2") && !opt_level.compare("3") && !opt_level.compare("s")) {
                    print.error("Invalid optimization level: " + opt_level);
                    exit(252);
                }
                
            }
            else
            {
                print.error("Optimization level flag requires an argument.");
                exit(252);
            }
        }
        else if (x == "-B" || x == "--bitcode")
        {
            if (cF || SF || BF || CF)
            {
                print.error("Only one of the compile, asm, bitcode, or check flags can be set.");
                exit(251);
            }
            BF = true;
        }
        else if (x == "-C" || x == "--check")
        {
            if (cF || SF || BF || CF)
            {
                print.error("Only one of the compile, asm, bitcode, or check flags can be set.");
                exit(250);
            }
            CF = true;
        }
        else if (x == "-h" || x == "--help")
        {
            show_help(argv[0]);
            exit(0);
        }
        else if (x == "-v" || x == "--version")
        {
            show_version();
            exit(0);
        }
        else
        {
            files.push_back(x);
        }
    }

    if ((SF && BF) || (SF && CF) || (SF && cF) ||
        (BF && CF) || (BF && cF) ||
        (CF && cF))
    {
        print.error("More than 1 flag was set. Run with --help to know more. Cannot continue operation. Exiting....");
        exit(128);
    }

    for (std::string file : files) {
        lexer::Lexer lexer(file, print);
        for (token::Token tok : lexer.lex()) {
            std::cout << "(" << tok.type << "," << tok.line << "," << tok.col << ",\"" << tok.lexeme << "\")\n";
        }
    }

    return 0;
}
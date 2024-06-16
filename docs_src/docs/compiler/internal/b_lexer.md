# 2. Lexer Documentation

## Overview

The `Lexer` class provides a mechanism for lexical analysis of source files.

## Process

The lexer first takes the file and then lexicographically processes the files character-by-character. It then transforms them in tokens with various data. Then it stores them into a vector. Upon reaching the end-of-file, it appends a custom `EOF` token to the vector and returns it to the caller.

Helper functions like `advance`, `peek` and `current` are used to retrieve character information.

The file and it's name is provided through the constructor.

## Example Usage

```cpp
#include <lexer.hh>
#include <iostream>

int main() {
    // Example usage of Lexer
    std::string fileContent = "fn main() { ret 0; }";
    std::string fileName = "example.zx";
    PrintGlobalState printState;

    Lexer lexer(fileContent, fileName, printState);
    auto tokens = lexer.lex();

    // Print tokens
    for (const auto& token : tokens) {
        std::cout << "Token: " << token.type << ", Value: " << token.lexeme << std::endl;
    }

    return 0;
}
```

## Insight

Here is the actual accessible part of the Lexer:
```cpp
#ifndef LEXER_HH
#define LEXER_HH

#include <string>
#include <vector>
#include <token.hh>
#include <print.hh>
#include <llvm/ADT/StringRef.h>

/**
 * @brief Lexer class for lexical analysis of a source file.
 */
class Lexer
{
public:
    /**
     * @brief Constructor for Lexer.
     * @param file Content of the file to lex.
     * @param file_name Name of the file being lexed.
     * @param print PrintGlobalState object for printing.
     */
    Lexer(const std::string &file, const std::string &file_name, PrintGlobalState &print);

    /**
     * @brief Lexical analysis of the file.
     * @return Vector of tokens generated from the lexing process.
     */
    std::vector<Token> lex();

    /**
     * @brief Get the content of the file being lexed.
     * @return Reference to the file content.
     */
    llvm::StringRef getFile();

private:
    size_t line, col;                ///< Current line and column in the file.
    std::vector<Token> tokens;       ///< Tokens generated during lexing.
    std::string file;                ///< Content of the file to lex.
    std::string file_name;           ///< Name of the file being lexed.
    PrintGlobalState &print;         ///< Reference to PrintGlobalState for printing.

    /**
     * @brief Get the current character in the file.
     * @return Current character.
     */
    inline char current() const;

    /**
     * @brief Get the next character in the file.
     * @return Next character.
     */
    inline char peek() const;

    /**
     * @brief Get the previous character in the file.
     * @return Previous character.
     */
    inline char previous() const;

    /**
     * @brief Advance to the next character in the file.
     */
    inline void advance();

    /**
     * @brief Handle keywords, datatypes, or identifiers.
     */
    void keywordOrDatatypeOrIdentifier();

    /**
     * @brief Handle numbers (integers and floats).
     */
    void number();

    /**
     * @brief Check if the character is a separator.
     * @param c Character to check.
     * @return True if character is a separator, false otherwise.
     */
    bool isSeperator(char c) const;

    /**
     * @brief Check if the character is an operator.
     * @param c Character to check.
     * @return True if character is an operator, false otherwise.
     */
    bool isOperator(char c) const;

    /**
     * @brief Handle operators.
     */
    void handleOperator();

    /**
     * @brief Handle string literals.
     */
    void handleStringLiteral();

    /**
     * @brief Handle character literals.
     */
    void handleCharLiteral();

    /**
     * @brief Handle comments.
     */
    void handleComment();
};

#endif
```
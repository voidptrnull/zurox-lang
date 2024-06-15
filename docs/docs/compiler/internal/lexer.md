# Lexer Documentation

## Overview

The `Lexer` class provides a mechanism for lexical analysis of source files.

## Process

The lexer first takes the file and then lexicographically processes the files character-by-character. It then transforms them in tokens with various data. Then it stores them into a vector. Upon reaching the end-of-file, it appends a custom `EOF` token to the vector and returns it to the caller.

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
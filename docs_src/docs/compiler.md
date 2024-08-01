# About

The two folders `internal` and `external` contains the documentation.

The `internal` folder is concerned with the actual implementation.

The `external` folder deals with usage and integration.

## Usage

Check `zuroxc --help` for usage info and `internal` for more details.

## Structure
The compiler & intepreter are implemented in this manner.

- Lexer (Lexical Analysis)

- Parser (Syntax Analysis)

- Analyzer (Semantic Analysis)

### Compiler (Not for Interpreter)
- Code Generator (LLVM IR Generator)


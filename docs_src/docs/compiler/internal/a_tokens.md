# 1. Tokens Documentation

Each token is treated as single word.

To give a vague idea, refer to the below example.

## Token Example
```c
int main() {
    int x = 42;
    return x;
}
```

Here the tokens will be the following:<br>
- `int`<br>
- `main`<br>
- `(`<br>
- `)`<br>
- `{`<br>
- `int`<br>
- `x`<br>
- `=`<br>
- `42`<br>
- `;`<br>
- `return`<br>
- `x`<br>
- `}`<br>

Notice how whitespace is ignored. Also notice how `(` and `)` are present together but treated differently.
But you may wonder now, how do we use this ? Well before we get to that, we will sort of classify them.

## Token Types

Here, the tokens can be roughly categorized into the following:<br>
- Keywords<br>
- Identifiers<br>
- Seperators<br>
- Operators<br>
- Literals<br>
- Custom Tokens like EOF, and ERR tokens.<br>

So take the above mentioned list of tokens for the C code, and you get:<br>
- Keywords like `return`<br>
- Seperators like `(`<br>
- Literals like `42`<br>
- Identifiers like `x`.<br>

Zurox has another category for the tokens, which are the datatypes like `int` in C-like languages or `i32` in Rust and also in Zurox.

This is the enumeration which is actually used. Notice the presence of literals.

```cpp
enum TokenType
{
    TK_DATATYPE,
    TK_KEYWORD,
    TK_SEPERATOR,
    TK_OPERATOR,
    TK_ID,

    TKL_INT,
    TKL_FLOAT,
    TKL_CHAR,
    TKL_STR,

    TK_ERR,
    __EOF,
};
```

But just knowing the type won't do, we also need to know information like line and column number, and most importantly the actual lexeme like `int`, etc. To achieve that, we implement the following struct:
```cpp
struct Token
{
    TokenType type;
    int_t line;
    int_t col;
    std::string lexeme;
    Token(TokenType _type, int_t _line, int_t _col, const std::string &_lexeme)
        : type(_type), line(_line), col(_col), lexeme(_lexeme) {}
    Token(TokenType _type, int_t _line, int_t _col, const char &_lexeme)
        : type(_type), line(_line), col(_col)
    {
        lexeme.push_back(_lexeme);
    }
};
```
>Note: `int_t` is just `unsigned long int`.

The constructors are used to create a new token, where the lexeme may be a single character as in case of seperators or a string.
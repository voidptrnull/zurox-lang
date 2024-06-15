# Tokens Documentation

Each token is treated as single word.

To give a vague idea, refer to the below example.

## Token Example
```c
int main() {
    int x = 42;
    return x;
}
```

Here the tokens will be the following:
- `int`
- `main`
- `(`
- `)`
- `{`
- `int`
- `x`
- `=`
- `42`
- `;`
- `return`
- `x`
- `}`

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
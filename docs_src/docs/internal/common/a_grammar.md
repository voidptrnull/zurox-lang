# Grammar

Before delving more into the implementation of the language, it is important to state the obvious - the grammar of the language. So here it is in ISO 14977 E-BNF format:

>Note: If it is not obvious, the grammar of the language is CFG

```ebnf
integer = decimal_integer | hexadecimal_integer | octal_integer | binary_integer ;
decimal_integer = ["-"], decimal_digit, { decimal_digit }, [integer_suffix] ;
hexadecimal_integer = "0x", hex_digit, { hex_digit }, [integer_suffix] ;
octal_integer = "0o", octal_digit, { octal_digit }, [integer_suffix] ;
binary_integer = "0b", binary_digit, { binary_digit }, [integer_suffix] ;

float = decimal_float | hexadecimal_float ;
decimal_float = ["-"], decimal_digit, { decimal_digit }, ".", { decimal_digit }, [exponent], [float_suffix] ;
hexadecimal_float = ["-"], "0x", hex_digit, { hex_digit }, ".", { hex_digit }, [exponent], [float_suffix] ;

exponent = ("e" | "E"), ["+" | "-"], decimal_digit, { decimal_digit } ;
integer_suffix = ("u8" | "u16" | "u32" | "u64" | "u128"  | "i8" | "i16" | "i32" | "i64" | "i128" ) ;
float_suffix = ("f32" | "f64" | "f80" | "f128") ;

decimal_digit = "0" | non_zero_digit, { digit } ;
hex_digit = decimal_digit | "a" | "b" | "c" | "d" | "e" | "f" | "A" | "B" | "C" | "D" | "E" | "F" ;
octal_digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" ;
binary_digit = "0" | "1" ;
non_zero_digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

letter = "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" |
         "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" |
         "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" |
         "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z" ;

(* Unicode is also allowed. I am too lazy to specify it. *)
identifier          = letter , { letter | decimal_digit | "_" } ;
NUMBER              = integer | float ;
STRING              = '"' , { character - '"' } , '"' ;
CHARACTER           = "'" , ( character - "'" ) , "'" ;

literal             = NUMBER
                    | STRING
                    | CHARACTER;

unary_op            = "+"
                    | "-"
                    | "!"
                    | "~"
                    | "ref"
                    | "deref" ;

type                = "i8"
                    | "i16"
                    | "i32"
                    | "i64"
                    | "u8"
                    | "u16"
                    | "u32"
                    | "u64"
                    | "f32"
                    | "f64"
                    | "f80"
                    | "f128"
                    | "char"
                    | "bool"
                    | "struct" , identifier
                    | "enum" , identifier 
                    | "ref" , type
                    | "deref", type ;

primary             = literal
                    | identifier
                    | "(" , expression , ")" ;

unary_expr          = primary
                    | unary_op , unary_expr ;

factor              = unary_expr , { ("*" | "/" | "%") , unary_expr } ;

term                = factor , { ("+" | "-") , factor } ;

shift_expr          = term , { ("<<" | ">>") , term } ;

and_expr            = shift_expr , { "&" , shift_expr } ;

xor_expr            = and_expr , { "^" , and_expr } ;

or_expr             = xor_expr , { "|" , xor_expr } ;

comparison          = or_expr , { ("==" | "!=" | "<" | "<=" | ">" | ">=") , or_expr } ;

logical_and_expr    = comparison , { "&&" , comparison } ;

logical_or_expr     = logical_and_expr , { "||" , logical_and_expr } ;

expression          = logical_or_expr;

parameter           = type , identifier ;

parameters          = parameter , { "," , parameter } ;

return_type         = "->" , type ;

block               = "{" , { statement } , "}" ;

statement           = if_statement
                    | loop_statement
                    | var_declaration
                    | expression_statement
                    | match_statement
                    | break_statement
                    | continue_statement 
                    | ret_statement
                    | asm_block
                    | llvm_block ;

asm_block           = "asm","{", [ asm_statements ], "}" ;

asm_statements      = asm_statement ,{ "," , asm_statement } ;

asm_statement       = STRING , [ "+", identifier, ["+", STRING] ] ;

llvm_block          = "llvm", "{", [ llvm_statements ], "}" ;

llvm_statements     = llvm_statement, { "," , llvm_statement } ;

llvm_statement      = STRING, [ "+", identifier, ["+", STRING] ] ;

ret_statement       = "ret", [expression];

if_statement        = "if" , "(" , expression , ")" , block , { "elif" , "(" , expression , ")" , block } , [ "else" , block ] ;

loop_statement      = "loop" , block ;

var_declaration     = type , identifier , [ "=" , expression ] , ";" ;

expression_statement = expression , ";" ;

match_statement     = "match" , "{" , { case_clause } , [ "_" , "->" , block ] , "}" ;

case_clause         = literal, {",", literal } , "->" , block ;

break_statement     = "break" , ";" ;

continue_statement  = "continue" , ";" ;

enum_fields         = identifier , { "," , identifier } , ";" ;

struct_fields       = type , identifier , { "," , type , identifier } , ";" ;

function_declaration = "fn" , identifier , "(" , [ parameters ] , ")" , [ return_type ] , block ;

enum_declaration    = "enum" , identifier , "{" , enum_fields , "}" ;

struct_declaration  = "struct" , identifier , "{" , struct_fields , "}" ;

declaration         = function_declaration
                    | enum_declaration
                    | struct_declaration 
                    | asm_block
                    | llvm_block ;

program             = { declaration } ;
```
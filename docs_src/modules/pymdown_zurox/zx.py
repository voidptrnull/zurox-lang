from pygments.lexer import RegexLexer, bygroups
from pygments.token import *

class ZxLexer(RegexLexer):
    tokens = {
        'root': [
            (r'\b(u8|u16|u32|u64|i8|i16|i32|i64|f32|f64|u128|i128|f80|f128|char|bool)\b', Keyword.Type),
            (r'\b(if|elif|else|loop|fn|ret|true|false|ref|deref|struct|sync|enum|void|volatile|null|import|break|continue|match)\b', Keyword),
            (r'\b[a-zA-Z_][a-zA-Z0-9_]*\b', Name),
            (r'\b[+-]?(?:\d*\.\d+|\d+\.\d*)(?:[eE][+-]?\d+)?\b', Number.Float),
            (r'\b[+-]?\d+\b', Number.Integer),
            (r'\b(>?>|<<|==|!=|\^|\||&|\+{1,2}|-{1,2}|\*|/|%)\b', Operator),
            (r'[,;{}()[\].]', Punctuation),
            (r'"(?:\\.|[^"\\])*"', String.Double),
            (r"'(?:\\.|[^'\\])*'", String.Single),
            (r'//.*?\n', Comment.Single),
            (r'/\*.*?\*/', Comment.Multiline),
            (r'\s+', Text),
        ]
    }
from pygments.lexer import RegexLexer
from pygments.token import Text, Keyword, Name, String, Number, Operator, Comment

class ZuroxLexer(RegexLexer):
    name = 'Zurox'
    aliases = ['zurox','zx']
    filenames = ['*.zx']

    tokens = {
        'root': [
            (r'\b(ref|deref|loop|break|if|elif|else|ret|asm|llvm|match|case|fn|enum|struct)\b', Keyword),
            (r'\b(i8|i16|i32|i64|u8|u16|u32|u64|f32|f64|f80|f128|char|bool)\b', Name.Builtin),
            (r'\b(true|false)\b', Name.Constant),
            (r'//.*?$', Comment.Single),
            (r'/\*.*?\*/', Comment.Multiline),
            (r'"(\\\\|\\"|[^"])*"', String),
            (r'\'(\\\\|\\\'|[^\'])*\'', String.Char),
            (r'\b[0-9]+(\.[0-9]*)?\b', Number),
            (r'[a-zA-Z_][a-zA-Z0-9_]*', Name),
            (r'[~!%^&*+=|?:<>/-]', Operator),
            (r'\s+', Text),
        ]
    }

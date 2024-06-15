#include <gtest/gtest.h>
#include <lexer.hh>

static const std::string file = "if elif else loop fn ret true false ref deref struct sync enum void volatile null import break continue match"
                                " IF ELIF ELSE LOOP FN RET TRUE FALSE REF DEREF STRUCT SYNC ENUM VOID VOLATILE NULL IMPORT BREAK CONTINUE MATCH";
static PrintGlobalState print;
static Lexer lex(file, "lexer_keywords.zx", print);
static const auto tokens = lex.lex();

TEST(LEXER_KEYWORDS, LEXER_KEYWORD_) {
    for (int i = 0;i < 20;i++) {
        EXPECT_EQ(tokens[i].type, TK_KEYWORD);
    }
}

TEST(LEXER_KEYWORDS, LEXER_ID_) {
    for (int i = 20;i < tokens.size() - 1;i++) {
        EXPECT_EQ(tokens[i].type, TK_ID);
    }
}

TEST(LEXER_KEYWORDS, LEXER_EOF_) {
    EXPECT_EQ(tokens.at(tokens.size() - 1).type, __EOF);
}
#include <gtest/gtest.h>
#include <lexer.hh>

static const std::string file = "// This is a comment;\n/// Another comment\nfn";
static PrintGlobalState print;
static Lexer lex(file, "lexer_comments.zx", print);
static auto tokens = lex.lex();

TEST(LEXER_COMMENT, LEXER_COMMENT_IGNORE_TEST) {
    EXPECT_EQ(tokens.size(), 2u);
}

TEST(LEXER_COMMENT, LEXER_COMMENT_KEYWORD_CHECK) {
    EXPECT_EQ(tokens[0].type, TK_KEYWORD);
}
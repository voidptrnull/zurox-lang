#include <gtest/gtest.h>
#include <lexer.hh>

static const std::string file = "({[;]}),:";

TEST(LEXER_SEPERATOR,LEXER_SEPERATOR) {
    PrintGlobalState print;
    Lexer lex(file,"lexer_seperator.zx",print);
    auto tokens = lex.lex();
    int i = 0;
    for (;i < file.length() - 1;i++) {
        EXPECT_EQ(tokens[i].type, TK_SEPERATOR);
    }
    EXPECT_EQ(tokens[++i].type, TK_ERR);
    
}
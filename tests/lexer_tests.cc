#include <gtest/gtest.h>
#include <lexer.hh>

// Fixture for Lexer tests
class LexerTest : public ::testing::Test {
protected:
    PrintGlobalState print;
    Lexer lexerInstance;

    // Constructor initializes lexerInstance directly
    LexerTest() : lexerInstance("", "", print) {
        // Additional setup if needed
    }
};

// Test case for Lexer::keywordOrDatatypeOrIdentifier() method with keywords
TEST_F(LexerTest, KeywordOrDatatypeOrIdentifierMethodTest_Keywords) {
    // Initialize Lexer directly in the test case
    Lexer lexer("if else ret true false ref deref struct sync enum void volatile null import break continue match", "file_name", print);
    
    auto tokens = lexer.lex();
    print.flush();
    
    ASSERT_EQ(tokens.size(), 18); // Ensure correct number of tokens
    
    EXPECT_EQ(tokens[0].type, TokenType::TK_KEYWORD); // if
    EXPECT_EQ(tokens[1].type, TokenType::TK_KEYWORD); // else
    EXPECT_EQ(tokens[2].type, TokenType::TK_KEYWORD); // ret
    EXPECT_EQ(tokens[3].type, TokenType::TK_KEYWORD); // true
    EXPECT_EQ(tokens[4].type, TokenType::TK_KEYWORD); // false
    EXPECT_EQ(tokens[5].type, TokenType::TK_KEYWORD); // ref
    EXPECT_EQ(tokens[6].type, TokenType::TK_KEYWORD); // deref
    EXPECT_EQ(tokens[7].type, TokenType::TK_KEYWORD); // struct
    EXPECT_EQ(tokens[8].type, TokenType::TK_KEYWORD); // sync
    EXPECT_EQ(tokens[9].type, TokenType::TK_KEYWORD); // enum
    EXPECT_EQ(tokens[10].type, TokenType::TK_KEYWORD); // void
    EXPECT_EQ(tokens[11].type, TokenType::TK_KEYWORD); // volatile
    EXPECT_EQ(tokens[12].type, TokenType::TK_KEYWORD); // null
    EXPECT_EQ(tokens[13].type, TokenType::TK_KEYWORD); // import
    EXPECT_EQ(tokens[14].type, TokenType::TK_KEYWORD); // break
    EXPECT_EQ(tokens[15].type, TokenType::TK_KEYWORD); // continue
    EXPECT_EQ(tokens[16].type, TokenType::TK_KEYWORD); // match
    EXPECT_EQ(tokens[17].type, TokenType::__EOF); // EOF
    
    // Check for correct line and column numbers (assuming correct token positions)
    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);
}

// Test case for Lexer::keywordOrDatatypeOrIdentifier() method with data types
TEST_F(LexerTest, KeywordOrDatatypeOrIdentifierMethodTest_DataTypes) {
    // Initialize Lexer directly in the test case
    Lexer lexer("u8 u16 u32 u64 i8 i16 i32 i64 f32 f64 u128 i128 f80 f128 char bool", "file_name", print);
    auto tokens = lexer.lex();
    print.flush();
    
    ASSERT_EQ(tokens.size(), 17); // Ensure correct number of tokens
    
    EXPECT_EQ(tokens[0].type, TokenType::TK_DATATYPE); // u8
    EXPECT_EQ(tokens[1].type, TokenType::TK_DATATYPE); // u16
    EXPECT_EQ(tokens[2].type, TokenType::TK_DATATYPE); // u32
    EXPECT_EQ(tokens[3].type, TokenType::TK_DATATYPE); // u64
    EXPECT_EQ(tokens[4].type, TokenType::TK_DATATYPE); // i8
    EXPECT_EQ(tokens[5].type, TokenType::TK_DATATYPE); // i16
    EXPECT_EQ(tokens[6].type, TokenType::TK_DATATYPE); // i32
    EXPECT_EQ(tokens[7].type, TokenType::TK_DATATYPE); // i64
    EXPECT_EQ(tokens[8].type, TokenType::TK_DATATYPE); // f32
    EXPECT_EQ(tokens[9].type, TokenType::TK_DATATYPE); // f64
    EXPECT_EQ(tokens[10].type, TokenType::TK_DATATYPE); // u128
    EXPECT_EQ(tokens[11].type, TokenType::TK_DATATYPE); // i128
    EXPECT_EQ(tokens[12].type, TokenType::TK_DATATYPE); // f80
    EXPECT_EQ(tokens[13].type, TokenType::TK_DATATYPE); // f128
    EXPECT_EQ(tokens[14].type, TokenType::TK_DATATYPE); // char
    EXPECT_EQ(tokens[15].type, TokenType::TK_DATATYPE); // bool
    EXPECT_EQ(tokens[16].type, TokenType::__EOF); // EOF
    
    // Check for correct line and column numbers (assuming correct token positions)
    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);
}

// Test case for Lexer::keywordOrDatatypeOrIdentifier() method with architecture-specific types
TEST_F(LexerTest, KeywordOrDatatypeOrIdentifierMethodTest_ArchSpecificTypes) {
    // Initialize Lexer directly in the test case
    Lexer lexer("u128 i128 f80 f128", "file_name", print);
    auto tokens = lexer.lex();
    print.flush();
    
    ASSERT_EQ(tokens.size(), 5); // Ensure correct number of tokens
    
    EXPECT_EQ(tokens[0].type, TokenType::TK_DATATYPE); // u128
    EXPECT_EQ(tokens[1].type, TokenType::TK_DATATYPE); // i128
    EXPECT_EQ(tokens[2].type, TokenType::TK_DATATYPE); // f80
    EXPECT_EQ(tokens[3].type, TokenType::TK_DATATYPE); // f128
    EXPECT_EQ(tokens[4].type, TokenType::__EOF); // EOF
    
    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);
}

TEST_F(LexerTest, NumberMethodTest) {
    Lexer lexer("0xAE 0x7E 0xe7 0b01 0o100 0x7e.2A 1.05e+27 -100", "file_name", print);
    auto tokens = lexer.lex();
    print.flush();

    ASSERT_EQ(tokens.size(), 10);

    EXPECT_EQ(tokens[0].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[1].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[2].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[3].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[4].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[5].type, TokenType::TKL_FLOAT);
    EXPECT_EQ(tokens[6].type, TokenType::TKL_FLOAT);
    EXPECT_EQ(tokens[7].type, TokenType::TK_OPERATOR);
    EXPECT_EQ(tokens[8].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[9].type, TokenType::__EOF);

    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);
    
};
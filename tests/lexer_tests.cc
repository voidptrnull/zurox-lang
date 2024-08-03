#include <gtest/gtest.h>
#include <lexer.hh>
#include <random>
#include <definitions.hh>

// Fixture for Lexer tests
class LexerTest : public ::testing::Test
{
protected:
    PrintGlobalState print;
    Lexer lexerInstance;

    // Constructor initializes lexerInstance directly
    LexerTest() : lexerInstance("", "", print)
    {
        // Additional setup if needed
    }

    const std::string generateRandomNumber()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<unsigned long int> dist_int(0, (1ULL << 63));
        static std::uniform_real_distribution<> dist_float(0.0, 1.7976931348623157e+308);

        std::ostringstream oss;
        int choice = dist_int(gen) % 2;
        if (choice == 0)
        {
            oss << dist_int(gen);
        }
        else
        {
            oss << dist_float(gen);
        }
        return oss.str();
    }

    const std::string generateRandomIdentifier()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist_len(1, 30);
        static std::uniform_int_distribution<> dist_char(0, 25);

        int length = dist_len(gen);
        std::string identifier;
        identifier.reserve(length);

        for (int i = 0; i < length; ++i)
        {
            identifier += 'a' + dist_char(gen);
        }
        return identifier;
    }
};

// Test case for Lexer::keywordOrDatatypeOrIdentifier() method with keywords
TEST_F(LexerTest, KeywordOrDatatypeOrIdentifierMethodTest_Keywords)
{
    // Initialize Lexer directly in the test case
    Lexer lexer("if else ret true false ref deref struct sync enum void volatile null import break continue match", "file_name", print);

    auto tokens = lexer.lex();
    print.flush();

    ASSERT_EQ(tokens.size(), 18); // Ensure correct number of tokens

    EXPECT_EQ(tokens[0].type, TokenType::TK_KEYWORD);  // if
    EXPECT_EQ(tokens[1].type, TokenType::TK_KEYWORD);  // else
    EXPECT_EQ(tokens[2].type, TokenType::TK_KEYWORD);  // ret
    EXPECT_EQ(tokens[3].type, TokenType::TK_KEYWORD);  // true
    EXPECT_EQ(tokens[4].type, TokenType::TK_KEYWORD);  // false
    EXPECT_EQ(tokens[5].type, TokenType::TK_KEYWORD);  // ref
    EXPECT_EQ(tokens[6].type, TokenType::TK_KEYWORD);  // deref
    EXPECT_EQ(tokens[7].type, TokenType::TK_KEYWORD);  // struct
    EXPECT_EQ(tokens[8].type, TokenType::TK_KEYWORD);  // sync
    EXPECT_EQ(tokens[9].type, TokenType::TK_KEYWORD);  // enum
    EXPECT_EQ(tokens[10].type, TokenType::TK_KEYWORD); // void
    EXPECT_EQ(tokens[11].type, TokenType::TK_KEYWORD); // volatile
    EXPECT_EQ(tokens[12].type, TokenType::TK_KEYWORD); // null
    EXPECT_EQ(tokens[13].type, TokenType::TK_KEYWORD); // import
    EXPECT_EQ(tokens[14].type, TokenType::TK_KEYWORD); // break
    EXPECT_EQ(tokens[15].type, TokenType::TK_KEYWORD); // continue
    EXPECT_EQ(tokens[16].type, TokenType::TK_KEYWORD); // match
    EXPECT_EQ(tokens[17].type, TokenType::__EOF);      // EOF

    // Check for correct line and column numbers (assuming correct token positions)
    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);
}

TEST_F(LexerTest, KeywordOrDatatypeOrIdentifierMethodTest_Identifier)
{
    std::ostringstream oss;
    for (int i = 0; i < 10000000; ++i)
    {
        oss << generateRandomIdentifier() << " ";
    }
    std::string large_input = oss.str();

    Lexer lexer(large_input, "file_name", print);

    auto start_time = std::chrono::high_resolution_clock::now();
    auto tokens = lexer.lex();
    auto end_time = std::chrono::high_resolution_clock::now();

    print.flush();
    for (auto i = 0ULL; i < tokens.size() - 1; i++)
    {
        if (!find_dt(tokens[i].lexeme).has_value() && !find_keyword(tokens[i].lexeme).has_value())
        {
            ASSERT_EQ(tokens[i].type, TK_ID);
        }
    }
    ASSERT_EQ(tokens.back().type, TokenType::__EOF); // Ensure the last token is EOF

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    double input_size_bytes = static_cast<double>(large_input.length());
    double input_size_mb = input_size_bytes / (1024 * 1024); // Convert bytes to MB

    std::cout << "Lexer took " << duration << " ms to lex random identifiers.\n";
    std::cout << "Avg. characters/ms: " << (input_size_bytes / duration) << "\n"; // Corrected type for clarity
    std::cout << "Input size: " << input_size_mb << " MB\n";

    // Convert duration from milliseconds to seconds for throughput calculation
    double duration_seconds = duration / 1000.0;
    std::cout << "Throughput: " << (input_size_mb / duration_seconds) << " MB/s\n"; // MB/s
}

// Test case for Lexer::keywordOrDatatypeOrIdentifier() method with data types
TEST_F(LexerTest, KeywordOrDatatypeOrIdentifierMethodTest_DataTypes)
{
    // Initialize Lexer directly in the test case
    Lexer lexer("u8 u16 u32 u64 i8 i16 i32 i64 f32 f64 u128 i128 f80 f128 char bool", "file_name", print);
    auto tokens = lexer.lex();
    print.flush();

    ASSERT_EQ(tokens.size(), 17); // Ensure correct number of tokens

    EXPECT_EQ(tokens[0].type, TokenType::TK_DATATYPE);  // u8
    EXPECT_EQ(tokens[1].type, TokenType::TK_DATATYPE);  // u16
    EXPECT_EQ(tokens[2].type, TokenType::TK_DATATYPE);  // u32
    EXPECT_EQ(tokens[3].type, TokenType::TK_DATATYPE);  // u64
    EXPECT_EQ(tokens[4].type, TokenType::TK_DATATYPE);  // i8
    EXPECT_EQ(tokens[5].type, TokenType::TK_DATATYPE);  // i16
    EXPECT_EQ(tokens[6].type, TokenType::TK_DATATYPE);  // i32
    EXPECT_EQ(tokens[7].type, TokenType::TK_DATATYPE);  // i64
    EXPECT_EQ(tokens[8].type, TokenType::TK_DATATYPE);  // f32
    EXPECT_EQ(tokens[9].type, TokenType::TK_DATATYPE);  // f64
    EXPECT_EQ(tokens[10].type, TokenType::TK_DATATYPE); // u128
    EXPECT_EQ(tokens[11].type, TokenType::TK_DATATYPE); // i128
    EXPECT_EQ(tokens[12].type, TokenType::TK_DATATYPE); // f80
    EXPECT_EQ(tokens[13].type, TokenType::TK_DATATYPE); // f128
    EXPECT_EQ(tokens[14].type, TokenType::TK_DATATYPE); // char
    EXPECT_EQ(tokens[15].type, TokenType::TK_DATATYPE); // bool
    EXPECT_EQ(tokens[16].type, TokenType::__EOF);       // EOF

    // Check for correct line and column numbers (assuming correct token positions)
    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);
}

inline bool test_float(TokenType t) {
    return (t == TKL_FLOAT || t == TKL_INT);
}

TEST_F(LexerTest, NumberMethodTest)
{
    Lexer lexer("0xAE 0x7E 0xe7 0b01 0o100 23.000535 1.05e+27 -100 100", "file_name", print);
    auto tokens = lexer.lex();
    print.flush();

    ASSERT_EQ(tokens.size(), 11);

    EXPECT_EQ(tokens[0].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[1].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[2].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[3].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[4].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[5].type, TokenType::TKL_FLOAT);
    EXPECT_EQ(tokens[6].type, TokenType::TKL_FLOAT);
    EXPECT_EQ(tokens[7].type, TokenType::TK_OPERATOR);
    EXPECT_EQ(tokens[8].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[9].type, TokenType::TKL_INT);
    EXPECT_EQ(tokens[10].type, TokenType::__EOF);

    EXPECT_EQ(tokens[0].line, 1);
    EXPECT_EQ(tokens[0].col, 0);

    std::ostringstream oss;
    for (unsigned long int i = 0; i < 1ULL << 20; ++i)
    {
        oss << generateRandomNumber() << " ";
    }
    std::string large_input = oss.str();

    Lexer lexer2(large_input, "file_name", print);

    auto start_time = std::chrono::high_resolution_clock::now();
    tokens = lexer2.lex();
    auto end_time = std::chrono::high_resolution_clock::now();

    print.flush();

    for (auto i = 0ULL; i < tokens.size() - 1; i++)
    {
        if (!test_float(tokens[i].type)) {
            print.info(tokens[i - 1].lexeme + "\n");
            print.info(tokens[i].lexeme + "\n");
            print.info(tokens[i + 1].lexeme + "\n");
            print.flush();
        }
        ASSERT_TRUE(test_float(tokens[i].type));
    }

    ASSERT_EQ(tokens.back().type, TokenType::__EOF); // Ensure the last token is EOF

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    double input_size_bytes = static_cast<double>(large_input.length());
    double input_size_mb = input_size_bytes / (1024 * 1024); // Convert bytes to MB

    std::cout << "Lexer took " << duration << " ms to lex random numbers.\n";
    std::cout << "Avg. characters/ms: " << (input_size_bytes / duration) << "\n"; // Corrected type for clarity
    std::cout << "Input size: " << input_size_mb << " MB\n";

    // Convert duration from milliseconds to seconds for throughput calculation
    double duration_seconds = duration / 1000.0;
    std::cout << "Throughput: " << (input_size_mb / duration_seconds) << " MB/s\n"; // MB/s
};
#ifndef PARSER_HH
#define PARSER_HH

#include <vector>
#include <memory>
#include <string>
#include "token.hh"
#include "ast.hh"
#include "print.hh"

class Parser
{
public:
    Parser(const std::vector<Token> &tokens, const std::string &file, PrintGlobalState &print);

    std::shared_ptr<ProgramNode> parse();

private:
    const std::vector<Token> &tokens;
    const std::string &file;
    PrintGlobalState print;
    int_t index;

    Token current_token() const;
    Token next_token() const;
    void advance();
    Token match(TokenType expected_type);
    Token match(TokenType expected_type, const std::string &expected_lexeme);

    std::shared_ptr<DeclarationNode> parse_declaration();
    std::shared_ptr<FunctionDeclarationNode> parse_function_declaration();
    std::vector<std::shared_ptr<ParameterNode>> parse_parameters();
    std::shared_ptr<ParameterNode> parse_parameter();
    std::shared_ptr<TypeNode> parse_type();
    std::shared_ptr<BlockNode> parse_block();
    std::shared_ptr<StatementNode> parse_statement();
    std::shared_ptr<IfStatementNode> parse_if_statement();
    std::shared_ptr<LoopStatementNode> parse_loop_statement();
    std::shared_ptr<VarDeclarationNode> parse_var_declaration();
    std::shared_ptr<ExpressionStatementNode> parse_expression_statement();
    std::shared_ptr<MatchStatementNode> parse_match_statement();
    std::shared_ptr<CaseClauseNode> parse_case_clause();
    std::shared_ptr<BreakStatementNode> parse_break_statement();
    std::shared_ptr<ContinueStatementNode> parse_continue_statement();
    std::shared_ptr<EnumDeclarationNode> parse_enum_declaration();
    std::shared_ptr<StructDeclarationNode> parse_struct_declaration();

    std::shared_ptr<ExpressionNode> parse_expression();
    std::shared_ptr<ExpressionNode> parse_term();
    std::shared_ptr<ExpressionNode> parse_factor();
    std::shared_ptr<ExpressionNode> parse_unary_expr();
    std::shared_ptr<ExpressionNode> parse_primary();
    std::shared_ptr<LiteralNode> parse_literal();

    bool is_literal(TokenType type);
};

#endif // PARSER_HH
#include <parser.hh>
#include <functional>
#include <definitions.hh>
#include <ast.hh>
#include <typeinfo>

Parser::Parser(const std::vector<Token> &tokens, const std::string &file, PrintGlobalState &print)
    : tokens(tokens), file(file), index(0), print(print) {}

std::shared_ptr<ProgramNode> Parser::parse()
{
    auto program_node = std::make_shared<ProgramNode>(std::vector<std::shared_ptr<DeclarationNode>>{});
    while (current_token().type != TokenType::__EOF)
    {
        auto declaration = parse_declaration();
        if (declaration)
        {
            program_node->declarations.push_back(declaration);
        }
        else
        {
            // Handle error or skip to synchronize
            auto t = tokens[index];
            print.error("Unable to parse declaration.", t.line, t.col, file);
            advance();
        }
    }
    return program_node;
}

Token Parser::current_token() const
{
    if (index < tokens.size())
    {
        return tokens[index];
    }
    else
    {
        Token eof_token;
        eof_token.type = TokenType::__EOF;
        eof_token.lexeme = "";
        eof_token.line = 0;
        eof_token.col = 0;
        return eof_token;
    }
}

Token Parser::next_token() const
{
    if (index + 1 < tokens.size())
    {
        return tokens[index + 1];
    }
    else
    {
        Token eof_token;
        eof_token.type = TokenType::__EOF;
        eof_token.lexeme = "";
        eof_token.line = 0;
        eof_token.col = 0;
        return eof_token;
    }
}

void Parser::advance()
{
    index++;
}

std::shared_ptr<DeclarationNode> Parser::parse_declaration()
{
    switch (current_token().type)
    {
    case TokenType::TK_KEYWORD:
        if (current_token().lexeme == "fn")
        {
            return parse_function_declaration();
        }
        else if (current_token().lexeme == "enum")
        {
            return parse_enum_declaration();
        }
        else if (current_token().lexeme == "struct")
        {
            return parse_struct_declaration();
        }
        else
        {
            // Handle error or skip to synchronize
            auto t = tokens[index];
            print.error("Unable to parse declaration.", t.line, t.col, file);
            advance();
            return nullptr;
        }
    default:
        // Handle error or skip to synchronize
        auto t = tokens[index];
        print.error("Unable to parse declaration.", t.line, t.col, file);
        advance();
        return nullptr;
    }
}

std::shared_ptr<FunctionDeclarationNode> Parser::parse_function_declaration()
{
    match(TokenType::TK_KEYWORD, "fn");
    auto name = match(TokenType::TK_ID).lexeme;
    match(TokenType::TK_SEPARATOR, "(");
    auto parameters_list = parse_parameters();
    match(TokenType::TK_SEPARATOR, ")");
    std::shared_ptr<TypeNode> return_type = nullptr;
    if (current_token().type == TokenType::TK_OPERATOR && current_token().lexeme == "->")
    {
        advance(); // Consume '->'
        return_type = parse_type();
    }
    auto body = parse_block();
    return std::make_shared<FunctionDeclarationNode>(name, parameters_list, return_type, body);
}

std::vector<std::shared_ptr<ParameterNode>> Parser::parse_parameters()
{
    std::vector<std::shared_ptr<ParameterNode>> parameters;
    if (current_token().type != TokenType::TK_SEPARATOR || current_token().lexeme != ")")
    {
        parameters.push_back(parse_parameter());
        while (current_token().type == TokenType::TK_SEPARATOR && current_token().lexeme == ",")
        {
            advance(); // Consume ','
            parameters.push_back(parse_parameter());
        }
    }
    return parameters;
}

std::shared_ptr<ParameterNode> Parser::parse_parameter()
{
    auto type_node = parse_type();
    auto name = match(TokenType::TK_ID).lexeme;
    return std::make_shared<ParameterNode>(type_node, name);
}

std::shared_ptr<TypeNode> Parser::parse_type()
{
    switch (current_token().type)
    {
    case TokenType::TK_DATATYPE:
        return std::make_shared<TypeNode>(match(TokenType::TK_DATATYPE).lexeme);
    case TokenType::TK_KEYWORD:
        if (current_token().lexeme == "struct" || current_token().lexeme == "enum")
        {
            return std::make_shared<TypeNode>(current_token().lexeme);
        }
        // Handle other type cases
    default:
        // Handle error or skip to synchronize
        auto t = tokens[index];
        print.error("Unable to parse declaration.", t.line, t.col, file);
        advance();
        return nullptr;
    }
}

std::shared_ptr<StatementNode> Parser::parse_statement()
{
    switch (current_token().type)
    {
    case TokenType::TK_KEYWORD:
        if (current_token().lexeme == "if")
        {
            return parse_if_statement();
        }
        else if (current_token().lexeme == "loop")
        {
            return parse_loop_statement();
        }
        else if (current_token().lexeme == "match")
        {
            return parse_match_statement();
        }
        else if (current_token().lexeme == "break")
        {
            return parse_break_statement();
        }
        else if (current_token().lexeme == "continue")
        {
            return parse_continue_statement();
        }
        else
        {
            return parse_var_declaration();
        }
    case TokenType::TK_SEPARATOR:
        if (current_token().lexeme == "{")
        {
            return parse_block();
        }
        else
        {
            // Handle error or skip to synchronize
            auto t = tokens[index];
            print.error("Unable to parse declaration.", t.line, t.col, file);
            advance();
            return nullptr;
        }
    default:
        // Handle error or skip to synchronize
        auto t = tokens[index];
        print.error("Unable to parse declaration.", t.line, t.col, file);
        advance();
        return nullptr;
    }
}

std::shared_ptr<IfStatementNode> Parser::parse_if_statement()
{
    match(TokenType::TK_KEYWORD, "if");
    match(TokenType::TK_SEPARATOR, "(");
    auto condition = parse_expression();
    match(TokenType::TK_SEPARATOR, ")");
    auto then_block = parse_block();
    std::vector<std::shared_ptr<IfStatementNode>> elif_statements;

    while (current_token().type == TokenType::TK_KEYWORD && current_token().lexeme == "elif")
    {
        advance(); // Consume 'elif'
        match(TokenType::TK_SEPARATOR, "(");
        auto elif_condition = parse_expression();
        match(TokenType::TK_SEPARATOR, ")");
        auto elif_block = parse_block();
        elif_statements.push_back(std::make_shared<IfStatementNode>(elif_condition, elif_block, std::vector<std::shared_ptr<IfStatementNode>>{}, nullptr));
    }

    std::shared_ptr<BlockNode> else_block = nullptr;
    if (current_token().type == TokenType::TK_KEYWORD && current_token().lexeme == "else")
    {
        advance(); // Consume 'else'
        else_block = parse_block();
    }

    return std::make_shared<IfStatementNode>(condition, then_block, elif_statements, else_block);
}

std::shared_ptr<LoopStatementNode> Parser::parse_loop_statement()
{
    match(TokenType::TK_KEYWORD, "loop");
    auto body = parse_block();
    return std::make_shared<LoopStatementNode>(body);
}

std::shared_ptr<VarDeclarationNode> Parser::parse_var_declaration()
{
    auto type_node = parse_type();
    auto name = match(TokenType::TK_ID).lexeme;
    std::shared_ptr<ExpressionNode> initializer = nullptr;
    if (current_token().type == TokenType::TK_OPERATOR && current_token().lexeme == "=")
    {
        advance(); // Consume '='
        initializer = parse_expression();
    }
    match(TokenType::TK_SEPARATOR, ";");
    return std::make_shared<VarDeclarationNode>(type_node, name, initializer);
}

std::shared_ptr<ExpressionStatementNode> Parser::parse_expression_statement()
{
    auto expression = parse_expression();
    match(TokenType::TK_SEPARATOR, ";");
    return std::make_shared<ExpressionStatementNode>(expression);
}

std::shared_ptr<MatchStatementNode> Parser::parse_match_statement()
{
    match(TokenType::TK_KEYWORD, "match");
    match(TokenType::TK_SEPARATOR, "{");
    std::vector<std::shared_ptr<CaseClauseNode>> cases;
    while (current_token().type == TokenType::TK_ID || is_literal(current_token().type))
    {
        cases.push_back(parse_case_clause());
    }
    std::shared_ptr<BlockNode> default_block = nullptr;
    if (current_token().type == TokenType::TK_OPERATOR && current_token().lexeme == "_")
    {
        advance(); // Consume '_'
        match(TokenType::TK_SEPARATOR, ":");
        default_block = parse_block();
    }
    match(TokenType::TK_SEPARATOR, "}");
    return std::make_shared<MatchStatementNode>(cases, default_block);
}

std::shared_ptr<CaseClauseNode> Parser::parse_case_clause()
{
    auto literal_node = parse_literal();
    match(TokenType::TK_OPERATOR, ":");
    auto block_node = parse_block();
    return std::make_shared<CaseClauseNode>(literal_node, block_node);
}

std::shared_ptr<BreakStatementNode> Parser::parse_break_statement()
{
    match(TokenType::TK_KEYWORD, "break");
    match(TokenType::TK_SEPARATOR, ";");
    return std::make_shared<BreakStatementNode>();
}

std::shared_ptr<ContinueStatementNode> Parser::parse_continue_statement()
{
    match(TokenType::TK_KEYWORD, "continue");
    match(TokenType::TK_SEPARATOR, ";");
    return std::make_shared<ContinueStatementNode>();
}

std::shared_ptr<EnumDeclarationNode> Parser::parse_enum_declaration()
{
    match(TokenType::TK_KEYWORD, "enum");
    auto name = match(TokenType::TK_ID).lexeme;
    match(TokenType::TK_SEPARATOR, "{");
    std::vector<std::string> fields;
    while (current_token().type == TokenType::TK_ID)
    {
        fields.push_back(match(TokenType::TK_ID).lexeme);
        if (current_token().type == TokenType::TK_SEPARATOR && current_token().lexeme == ",")
        {
            advance(); // Consume ','
        }
    }
    match(TokenType::TK_SEPARATOR, "}");
    return std::make_shared<EnumDeclarationNode>(name, fields);
}

std::shared_ptr<StructDeclarationNode> Parser::parse_struct_declaration()
{
    match(TokenType::TK_KEYWORD, "struct");
    auto name = match(TokenType::TK_ID).lexeme;
    match(TokenType::TK_SEPARATOR, "{");
    std::vector<std::shared_ptr<ParameterNode>> fields;
    while (current_token().type == TokenType::TK_DATATYPE || current_token().type == TokenType::TK_ID)
    {
        fields.push_back(parse_parameter());
        if (current_token().type == TokenType::TK_SEPARATOR && current_token().lexeme == ",")
        {
            advance(); // Consume ','
        }
    }
    match(TokenType::TK_SEPARATOR, "}");
    return std::make_shared<StructDeclarationNode>(name, fields);
}

std::shared_ptr<ExpressionNode> Parser::parse_expression()
{
    return parse_term();
}

std::shared_ptr<ExpressionNode> Parser::parse_term()
{
    auto node = parse_factor();
    while (current_token().type == TokenType::TK_OPERATOR &&
           (current_token().lexeme == "+" || current_token().lexeme == "-"))
    {
        auto op = current_token().lexeme;
        advance(); // Consume operator
        auto right = parse_factor();
        node = std::make_shared<BinaryExprNode>(node, op, right);
    }
    return node;
}

std::shared_ptr<ExpressionNode> Parser::parse_factor()
{
    auto node = parse_unary_expr();
    while (current_token().type == TokenType::TK_OPERATOR &&
           (current_token().lexeme == "*" || current_token().lexeme == "/" || current_token().lexeme == "%"))
    {
        auto op = current_token().lexeme;
        advance(); // Consume operator
        auto right = parse_unary_expr();
        node = std::make_shared<BinaryExprNode>(node, op, right);
    }
    return node;
}

std::shared_ptr<ExpressionNode> Parser::parse_unary_expr()
{
    if (current_token().type == TokenType::TK_OPERATOR &&
        (current_token().lexeme == "+" || current_token().lexeme == "-" ||
         current_token().lexeme == "!" || current_token().lexeme == "~"))
    {
        auto op = current_token().lexeme;
        advance(); // Consume operator
        auto right = parse_unary_expr();
        return std::make_shared<UnaryExprNode>(op, right);
    }
    return parse_primary();
}

std::shared_ptr<ExpressionNode> Parser::parse_primary()
{
    switch (current_token().type)
    {
    case TokenType::TKL_INT:
    case TokenType::TKL_FLOAT:
    case TokenType::TKL_CHAR:
    case TokenType::TKL_STR:
        return parse_literal();
    case TokenType::TK_ID:
    {
        auto identifier = std::make_shared<IdentifierNode>(match(TokenType::TK_ID).lexeme);
        return std::static_pointer_cast<ExpressionNode>(identifier);
    }
    case TokenType::TK_SEPARATOR:
        if (current_token().lexeme == "(")
        {
            advance(); // Consume '('
            auto node = parse_expression();
            match(TokenType::TK_SEPARATOR, ")");
            return node;
        }
        // fall through
    default:
        // Handle error or throw exception
        auto t = tokens[index];
        print.error("Unable to parse declaration.", t.line, t.col, file);
        advance();
        return nullptr;
    }
}

std::shared_ptr<LiteralNode> Parser::parse_literal()
{
    switch (current_token().type)
    {
    case TokenType::TKL_INT:
        return std::make_shared<LiteralNode>(match(TokenType::TKL_INT).lexeme, "int");
    case TokenType::TKL_FLOAT:
        return std::make_shared<LiteralNode>(match(TokenType::TKL_FLOAT).lexeme, "float");
    case TokenType::TKL_CHAR:
        return std::make_shared<LiteralNode>(match(TokenType::TKL_CHAR).lexeme, "char");
    case TokenType::TKL_STR:
        return std::make_shared<LiteralNode>(match(TokenType::TKL_STR).lexeme, "string");
    default:
        // Handle error
        auto t = tokens[index];
        print.error("Unable to parse declaration.", t.line, t.col, file);
        advance();
        return nullptr;
    }
}

Token Parser::match(TokenType expected_type, const std::string &expected_lexeme)
{
    Token token = current_token();
    if (token.type != expected_type || (!expected_lexeme.empty() && token.lexeme != expected_lexeme))
    {
        // Handle error: Unexpected token
        auto t = tokens[index];
        print.error("Unable to parse declaration.", t.line, t.col, file);
    }
    advance(); // Consume the matched token
    return token;
}

Token Parser::match(TokenType expected_type) {
    Token t = current_token();
    if (t.type != expected_type) {
        print.error("Unable to parse declaration.", t.line, t.col, file);
    }
    return t;
}

bool Parser::is_literal(TokenType type)
{
    return type == TKL_CHAR || type == TKL_FLOAT || type == TKL_INT || type == TKL_STR;
}

std::shared_ptr<BlockNode> Parser::parse_block()
{
    match(TokenType::TK_SEPARATOR, "{");
    std::vector<std::shared_ptr<StatementNode>> statements;
    while (current_token().type != TokenType::TK_SEPARATOR || current_token().lexeme != "}")
    {
        auto statement = parse_statement();
        if (statement)
        {
            statements.push_back(statement);
        }
        else
        {
            auto t = tokens[index];
            print.error("Unable to parse declaration.", t.line, t.col, file);
            advance();
        }
    }
    match(TokenType::TK_SEPARATOR, "}");
    return std::make_shared<BlockNode>(statements);
}

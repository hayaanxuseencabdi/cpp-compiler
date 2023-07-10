#include <format>
#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "scanner.h"
#include "token.h"

namespace {

using frontend::Token;

void compare_tokens(const std::vector<Token>& parsed_tokens,
                    const std::vector<Token>& expected_tokens) {
    EXPECT_EQ(parsed_tokens.size(), expected_tokens.size());

    for (size_t index = 0; index < expected_tokens.size(); ++index) {
        EXPECT_EQ(parsed_tokens[index], expected_tokens[index]) << std::vformat(
            "{}'th token: parsed: {} vs. expected: {}",
            std::make_format_args(index, parsed_tokens[index].to_string(),
                                  expected_tokens[index].to_string()));
    }
}

TEST(ScannerTests, ScanTokens) {
    frontend::Scanner scanner("[]]]]!=[[{{{==[=");
    const std::vector<frontend::Token> expected_tokens{
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::RIGHT_BRACKET, "]"),
        Token(Token::Type::RIGHT_BRACKET, "]"),
        Token(Token::Type::RIGHT_BRACKET, "]"),
        Token(Token::Type::RIGHT_BRACKET, "]"),
        Token(Token::Type::BANG_EQUAL, "!="),
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::LEFT_BRACE, "{"),
        Token(Token::Type::LEFT_BRACE, "{"),
        Token(Token::Type::LEFT_BRACE, "{"),
        Token(Token::Type::EQUAL_EQUAL, "=="),
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::EQUAL, "=")};

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, ScanStrings) {
    frontend::Scanner scanner(R"(["test"])");
    const std::vector<frontend::Token> expected_tokens{
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::STRING, "test"),
        Token(Token::Type::RIGHT_BRACKET, "]"),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, ScanNumbers) {
    frontend::Scanner scanner("[542] [342.024]");
    const std::vector<frontend::Token> expected_tokens{
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::NUMBER, "542"),
        Token(Token::Type::RIGHT_BRACKET, "]"),

        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::NUMBER, "342.024"),
        Token(Token::Type::RIGHT_BRACKET, "]"),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, ScanIdentifier) {
    frontend::Scanner scanner("[542] point2 abc _ab");
    const std::vector<frontend::Token> expected_tokens{
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::NUMBER, "542"),
        Token(Token::Type::RIGHT_BRACKET, "]"),

        Token(Token::Type::IDENTIFIER, "point2"),
        Token(Token::Type::IDENTIFIER, "abc"),
        Token(Token::Type::IDENTIFIER, "_ab"),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, ScanKeywords) {
    frontend::Scanner scanner(R"(
        if (x <= 5) {
            return 2;
        }

        return false;

        class Animal {};
    )");
    const std::vector<frontend::Token> expected_tokens{
        Token(Token::Type::IF, "if"),
        Token(Token::Type::LEFT_PAREN, "("),
        Token(Token::Type::IDENTIFIER, "x"),
        Token(Token::Type::LESS_EQUAL, "<="),
        Token(Token::Type::NUMBER, "5"),
        Token(Token::Type::RIGHT_PAREN, ")"),
        Token(Token::Type::LEFT_BRACE, "{"),
        Token(Token::Type::RETURN, "return"),
        Token(Token::Type::NUMBER, "2"),
        Token(Token::Type::SEMICOLON, ";"),
        Token(Token::Type::RIGHT_BRACE, "}"),

        Token(Token::Type::RETURN, "return"),
        Token(Token::Type::FALSE, "false"),
        Token(Token::Type::SEMICOLON, ";"),

        Token(Token::Type::CLASS, "class"),
        Token(Token::Type::IDENTIFIER, "Animal"),
        Token(Token::Type::LEFT_BRACE, "{"),
        Token(Token::Type::RIGHT_BRACE, "}"),
        Token(Token::Type::SEMICOLON, ";"),

    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, HandleUnexpectedTokens) {
    frontend::Scanner scanner("_|_[|_= !=");
    const std::vector<frontend::Token> expected_tokens{
        Token(Token::Type::IDENTIFIER, "_"),
        Token(Token::Type::IDENTIFIER, "_"),
        Token(Token::Type::LEFT_BRACKET, "["),
        Token(Token::Type::IDENTIFIER, "_"),
        Token(Token::Type::EQUAL, "="),
        Token(Token::Type::BANG_EQUAL, "!="),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

} // namespace

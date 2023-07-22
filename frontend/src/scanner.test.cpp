#include <cstddef>
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

    for (std::size_t index = 0; index < expected_tokens.size(); ++index) {
        EXPECT_EQ(parsed_tokens[index], expected_tokens[index]) << std::vformat(
            "{}'th token: parsed: {} vs. expected: {}",
            std::make_format_args(index, parsed_tokens[index].to_string(),
                                  expected_tokens[index].to_string()));
    }
}

TEST(ScannerTests, Tokens) {
    frontend::Scanner scanner("[]<>]!=[[{{{==[=");
    const std::vector<frontend::Token> expected_tokens{
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::RIGHT_BRACKET),
        Token(1, Token::Type::LESS),
        Token(1, Token::Type::GREATER),
        Token(1, Token::Type::RIGHT_BRACKET),
        Token(1, Token::Type::BANG_EQUAL),
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::LEFT_BRACE),
        Token(1, Token::Type::LEFT_BRACE),
        Token(1, Token::Type::LEFT_BRACE),
        Token(1, Token::Type::EQUAL_EQUAL),
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::EQUAL),
        Token(1, Token::Type::END_OF_FILE),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, Strings) {
    frontend::Scanner scanner(R"(["test"])");
    const std::vector<frontend::Token> expected_tokens{
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::STRING, "test"),
        Token(1, Token::Type::RIGHT_BRACKET),
        Token(1, Token::Type::END_OF_FILE),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, Numbers) {
    frontend::Scanner scanner("[542] [342.024]");
    const std::vector<frontend::Token> expected_tokens{
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::NUMBER, "542"),
        Token(1, Token::Type::RIGHT_BRACKET),
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::NUMBER, "342.024"),
        Token(1, Token::Type::RIGHT_BRACKET),
        Token(1, Token::Type::END_OF_FILE),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, Identifier) {
    frontend::Scanner scanner("[542] point2 abc _ab");
    const std::vector<frontend::Token> expected_tokens{
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::NUMBER, "542"),
        Token(1, Token::Type::RIGHT_BRACKET),
        Token(1, Token::Type::IDENTIFIER, "point2"),
        Token(1, Token::Type::IDENTIFIER, "abc"),
        Token(1, Token::Type::IDENTIFIER, "_ab"),
        Token(1, Token::Type::END_OF_FILE),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, ReservedWords) {
    frontend::Scanner scanner(R"(
        if (x <= 5) {
            return (2 >> 2);
        }

        return false;

        class Animal {};
    )");
    const std::vector<frontend::Token> expected_tokens{
        Token(2, Token::Type::IF),
        Token(2, Token::Type::LEFT_PAREN),
        Token(2, Token::Type::IDENTIFIER, "x"),
        Token(2, Token::Type::LESS_EQUAL),
        Token(2, Token::Type::NUMBER, "5"),
        Token(2, Token::Type::RIGHT_PAREN),
        Token(2, Token::Type::LEFT_BRACE),
        Token(3, Token::Type::RETURN),
        Token(3, Token::Type::LEFT_PAREN),
        Token(3, Token::Type::NUMBER, "2"),
        Token(3, Token::Type::GREATER_GREATER),
        Token(3, Token::Type::NUMBER, "2"),
        Token(3, Token::Type::RIGHT_PAREN),
        Token(3, Token::Type::SEMICOLON),
        Token(4, Token::Type::RIGHT_BRACE),
        Token(6, Token::Type::RETURN),
        Token(6, Token::Type::FALSE),
        Token(6, Token::Type::SEMICOLON),
        Token(8, Token::Type::CLASS),
        Token(8, Token::Type::IDENTIFIER, "Animal"),
        Token(8, Token::Type::LEFT_BRACE),
        Token(8, Token::Type::RIGHT_BRACE),
        Token(8, Token::Type::SEMICOLON),
        Token(9, Token::Type::END_OF_FILE),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

TEST(ScannerTests, UnexpectedTokens) {
    frontend::Scanner scanner("_|_[|_= !=");
    const std::vector<frontend::Token> expected_tokens{
        Token(1, Token::Type::IDENTIFIER, "_"),
        Token(1, Token::Type::IDENTIFIER, "_"),
        Token(1, Token::Type::LEFT_BRACKET),
        Token(1, Token::Type::IDENTIFIER, "_"),
        Token(1, Token::Type::EQUAL),
        Token(1, Token::Type::BANG_EQUAL),
        Token(1, Token::Type::END_OF_FILE),
    };

    const auto parsed_tokens = scanner.scan_tokens();

    compare_tokens(parsed_tokens, expected_tokens);
}

} // namespace

#include <format>
#include <iostream>

#include "scanner.h"
#include "token.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

using frontend::Token;

TEST(ScannerTests, ScanTokens) {
  frontend::Scanner scanner("[]]]]!=[[{{{==[=");
  const std::vector<frontend::Token> expected_tokens{
      Token(Token::Type::LEFT_BRACKET, "["),
      Token(Token::Type::RIGHT_BRACKET, "]"),
      Token(Token::Type::RIGHT_BRACKET, "]"),
      Token(Token::Type::RIGHT_BRACKET, "]"),
      Token(Token::Type::RIGHT_BRACKET, "]"),
      Token(Token::Type::LOGICAL_NOT_EQUAL, "!="),
      Token(Token::Type::LEFT_BRACKET, "["),
      Token(Token::Type::LEFT_BRACKET, "["),
      Token(Token::Type::LEFT_BRACE, "{"),
      Token(Token::Type::LEFT_BRACE, "{"),
      Token(Token::Type::LEFT_BRACE, "{"),
      Token(Token::Type::LOGICAL_EQUAL, "=="),
      Token(Token::Type::LEFT_BRACKET, "["),
      Token(Token::Type::ASSIGN, "="),
  };

  const auto tokens = scanner.scan_tokens();

  EXPECT_THAT(tokens, ::testing::ContainerEq(expected_tokens));
}

} // namespace

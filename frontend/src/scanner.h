#pragma once

#include <string>
#include <utility>
#include <vector>

#include "token.h"

namespace frontend {

class Scanner {
public:
  Scanner(std::string source_code) : source_code_(std::move(source_code)) {}

  std::vector<Token> scan_tokens() {
    while (!is_at_end()) {
      start_ = current_;
      // FIXME: make scan_token optionally return a token rather than having a
      // data member to store them and share them across functions
      scan_token();
    }
    return tokens_;
  }

private:
  bool is_at_end() const { return current_ >= source_code_.length(); }

  char advance() { return source_code_.at(current_++); }

  bool match(char expected_character) {
    if (is_at_end()) {
      return false;
    }

    if (source_code_.at(current_) != expected_character) {
      return false;
    }

    ++current_;
    return true;
  }

  void scan_token() {
    const char c = advance();
    switch (c) {
    case '(': add_token(Token::Type::LEFT_PAREN); break;
    case ')': add_token(Token::Type::RIGHT_PAREN); break;
    case '{': add_token(Token::Type::LEFT_BRACE); break;
    case '}': add_token(Token::Type::RIGHT_BRACE); break;
    case '[': add_token(Token::Type::LEFT_BRACKET); break;
    case ']': add_token(Token::Type::RIGHT_BRACKET); break;
    default:
      // TODO: log warning/error?
      break;
    }
  }

  void add_token(Token::Type type) {
      std::string lexeme = source_code_.substr(start_, current_ - start_ + 1);
      tokens_.emplace_back(type, lexeme);
  }

  std::string source_code_;
  size_t start_ = 0;
  size_t current_ = 0;
  std::vector<Token> tokens_{};
};

} // namespace frontend

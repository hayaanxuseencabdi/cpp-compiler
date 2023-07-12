#pragma once

#include <optional>
#include <string>
#include <vector>

#include "token.h"

namespace frontend {

class Scanner {
public:
    Scanner(std::string source_code);
    std::vector<Token> scan_tokens();

private:
    static constexpr bool is_alpha(char c);
    static constexpr bool is_digit(char c);
    static constexpr bool is_alpha_numeric(char c);
    bool is_at_end() const;
    char advance();
    bool match(char expected_character);
    char peek() const;
    char peek_next() const;
    Token create_simple_token(Token::Type type);
    std::optional<Token> scan_string();
    std::optional<Token> scan_number();
    std::optional<Token> scan_identifier();
    std::optional<Token> scan_token();

    std::string source_code_;
    size_t line_ = 1;
    size_t start_ = 0;
    size_t current_ = 0;
};

} // namespace frontend

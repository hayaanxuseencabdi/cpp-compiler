#pragma once

#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "token.h"

namespace frontend {

class Scanner {
public:
    Scanner(std::string source_code) : source_code_(std::move(source_code)) {}

    std::vector<Token> scan_tokens() {
        std::vector<Token> tokens;
        while (!is_at_end()) {
            start_ = current_;
            auto scanned_token = scan_token();
            if (scanned_token.has_value()) {
                tokens.push_back(scanned_token.value());
            }
        }
        return tokens;
    }

private:
    static constexpr bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    static constexpr bool is_digit(char c) {
        return (c >= '0' && c <= '9');
    }

    static constexpr bool is_alpha_numeric(char c) {
        return is_alpha(c) || is_digit(c);
    }

    bool is_at_end() const {
        return current_ >= source_code_.length();
    }

    char advance() {
        return source_code_.at(current_++);
    }

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

    char peek() const {
        if (is_at_end()) {
            return '\0';
        }
        return source_code_.at(current_);
    }

    char peek_next() const {
        if (current_ + 1 >= source_code_.length()) {
            return '\0';
        }
        return source_code_.at(current_ + 1);
    }

    Token create_simple_token(Token::Type type) {
        std::string lexeme = source_code_.substr(start_, current_ - start_);
        return Token(type, lexeme);
    }

    std::optional<Token> scan_string() {
        while (!is_at_end() && peek() != '"') {
            if (peek() == '\n') {
                ++line_;
            }
            advance();
        }

        if (is_at_end()) {
            // TODO: error("unterminated string")
            return std::nullopt;
        }

        // handles the closing quotes
        advance();

        // +1 and -2 offsets to trim the surrounding quotes
        std::string lexeme =
            source_code_.substr(start_ + 1, current_ - start_ - 2);
        return Token(Token::Type::STRING, lexeme);
    }

    std::optional<Token> scan_number() {
        while (is_digit(peek())) {
            advance();
        }

        if (peek() == '.' && is_digit(peek_next())) {
            advance();
            while (is_digit(peek())) {
                advance();
            }
        }

        const auto lexeme = source_code_.substr(start_, current_ - start_);
        // TODO: convert to actual number?
        return Token(Token::Type::NUMBER, lexeme);
    }

    std::optional<Token> scan_identifier() {
        while (is_alpha_numeric(peek())) {
            advance();
        }

        const auto lexeme = source_code_.substr(start_, current_ - start_);
        return Token(Token::Type::IDENTIFIER, lexeme);
    }

    std::optional<Token> scan_token() {
        const char c = advance();
        switch (c) {
        case '(':
            return create_simple_token(Token::Type::LEFT_PAREN);
        case ')':
            return create_simple_token(Token::Type::RIGHT_PAREN);
        case '{':
            return create_simple_token(Token::Type::LEFT_BRACE);
        case '}':
            return create_simple_token(Token::Type::RIGHT_BRACE);
        case '[':
            return create_simple_token(Token::Type::LEFT_BRACKET);
        case ']':
            return create_simple_token(Token::Type::RIGHT_BRACKET);
        case '!':
            return create_simple_token(match('=') ? Token::Type::BANG_EQUAL
                                                  : Token::Type::BANG);
        case '=':
            return create_simple_token(match('=') ? Token::Type::EQUAL_EQUAL
                                                  : Token::Type::EQUAL);
        case '/':
            if (match('/')) {
                while (!is_at_end() && peek() != '\n') {
                    advance();
                }
            } else {
                return create_simple_token(Token::Type::SLASH);
            }
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            ++line_;
            break;
        case '"':
            return scan_string();
            break;
        default:
            if (is_digit(c)) {
                return scan_number();
            } else if (is_alpha(c)) {
                return scan_identifier();
            }
            // TODO: log warning/error
            std::cerr << std::vformat(
                "Failed to match the following the following character: {}\n",
                std::make_format_args(c));
            break;
        }
        return std::nullopt;
    }

    std::string source_code_;
    size_t line_ = 0;
    size_t start_ = 0;
    size_t current_ = 0;
};

} // namespace frontend

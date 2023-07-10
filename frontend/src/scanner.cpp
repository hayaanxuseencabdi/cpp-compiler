#include "scanner.h"

#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace frontend {

namespace {
const std::unordered_map<std::string_view, Token::Type> KEYWORDS{
    {"const", Token::Type::CONST}, {"and", Token::Type::AND},
    {"class", Token::Type::CLASS}, {"else", Token::Type::ELSE},
    {"false", Token::Type::FALSE}, {"fun", Token::Type::FUN},
    {"for", Token::Type::FOR},     {"if", Token::Type::IF},
    {"or", Token::Type::OR},       {"return", Token::Type::RETURN},
    {"this", Token::Type::THIS},   {"true", Token::Type::TRUE},
    {"while", Token::Type::WHILE},
};

}

Scanner::Scanner(std::string source_code)
    : source_code_(std::move(source_code)) {}

std::vector<Token> Scanner::scan_tokens() {
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

constexpr bool Scanner::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

constexpr bool Scanner::is_digit(char c) {
    return (c >= '0' && c <= '9');
}

constexpr bool Scanner::is_alpha_numeric(char c) {
    return is_alpha(c) || is_digit(c);
}

bool Scanner::is_at_end() const {
    return current_ >= source_code_.length();
}

char Scanner::advance() {
    return source_code_.at(current_++);
}

bool Scanner::match(char expected_character) {
    if (is_at_end()) {
        return false;
    }

    if (source_code_.at(current_) != expected_character) {
        return false;
    }

    ++current_;
    return true;
}

char Scanner::peek() const {
    if (is_at_end()) {
        return '\0';
    }
    return source_code_.at(current_);
}

char Scanner::peek_next() const {
    if (current_ + 1 >= source_code_.length()) {
        return '\0';
    }
    return source_code_.at(current_ + 1);
}

Token Scanner::create_simple_token(Token::Type type) {
    std::string lexeme = source_code_.substr(start_, current_ - start_);
    return Token(type, lexeme);
}

std::optional<Token> Scanner::scan_string() {
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
    std::string lexeme = source_code_.substr(start_ + 1, current_ - start_ - 2);
    return Token(Token::Type::STRING, lexeme);
}

std::optional<Token> Scanner::scan_number() {
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

std::optional<Token> Scanner::scan_identifier() {
    while (is_alpha_numeric(peek())) {
        advance();
    }

    const auto lexeme = source_code_.substr(start_, current_ - start_);

    const auto keyword_entry = KEYWORDS.find(lexeme);
    if (keyword_entry != KEYWORDS.end()) {
        return Token(keyword_entry->second, lexeme);
    }

    return Token(Token::Type::IDENTIFIER, lexeme);
}

std::optional<Token> Scanner::scan_token() {
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
        case ';':
            return create_simple_token(Token::Type::SEMICOLON);
        case '!':
            return create_simple_token(match('=') ? Token::Type::BANG_EQUAL
                                                  : Token::Type::BANG);
        case '=':
            return create_simple_token(match('=') ? Token::Type::EQUAL_EQUAL
                                                  : Token::Type::EQUAL);
        case '<':
            return create_simple_token(match('=') ? Token::Type::LESS_EQUAL
                                                  : Token::Type::LESS);
        case '>':
            return create_simple_token(match('=') ? Token::Type::GREATER_EQUAL
                                                  : Token::Type::GREATER);
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

} // namespace frontend

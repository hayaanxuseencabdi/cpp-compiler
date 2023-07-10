#pragma once

#include <format>
#include <string>

namespace frontend {

class Token {
public:
    enum class Type {
        // clang-format off

        // Single-character tokens.
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, ASTERISK,
        // One or two character tokens.
        BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
        // Literals.
        IDENTIFIER, STRING, NUMBER,
        // Keywords.
        CONST, AND, CLASS, ELSE, FALSE, FUN, FOR, IF, OR, RETURN, THIS, TRUE, WHILE,

        END_OF_FILE,

        // clang-format on
    };

    Token(Type type, std::optional<std::string> lexeme)
        : line_(0), type_(type), lexeme_(std::move(lexeme)) {}

    // FIXME: less/greater than semantics for tokens does not make sense
    constexpr auto operator<=>(const Token& other) const = default;

    std::string to_string() const {
        return std::vformat(
            "{}: {} ({})",
            std::make_format_args(line_, type_, lexeme_.value_or("NONE")));
    }

private:
    std::size_t line_;
    Type type_;
    std::optional<std::string> lexeme_;
    // TODO: optional literal
};

} // namespace frontend

namespace std {

template <> struct formatter<frontend::Token::Type> : formatter<string_view> {
    auto format(frontend::Token::Type type, format_context& ctx) const {
        string_view name = "UNDEFINED";
        switch (type) {
            case frontend::Token::Type::LEFT_PAREN:
                name = "LEFT_PAREN";
                break;
            case frontend::Token::Type::RIGHT_PAREN:
                name = "RIGHT_PAREN";
                break;
            case frontend::Token::Type::LEFT_BRACE:
                name = "LEFT_BRACE";
                break;
            case frontend::Token::Type::RIGHT_BRACE:
                name = "RIGHT_BRACE";
                break;
            case frontend::Token::Type::LEFT_BRACKET:
                name = "LEFT_BRACKET";
                break;
            case frontend::Token::Type::RIGHT_BRACKET:
                name = "RIGHT_BRACKET";
                break;
            case frontend::Token::Type::COMMA:
                name = "COMMA";
                break;
            case frontend::Token::Type::DOT:
                name = "DOT";
                break;
            case frontend::Token::Type::MINUS:
                name = "MINUS";
                break;
            case frontend::Token::Type::PLUS:
                name = "PLUS";
                break;
            case frontend::Token::Type::SEMICOLON:
                name = "SEMICOLON";
                break;
            case frontend::Token::Type::SLASH:
                name = "SLASH";
                break;
            case frontend::Token::Type::ASTERISK:
                name = "ASTERISK";
                break;
            case frontend::Token::Type::BANG:
                name = "BANG";
                break;
            case frontend::Token::Type::BANG_EQUAL:
                name = "BANG_EQUAL";
                break;
            case frontend::Token::Type::EQUAL:
                name = "EQUAL";
                break;
            case frontend::Token::Type::EQUAL_EQUAL:
                name = "EQUAL_EQUAL";
                break;
            case frontend::Token::Type::GREATER:
                name = "GREATER";
                break;
            case frontend::Token::Type::GREATER_EQUAL:
                name = "GREATER_EQUAL";
                break;
            case frontend::Token::Type::LESS:
                name = "LESS";
                break;
            case frontend::Token::Type::LESS_EQUAL:
                name = "LESS_EQUAL";
                break;
            case frontend::Token::Type::IDENTIFIER:
                name = "IDENTIFIER";
                break;
            case frontend::Token::Type::STRING:
                name = "STRING";
                break;
            case frontend::Token::Type::NUMBER:
                name = "NUMBER";
                break;
            case frontend::Token::Type::AND:
                name = "AND";
                break;
            case frontend::Token::Type::CLASS:
                name = "CLASS";
                break;
            case frontend::Token::Type::ELSE:
                name = "ELSE";
                break;
            case frontend::Token::Type::FALSE:
                name = "FALSE";
                break;
            case frontend::Token::Type::FUN:
                name = "FUN";
                break;
            case frontend::Token::Type::FOR:
                name = "FOR";
                break;
            case frontend::Token::Type::IF:
                name = "IF";
                break;
            case frontend::Token::Type::OR:
                name = "OR";
                break;
            case frontend::Token::Type::RETURN:
                name = "RETURN";
                break;
            case frontend::Token::Type::THIS:
                name = "THIS";
                break;
            case frontend::Token::Type::TRUE:
                name = "TRUE";
                break;
            case frontend::Token::Type::WHILE:
                name = "WHILE";
                break;
            case frontend::Token::Type::END_OF_FILE:
                name = "END_OF_FILE";
                break;
            default:
                break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

} // namespace std

#pragma once

#include <format>
#include <string>

namespace frontend {

class Token {
public:
  enum class Type {
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    STAR,
    LOGICAL_EQUAL,
    LOGICAL_NOT_EQUAL,
    LOGICAL_NOT,
    ASSIGN,


    // Keywords
    AND,
    ELSE,
    FALSE,
    FUNCTION,
    FOR,
    IF,
    OR,
    RETURN,
    THIS,
    TRUE,
    VAR,
    WHILE,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    END_OF_FILE
  };

  Token(Type type, std::optional<std::string> lexeme)
      : type_(type), lexeme_(lexeme), line_(0) {}

  constexpr auto operator<=>(const Token& other) const = default;

  std::string to_string() const {
    return std::vformat("{}: {} ({})", std::make_format_args(line_, type_, lexeme_.value_or("NONE")));
  }

private:
  Type type_;
  std::optional<std::string> lexeme_;
  // TODO: optional literal
  std::size_t line_;
};

} // namespace frontend

namespace std {

template <> struct formatter<frontend::Token::Type> : formatter<string_view> {
  auto format(frontend::Token::Type type, format_context &ctx) const {
    string_view name = "Unknown";
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
    case frontend::Token::Type::STAR:
      name = "STAR";
      break;
    case frontend::Token::Type::AND:
      name = "AND";
      break;
    case frontend::Token::Type::ELSE:
      name = "ELSE";
      break;
    case frontend::Token::Type::FALSE:
      name = "FALSE";
      break;
    case frontend::Token::Type::FUNCTION:
      name = "FUNCTION";
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
    case frontend::Token::Type::VAR:
      name = "VAR";
      break;
    case frontend::Token::Type::WHILE:
      name = "WHILE";
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

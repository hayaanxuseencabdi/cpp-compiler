#pragma once

#include <format>
#include <initializer_list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ast/ast.h"
#include "ast/node.h"
#include "ast/operator.h"
#include "token.h"

namespace frontend {

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {
        if (tokens_.empty()) {
            throw std::logic_error("Tokens were supplied");
        }
        if (tokens_.back().type_ != Token::Type::END_OF_FILE) {
            throw std::logic_error("No final EOF marker");
        }
    }

    ast::AbstractSyntaxTree parse() {
        auto block = std::make_unique<ast::Block>();
        while (!is_at_end()) {
            if (auto node = statement(); node != nullptr) {
                block->statements_.push_back(std::move(node));
            }
        }
        return ast::AbstractSyntaxTree(std::move(block));
    }

private:
    Token peek() const {
        return tokens_.at(current_);
    }

    Token previous() const {
        return tokens_.at(current_ - 1);
    }

    bool is_at_end() const {
        return peek().type_ == Token::Type::END_OF_FILE;
    }

    void advance() {
        if (!is_at_end()) {
            ++current_;
        }
    }

    bool check(Token::Type expected_type) {
        return peek().type_ == expected_type;
    }

    bool match(std::initializer_list<Token::Type> expected_types) {
        for (Token::Type expected_type : expected_types) {
            if (check(expected_type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    void consume(Token::Type expected_type) {
        if (peek().type_ != expected_type) {
            throw std::logic_error(std::vformat(
                "Expected {} but encountered {} instead",
                std::make_format_args(expected_type, peek().to_string())));
        }
        advance();
    }

    std::unique_ptr<ast::Statement> statement() {
        // selection-statement
        if (match({Token::Type::IF})) {
            consume(Token::Type::LEFT_PAREN);
            auto condition = expression();
            consume(Token::Type::RIGHT_PAREN);
            auto stmt = statement();
            return std::make_unique<ast::IfStatement>(std::move(condition),
                                                      std::move(stmt));
        }

        // TODO: compound statement
        if (match({Token::Type::LEFT_BRACE})) {
            while (!match({Token::Type::RIGHT_BRACE})) {
                // TODO: add statements to
                advance();
            }
            consume(Token::Type::RIGHT_BRACE);
            return nullptr;
        }

        return expression_statement();
    }

    std::unique_ptr<ast::Statement> expression_statement() {
        auto expr = expression();
        consume(Token::Type::SEMICOLON);
        return expr;
    }

    std::unique_ptr<ast::Expression> expression() {
        // TODO
        return equality_expression();
    }

    std::unique_ptr<ast::Expression> bitwise_or_expression() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ast::Expression> bitwise_xor_expression() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ast::Expression> bitwise_and_expression() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ast::Expression> equality_expression() {
        auto expr = relational_expression();
        while (match({Token::Type::EQUAL_EQUAL, Token::Type::BANG_EQUAL})) {
            auto op = [&]() {
                switch (previous().type_) {
                    case Token::Type::EQUAL_EQUAL:
                        return ast::Operator::Type::EQUAL_TO;
                    case Token::Type::BANG_EQUAL:
                        return ast::Operator::Type::NOT_EQUAL_TO;
                    default:
                        throw std::logic_error("should be unreachable");
                }
            }();
            auto rhs = relational_expression();
            expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op,
                                                           std::move(rhs));
        }
        return expr;
    }

    std::unique_ptr<ast::Expression> relational_expression() {
        auto expr = shift_expression();
        while (match({Token::Type::LESS, Token::Type::LESS_EQUAL,
                      Token::Type::GREATER, Token::Type::GREATER_EQUAL})) {
            auto op = [&]() {
                switch (previous().type_) {
                    case Token::Type::LESS:
                        return ast::Operator::Type::LESS_THAN;
                    case Token::Type::LESS_EQUAL:
                        return ast::Operator::Type::LESS_THAN_OR_EQUAL_TO;
                    case Token::Type::GREATER:
                        return ast::Operator::Type::GREATER_THAN;
                    case Token::Type::GREATER_EQUAL:
                        return ast::Operator::Type::GREATER_THAN_OR_EQUAL_TO;
                    default:
                        throw std::logic_error("should be unreachable");
                }
            }();
            auto rhs = shift_expression();
            expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op,
                                                           std::move(rhs));
        }
        return expr;
    }

    std::unique_ptr<ast::Expression> shift_expression() {
        auto expr = additive_expression();
        while (match({Token::Type::GREATER_GREATER, Token::Type::LESS_LESS})) {
            auto op = previous().type_ == Token::Type::GREATER_GREATER
                          ? ast::Operator::Type::BITWISE_RIGHT_SHIFT
                          : ast::Operator::Type::BITWISE_LEFT_SHIFT;
            auto rhs = additive_expression();
            expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op,
                                                           std::move(rhs));
        }
        return expr;
    }

    std::unique_ptr<ast::Expression> additive_expression() {
        auto expr = multiplicative_expression();
        while (match({Token::Type::PLUS, Token::Type::MINUS})) {
            auto op = previous().type_ == Token::Type::PLUS
                          ? ast::Operator::Type::ADDITION
                          : ast::Operator::Type::SUBTRACTION;
            auto rhs = multiplicative_expression();
            expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op,
                                                           std::move(rhs));
        }
        return expr;
    }

    std::unique_ptr<ast::Expression> multiplicative_expression() {
        auto expr = primary_expression();
        while (match(
            {Token::Type::STAR, Token::Type::SLASH, Token::Type::PERCENT})) {
            auto op = [&]() {
                switch (previous().type_) {
                    case Token::Type::STAR:
                        return ast::Operator::Type::MULTIPLICATION;
                    case Token::Type::SLASH:
                        return ast::Operator::Type::DIVISION;
                    case Token::Type::PERCENT:
                        return ast::Operator::Type::REMAINDER;
                    default:
                        throw std::logic_error("should be unreachable");
                }
            }();
            auto rhs = primary_expression();
            expr = std::make_unique<ast::BinaryExpression>(std::move(expr), op,
                                                           std::move(rhs));
        }
        return expr;
    }

    std::unique_ptr<ast::Expression> primary_expression() {
        using Bool = ast::Literal<bool>;
        using String = ast::Literal<std::string>;

        if (match({Token::Type::TRUE})) {
            return std::make_unique<Bool>(true);
        }
        if (match({Token::Type::FALSE})) {
            return std::make_unique<Bool>(false);
        }

        if (match({Token::Type::STRING, Token::Type::NUMBER})) {
            if (auto token = previous(); token.lexeme_.has_value()) {
                return std::make_unique<String>(token.lexeme_.value());
            } else {
                // TODO: error
            }
        }

        if (match({Token::Type::LEFT_PAREN})) {
            auto inner_expression = expression();
            if (inner_expression == nullptr) {
                // TODO: error
            }
            consume(Token::Type::RIGHT_PAREN);
            return inner_expression;
        }

        return nullptr;
    }

    std::size_t current_ = 0;
    std::vector<Token> tokens_;
    // TODO: might need a symbol table? or hold it in the AST instead
};

} // namespace frontend

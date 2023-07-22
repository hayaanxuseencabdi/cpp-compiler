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
            if (auto node = additive_expression(); node.has_value()) {
                block->statements_.push_back(std::move(node.value()));
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

    Token advance() {
        if (!is_at_end()) {
            ++current_;
        }
        return previous();
    }

    bool check(Token::Type expected_type) {
        if (is_at_end()) {
            return false;
        }
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

    Token consume(Token::Type expected_type) {
        if (check(expected_type)) {
            return advance();
        }
        throw std::logic_error(
            std::vformat("Expected {} but encountered {} instead",
                         std::make_format_args(expected_type, peek().type_)));
    }

    std::optional<std::unique_ptr<ast::Expression>> expression() {
        return std::nullopt;
    }

    std::optional<std::unique_ptr<ast::Expression>> assignment_expression() {
        return std::nullopt;
    }

    std::optional<std::unique_ptr<ast::Expression>> shift_expression() {
        auto expr = additive_expression();
        while (match({Token::Type::GREATER_GREATER, Token::Type::LESS_LESS})) {
            auto op = previous().type_ == Token::Type::GREATER_GREATER
                          ? ast::Operator::Type::BITWISE_RIGHT_SHIFT
                          : ast::Operator::Type::BITWISE_LEFT_SHIFT;
            auto rhs = additive_expression();
            expr = std::make_unique<ast::BinaryExpression>(
                std::move(expr.value()), op, std::move(rhs.value()));
        }
        return expr;
    }

    std::optional<std::unique_ptr<ast::Expression>> additive_expression() {
        auto expr = multiplicative_expression();
        while (match({Token::Type::PLUS, Token::Type::MINUS})) {
            auto op = previous().type_ == Token::Type::PLUS
                          ? ast::Operator::Type::ADDITION
                          : ast::Operator::Type::SUBTRACTION;
            auto rhs = multiplicative_expression();
            expr = std::make_unique<ast::BinaryExpression>(
                std::move(expr.value()), op, std::move(rhs.value()));
        }
        return expr;
    }

    std::optional<std::unique_ptr<ast::Expression>>
    multiplicative_expression() {
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
            expr = std::make_unique<ast::BinaryExpression>(
                std::move(expr.value()), op, std::move(rhs.value()));
        }
        return expr;
    }

    std::optional<std::unique_ptr<ast::Expression>> primary_expression() {
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
            if (!inner_expression.has_value()) {
                // TODO: error
            }
            consume(Token::Type::RIGHT_PAREN);
            return inner_expression;
        }

        return std::nullopt;
    }

    std::size_t current_ = 0;
    std::vector<Token> tokens_;
    // TODO: might need a symbol table? or hold it in the AST instead
};

} // namespace frontend

#pragma once

#include <format>
#include <initializer_list>
#include <memory>
#include <optional>
#include <vector>

#include "ast/ast.h"
#include "ast/node.h"
#include "token.h"

namespace frontend {

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

    ast::AbstractSyntaxTree parse() {
        auto block = std::make_unique<ast::Block>();
        while (!is_at_end()) {
            auto node = primary();
            if (node.has_value()) {
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

    std::optional<std::unique_ptr<ast::Expression>> primary() {
        using Bool = ast::Literal<bool>;
        using String = ast::Literal<std::string>;

        if (match({Token::Type::TRUE})) {
            return std::make_unique<Bool>(true);
        }
        if (match({Token::Type::FALSE})) {
            return std::make_unique<Bool>(false);
        }

        if (match({Token::Type::STRING, Token::Type::NUMBER})) {
            auto token = previous();
            if (!token.lexeme_.has_value()) {
                // TODO: error
            }
            return std::make_unique<String>(token.lexeme_.value());
        }

        if (match({Token::Type::LEFT_PAREN})) {
            // TODO:
        }

        return std::nullopt;
    }

    std::size_t current_ = 0;
    std::vector<Token> tokens_;
    // TODO: might need a symbol table? or hold it in the AST instead
};

} // namespace frontend

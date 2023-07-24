#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <gsl/pointers>

#include "ast/operator.h"

namespace frontend::ast {

class Node {
public:
    virtual std::string to_string() const = 0;
    virtual ~Node() = default;
};

class Expression : public Node {};

template <typename T>
struct Literal final : public Expression {
public:
    constexpr Literal(T value) : value_(value) {}

    std::string to_string() const override {
        return std::vformat("Literal(value: {})",
                            std::make_format_args(value_));
    }

    T value_;
};

class UnaryExpression final : public Expression {
public:
    UnaryExpression(Operator::Type op, std::unique_ptr<Expression> operand)
        : operator_(op), operand_(gsl::make_not_null(std::move(operand))) {}

    std::string to_string() const override {
        return std::vformat(
            "UnaryExpression(operator: {}, operand: {})",
            std::make_format_args(operator_, operand_->to_string()));
    }

private:
    Operator::Type operator_;
    gsl::not_null<std::unique_ptr<Expression>> operand_;
};

class BinaryExpression final : public Expression {
public:
    BinaryExpression(std::unique_ptr<Expression> left, Operator::Type op,
                     std::unique_ptr<Expression> right)
        : left_(gsl::make_not_null(std::move(left))), operator_(op),
          right_(gsl::make_not_null(std::move(right))) {}

    std::string to_string() const override {
        return std::vformat(
            "BinaryExpression(left: {}, operation: {}, right: {})",
            std::make_format_args(left_->to_string(), operator_,
                                  right_->to_string()));
    }

private:
    gsl::not_null<std::unique_ptr<Expression>> left_;
    Operator::Type operator_;
    gsl::not_null<std::unique_ptr<Expression>> right_;
};

struct Statement : public Node {};

struct ExpressionStatement final : public Statement {
    ExpressionStatement(std::unique_ptr<Expression> expression)
        : expression_(std::move(expression)) {}

    std::string to_string() const override {
        return std::vformat("ExpressionStatement(expression: {})",
                            std::make_format_args(expression_.get()));
    }

    std::unique_ptr<Expression> expression_;
};

class CompoundStatement final : public Statement {
public:
    CompoundStatement(std::vector<std::unique_ptr<Statement>>&& statements)
        : statements_(std::move(statements)) {}

    std::string to_string() const override {
        std::string str;
        if (!statements_.empty()) {
            str = std::vformat(
                "{}", std::make_format_args(statements_.front()->to_string()));
            for (std::size_t i = 1; i < statements_.size(); ++i) {
                str += std::vformat(
                    ", {}", std::make_format_args(statements_[i]->to_string()));
            }
        }

        return std::vformat("CompoundStatement(statements: [{}])",
                            std::make_format_args(str));
    }

    // TODO: should be wrapped within gsl::not_null
    std::vector<std::unique_ptr<Statement>> statements_;
};

class IfStatement final : public Statement {
public:
    IfStatement(std::unique_ptr<Expression> condition,
                std::unique_ptr<Statement> then,
                std::unique_ptr<Statement> else_stmt)
        : condition_(gsl::make_not_null(std::move(condition))),
          then_(gsl::make_not_null(std::move(then))),
          else_(std::move(else_stmt)) {}

    std::string to_string() const override {
        return std::vformat("IfStatement(condition: {}, then: {}, else: {})",
                            std::make_format_args(condition_->to_string(),
                                                  then_->to_string(),
                                                  else_.get()));
    }

private:
    gsl::not_null<std::unique_ptr<Expression>> condition_;
    gsl::not_null<std::unique_ptr<Statement>> then_;
    std::unique_ptr<Statement> else_;
};

} // namespace frontend::ast

template <>
struct std::formatter<frontend::ast::Node*> : std::formatter<std::string> {
    auto format(frontend::ast::Node* node, std::format_context& ctx) const {
        return std::formatter<std::string>::format(
            node ? node->to_string() : "None", ctx);
    }
};

template <>
struct std::formatter<frontend::ast::Expression*>
    : std::formatter<frontend::ast::Node*> {
    auto format(frontend::ast::Expression* expr,
                std::format_context& ctx) const {
        return std::formatter<frontend::ast::Node*>::format(expr, ctx);
    }
};

template <>
struct std::formatter<frontend::ast::Statement*>
    : std::formatter<frontend::ast::Node*> {
    auto format(frontend::ast::Statement* stmt,
                std::format_context& ctx) const {
        return std::formatter<frontend::ast::Node*>::format(stmt, ctx);
    }
};

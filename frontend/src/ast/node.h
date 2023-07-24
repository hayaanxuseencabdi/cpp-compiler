#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <gsl/pointers>

#include "ast/operator.h"

namespace frontend::ast {

template <typename Node, typename... Args>
constexpr gsl::not_null<std::unique_ptr<Node>> create_node(Args&&... args) {
    return gsl::not_null(std::make_unique<Node>(std::forward<Args>(args)...));
}

class Node {
public:
    virtual std::string to_string() const = 0;
    virtual ~Node() = default;
};

class Statement : public Node {};
// FIXME: unsure if an ExpressionStatement node is needed or wise
class Expression : public Statement {};

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

class IfStatement final : public Statement {
public:
    IfStatement(std::unique_ptr<Expression> condition,
                std::unique_ptr<Statement> then,
                std::unique_ptr<Statement> else_stmt)
        : condition_(gsl::make_not_null(std::move(condition))),
          then_(gsl::make_not_null(std::move(then))),
          else_(std::move(else_stmt)) {}

    std::string to_string() const override {
        return std::vformat(
            "IfStatement(condition: {}, then: {}, else: {})",
            std::make_format_args(condition_->to_string(), then_->to_string(),
                                  else_ != nullptr ? else_->to_string()
                                                   : "None"));
    }

private:
    gsl::not_null<std::unique_ptr<Expression>> condition_;
    // TODO: should have a Statement node instead
    gsl::not_null<std::unique_ptr<Statement>> then_;
    std::unique_ptr<Statement> else_;
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

} // namespace frontend::ast

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

class Block final : public Node {
public:
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

        return std::vformat("Block(statements: [{}])",
                            std::make_format_args(str));
    }

    // TODO: should be wrapped within gsl::not_null
    std::vector<std::unique_ptr<Node>> statements_{};
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

class UnaryOperation final : public Expression {
public:
    UnaryOperation(Operator::Type op, std::unique_ptr<Expression> operand)
        : operator_(op), operand_(gsl::make_not_null(std::move(operand))) {}

    std::string to_string() const override {
        return std::vformat(
            "UnaryOperation(operator: {}, operand: {})",
            std::make_format_args(operator_, operand_->to_string()));
    }

private:
    Operator::Type operator_;
    gsl::not_null<std::unique_ptr<Expression>> operand_;
};

class BinaryOperation final : public Expression {
public:
    BinaryOperation(std::unique_ptr<Expression> left, Operator::Type op,
                    std::unique_ptr<Expression> right)
        : left_(gsl::make_not_null(std::move(left))), operator_(op),
          right_(gsl::make_not_null(std::move(right))) {}

    std::string to_string() const override {
        return std::vformat(
            "BinaryOperation(left: {}, operation: {}, right: {})",
            std::make_format_args(left_->to_string(), operator_,
                                  right_->to_string()));
    }

private:
    gsl::not_null<std::unique_ptr<Expression>> left_;
    Operator::Type operator_;
    gsl::not_null<std::unique_ptr<Expression>> right_;
};

} // namespace frontend::ast

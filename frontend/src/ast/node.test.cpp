#include <memory>

#include <gsl/pointers>
#include <gtest/gtest.h>

#include "ast/node.h"
#include "ast/operator.h"

namespace {

using Double = frontend::ast::Literal<double>;

TEST(Node, Literal) {
    auto num = frontend::ast::create_node<Double>(3.14);

    FAIL() << num->to_string();
}

TEST(Node, UnaryOperation) {
    auto op = frontend::ast::Operator::Type::ADDITION;
    auto operand = std::make_unique<Double>(3.15);

    auto unary_op = frontend::ast::create_node<frontend::ast::UnaryOperation>(
        op, std::move(operand));

    FAIL() << unary_op->to_string();
}

TEST(Node, BinaryOperation) {
    auto left = std::make_unique<Double>(3.14);
    auto op = frontend::ast::Operator::Type::ADDITION;
    auto right = std::make_unique<Double>(3.15);

    auto bin_op = frontend::ast::create_node<frontend::ast::BinaryOperation>(
        std::move(left), op, std::move(right));

    FAIL() << bin_op->to_string();
}

} // namespace

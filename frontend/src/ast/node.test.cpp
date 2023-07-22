#include <memory>

#include <gsl/pointers>
#include <gtest/gtest.h>

#include "ast/node.h"
#include "ast/operator.h"

namespace {

using Double = frontend::ast::Literal<double>;

TEST(Node, Literal) {
    auto num = frontend::ast::create_node<Double>(3.14);

    EXPECT_DOUBLE_EQ(num->value_, 3.14);
}

TEST(Node, UnaryOperation) {
    auto op = frontend::ast::Operator::Type::UNARY_PLUS;
    auto operand = std::make_unique<Double>(3.15);

    auto unary_expr =
        frontend::ast::create_node<frontend::ast::UnaryExpression>(
            op, std::move(operand));

    EXPECT_STREQ(
        unary_expr->to_string().c_str(),
        "UnaryOperation(operator: UNARY_PLUS, operand: Literal(value: 3.15))");
}

TEST(Node, BinaryOperation) {
    auto left = std::make_unique<Double>(3.14);
    auto op = frontend::ast::Operator::Type::ADDITION;
    auto right = std::make_unique<Double>(3.15);

    auto bin_expr = frontend::ast::create_node<frontend::ast::BinaryExpression>(
        std::move(left), op, std::move(right));

    EXPECT_STREQ(bin_expr->to_string().c_str(),
                 "BinaryOperation(left: Literal(value: 3.14), operation: "
                 "ADDITION, right: Literal(value: 3.15))");
}

} // namespace

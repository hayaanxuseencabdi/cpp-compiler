#include <memory>

#include <gsl/pointers>
#include <gtest/gtest.h>

#include "ast/node.h"
#include "token.h"

namespace {

using Double = frontend::ast::Literal<double>;

TEST(Node, Literals) {
    auto num = frontend::ast::create_node<Double>(3.14);

    FAIL() << num->to_string();
}

TEST(Node, BinaryOperations) {
    auto left = std::make_unique<Double>(3.14);
    auto op = frontend::Token::Type::PLUS;
    auto right = std::make_unique<Double>(3.15);

    auto bin_op = frontend::ast::create_node<frontend::ast::BinaryOperation>(
        std::move(left), op, std::move(right));

    FAIL() << bin_op->to_string();
}

} // namespace

#pragma once

#include <gsl/pointers>

#include "token.h"

namespace frontend::ast {

class Node {};

class Expression : Node {
public:
    void evaluate() {}

protected:
private:
};

class UnaryOperation final : Expression {
public:
private:
    Token operator_;
    gsl::not_null<Expression*> operand_;
};

class BinaryOperation final : Expression {
public:
    void evaluate() {
        left_->evaluate();
        right_->evaluate();

        // TODO
    }

private:
    gsl::not_null<Expression*> left_;
    Token operator_;
    gsl::not_null<Expression*> right_;
};

} // namespace frontend::ast

#include "parser.h"
#include "scanner.h"

#include <format>
#include <gtest/gtest.h>

namespace {

TEST(Parser, ParseNumbers) {
    frontend::Scanner scanner("43.3242; 4.395; 986.345;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: Block(statements: ["
            "Literal(value: 43.3242), "
            "Literal(value: 4.395), "
            "Literal(value: 986.345)"
        "]))"
            // clang-format off
            );
}

TEST(Parser, ParseMultiplicativeAndAdditiveExpressions) {
    frontend::Scanner scanner("4 % 3 + 5 * 2;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: Block(statements: ["
            "BinaryOperation("
                "left: BinaryOperation("
                    "left: Literal(value: 4), "
                    "operation: REMAINDER, "
                    "right: Literal(value: 3)"
                "), "
                "operation: ADDITION, "
                "right: BinaryOperation("
                    "left: Literal(value: 5), "
                    "operation: MULTIPLICATION, "
                    "right: Literal(value: 2)"
        "))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseShiftAndAdditiveExpressions) {
    frontend::Scanner scanner("4 << 3 + 5;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: Block(statements: ["
            "BinaryOperation("
                "left: Literal(value: 4), "
                "operation: BITWISE_LEFT_SHIFT, "
                "right: BinaryOperation("
                    "left: Literal(value: 3), "
                    "operation: ADDITION, "
                    "right: Literal(value: 5)"
        "))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseRelationalAndAdditiveExpressions) {
    frontend::Scanner scanner("4 - 4 < 3 + 5;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: Block(statements: ["
            "BinaryOperation("
                "left: BinaryOperation("
                    "left: Literal(value: 4), "
                    "operation: SUBTRACTION, "
                    "right: Literal(value: 4)"
                "), "
                "operation: LESS_THAN, "
                "right: BinaryOperation("
                    "left: Literal(value: 3), "
                    "operation: ADDITION, "
                    "right: Literal(value: 5)"
        "))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseEqualityAndAdditiveExpressions) {
    frontend::Scanner scanner("4 - 1 == 3 + 1;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: Block(statements: ["
            "BinaryOperation("
                "left: BinaryOperation("
                    "left: Literal(value: 4), "
                    "operation: SUBTRACTION, "
                    "right: Literal(value: 1)"
                "), "
                "operation: EQUAL_TO, "
                "right: BinaryOperation("
                    "left: Literal(value: 3), "
                    "operation: ADDITION, "
                    "right: Literal(value: 1)"
        "))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseEmptyExpressionStatement) {
    frontend::Scanner scanner(";;34;;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 "AST(root: Block(statements: [Literal(value: 34)]))");
}

TEST(Parser, ParseSimpleIfStatement) {
    frontend::Scanner scanner(R"(if (2 <= 5) 3;)");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
         "AST(root: Block(statements: ["
            "IfStatement("
                "condition: BinaryOperation("
                    "left: Literal(value: 2), "
                    "operation: LESS_THAN_OR_EQUAL_TO, "
                    "right: Literal(value: 5)), "
                    "statement: Literal(value: 3)"
         ")]))"
                 // clang-format on
    );
}

} // namespace

#include <format>

#include <gtest/gtest.h>

#include "parser.h"
#include "scanner.h"

namespace {

TEST(Parser, ParseNumbers) {
    frontend::Scanner scanner("43.3242; 4.395; 986.345;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: CompoundStatement(statements: ["
            "ExpressionStatement(expression: Literal(value: 43.3242)), "
            "ExpressionStatement(expression: Literal(value: 4.395)), "
            "ExpressionStatement(expression: Literal(value: 986.345))]))"
            // clang-format off
            );
}

TEST(Parser, ParseMultiplicativeAndAdditiveExpressions) {
    frontend::Scanner scanner("4 % 3 + 5 * 2;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: CompoundStatement(statements: ["
            "ExpressionStatement(expression: BinaryExpression("
                "left: BinaryExpression("
                    "left: Literal(value: 4), "
                    "operation: REMAINDER, "
                    "right: Literal(value: 3)"
                "), "
                "operation: ADDITION, "
                "right: BinaryExpression("
                    "left: Literal(value: 5), "
                    "operation: MULTIPLICATION, "
                    "right: Literal(value: 2))))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseShiftAndAdditiveExpressions) {
    frontend::Scanner scanner("4 << 3 + 5;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: CompoundStatement(statements: ["
            "ExpressionStatement(expression: BinaryExpression("
                "left: Literal(value: 4), "
                "operation: BITWISE_LEFT_SHIFT, "
                "right: BinaryExpression("
                    "left: Literal(value: 3), "
                    "operation: ADDITION, "
                    "right: Literal(value: 5))))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseRelationalAndAdditiveExpressions) {
    frontend::Scanner scanner("4 - 4 < 3 + 5;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: CompoundStatement(statements: ["
            "ExpressionStatement(expression: BinaryExpression("
                "left: BinaryExpression("
                    "left: Literal(value: 4), "
                    "operation: SUBTRACTION, "
                    "right: Literal(value: 4)"
                "), "
                "operation: LESS_THAN, "
                "right: BinaryExpression("
                    "left: Literal(value: 3), "
                    "operation: ADDITION, "
                    "right: Literal(value: 5))))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseEqualityAndAdditiveExpressions) {
    frontend::Scanner scanner("4 - 1 == 3 + 1;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: CompoundStatement(statements: ["
            "ExpressionStatement(expression: BinaryExpression("
                "left: BinaryExpression("
                    "left: Literal(value: 4), "
                    "operation: SUBTRACTION, "
                    "right: Literal(value: 1)"
                "), "
                "operation: EQUAL_TO, "
                "right: BinaryExpression("
                    "left: Literal(value: 3), "
                    "operation: ADDITION, "
                    "right: Literal(value: 1))))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseEmptyExpressionStatement) {
    frontend::Scanner scanner(";;34;;");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
        "AST(root: CompoundStatement(statements: ["
            "ExpressionStatement(expression: None), "
            "ExpressionStatement(expression: None), "
            "ExpressionStatement(expression: Literal(value: 34)), "
            "ExpressionStatement(expression: None)]))"
                 // clang-format on
    );
}

TEST(Parser, ParseSimpleIfStatement) {
    frontend::Scanner scanner(R"(if (2 <= 5) 3;)");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
         "AST(root: CompoundStatement(statements: ["
            "IfStatement("
                "condition: BinaryExpression("
                    "left: Literal(value: 2), "
                    "operation: LESS_THAN_OR_EQUAL_TO, "
                    "right: Literal(value: 5)), "
                "then: ExpressionStatement(expression: Literal(value: 3)), "
                "else: None)]))"
                 // clang-format on
    );
}

TEST(Parser, ParseSimpleIfElseStatement) {
    frontend::Scanner scanner(R"(
        if (2 <= 5) 3;
        else { 4; }
    )");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
         "AST(root: CompoundStatement(statements: ["
            "IfStatement("
                "condition: BinaryExpression("
                    "left: Literal(value: 2), "
                    "operation: LESS_THAN_OR_EQUAL_TO, "
                    "right: Literal(value: 5)), "
                "then: ExpressionStatement(expression: Literal(value: 3)), "
                "else: CompoundStatement(statements: ["
                        "ExpressionStatement(expression: Literal(value: 4))]))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseIfElseIfElseIfElseStatement) {
    frontend::Scanner scanner(R"(
        if (2 <= 5) 3;
        else if (0 == 1) 4;
        else if (1 == 1) ;
        else { 43; }
    )");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
         "AST(root: CompoundStatement(statements: ["
            "IfStatement("
                "condition: BinaryExpression("
                    "left: Literal(value: 2), "
                    "operation: LESS_THAN_OR_EQUAL_TO, "
                    "right: Literal(value: 5)), "
                "then: ExpressionStatement(expression: Literal(value: 3)), "
                "else: IfStatement("
                    "condition: BinaryExpression("
                        "left: Literal(value: 0), "
                        "operation: EQUAL_TO, "
                        "right: Literal(value: 1)), "
                    "then: ExpressionStatement(expression: Literal(value: 4)), "
                    "else: IfStatement("
                        "condition: BinaryExpression("
                            "left: Literal(value: 1), "
                            "operation: EQUAL_TO, "
                            "right: Literal(value: 1)), "
                        "then: ExpressionStatement(expression: None), "
                        "else: CompoundStatement(statements: [ExpressionStatement(expression: Literal(value: 43))])"
                    ")"
             "))]))"
                 // clang-format on
    );
}

TEST(Parser, ParseCompoundStatements) {
    frontend::Scanner scanner(R"(
        {}

        {
            ;
            54;
            4 < 2;
        }
    )");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
         "AST(root: CompoundStatement(statements: ["
            "CompoundStatement(statements: []), "

            "CompoundStatement(statements: ["
                "ExpressionStatement(expression: None), "
                "ExpressionStatement(expression: Literal(value: 54)), "
                "ExpressionStatement(expression: BinaryExpression("
                    "left: Literal(value: 4), "
                    "operation: LESS_THAN, "
                    "right: Literal(value: 2)))])]))"
                 // clang-format on
    );
}

TEST(Parser, ParseDifferentTypesOfStatements) {
    frontend::Scanner scanner(R"(
        {}
        ;
        4;
        2 + 4;
        1 / 1;
        {
            ;
            54;
            4 < 2;
        }
    )");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
         "AST(root: CompoundStatement(statements: ["
            "CompoundStatement(statements: []), "
            "ExpressionStatement(expression: None), "
            "ExpressionStatement(expression: Literal(value: 4)), "
            "ExpressionStatement(expression: BinaryExpression("
                "left: Literal(value: 2), "
                "operation: ADDITION, "
                "right: Literal(value: 4))), "
            "ExpressionStatement(expression: BinaryExpression("
                "left: Literal(value: 1), "
                "operation: DIVISION, "
                "right: Literal(value: 1))), "
            "CompoundStatement(statements: ["
                "ExpressionStatement(expression: None), "
                "ExpressionStatement(expression: Literal(value: 54)), "
                "ExpressionStatement(expression: BinaryExpression("
                    "left: Literal(value: 4), "
                    "operation: LESS_THAN, "
                    "right: Literal(value: 2)))])]))"
                 // clang-format on
    );
}

} // namespace

#include "parser.h"
#include "scanner.h"

#include <format>
#include <gtest/gtest.h>

namespace {

TEST(Parser, ParseNumbers) {
    frontend::Scanner scanner("43.3242 4.395 986.345");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    EXPECT_STREQ(ast.to_string().c_str(),
                 // clang-format off
                 "AST(root: Block(statements: ["
                    "Literal(value: 43.3242), "
                    "Literal(value: 4.395), "
                    "Literal(value: 986.345)]))"
            // clang-format off
            );
}

TEST(Parser, ParseBinaryExpression) {
    frontend::Scanner scanner("4 % 3 + 5 * 2");
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

TEST(Parser, IfStatement) {
    frontend::Scanner scanner(R"(if (x <= 5) { return 2; })");
    frontend::Parser parser(scanner.scan_tokens());

    // TODO
    FAIL();
}

} // namespace

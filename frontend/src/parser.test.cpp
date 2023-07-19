#include "parser.h"
#include "scanner.h"

#include <format>
#include <gtest/gtest.h>

namespace {

TEST(Parser, Literal) {
    frontend::Scanner scanner("43.3242 4.395 986.345");
    frontend::Parser parser(scanner.scan_tokens());

    auto ast = parser.parse();

    FAIL() << ast.to_string();
}

TEST(Parser, IfStatement) {
    frontend::Scanner scanner(R"(if (x <= 5) { return 2; })");
    frontend::Parser parser(scanner.scan_tokens());

    // TODO

    FAIL();
}

} // namespace

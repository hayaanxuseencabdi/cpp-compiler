#include "parser.h"
#include "scanner.h"

#include <gtest/gtest.h>

namespace {

TEST(ParserTests, SimpleIfStatement) {
    frontend::Scanner scanner(R"(if (x <= 5) { return 2; })");
    frontend::Parser parser(scanner.scan_tokens());
    // TODO
}

} // namespace

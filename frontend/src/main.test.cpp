#include <format>
#include <gtest/gtest.h>
#include <iostream>

#include "scanner.h"

namespace {

TEST(ScannerTests, ScanTokens) {
  frontend::Scanner scanner("[]]]][[{{{");
  const auto tokens = scanner.scan_tokens();
  for (size_t index = 0; index < tokens.size(); ++index) {
    std::cout << std::vformat(
                     "Token #{}: {}",
                     std::make_format_args(index, tokens[index].to_string()))
              << '\n';
  }
  EXPECT_EQ(tokens, std::vector<frontend::Token>{});
}

} // namespace

#pragma once

#include <vector>

#include "ast/node.h"
#include "token.h"

namespace frontend {

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}
    // TODO
private:
    std::vector<Token> tokens_;
};

} // namespace frontend

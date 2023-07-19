#pragma once

#include <memory>
#include <utility>

#include "ast/node.h"

namespace frontend::ast {

class AbstractSyntaxTree {
public:
    AbstractSyntaxTree(std::unique_ptr<Node> root) : root_(std::move(root)) {}

    std::string to_string() const {
        return std::vformat("AST(root: {})",
                            std::make_format_args(root_ != nullptr
                                                      ? root_->to_string()
                                                      : "nullptr"));
    }

private:
    std::unique_ptr<Node> root_;
};

} // namespace frontend::ast

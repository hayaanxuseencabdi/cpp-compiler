#pragma once

#include <format>
#include <string_view>

namespace frontend::ast {

struct Operator {
    enum class Type {
        UNARY_PLUS,
        UNARY_MINUS,

        ADDITION,
        SUBTRACTION,

        MULTIPLICATION,
        DIVISION,
        REMAINDER,

        BITWISE_NOT,
        BITWISE_AND,
        BITWISE_OR,
        BITWISE_XOR,
        BITWISE_LEFT_SHIFT,
        BITWISE_RIGHT_SHIFT,

        LOGICAL_NOT,
        LOGICAL_AND,
        LOGICAL_OR,
    };
};

} // namespace frontend::ast

template <>
struct std::formatter<frontend::ast::Operator::Type>
    : std::formatter<std::string_view> {
    auto format(frontend::ast::Operator::Type type,
                std::format_context& ctx) const {
        std::string_view name = "UNDEFINED";
        switch (type) {
            case frontend::ast::Operator::Type::UNARY_PLUS:
                name = "UNARY_PLUS";
                break;
            case frontend::ast::Operator::Type::UNARY_MINUS:
                name = "UNARY_MINUS";
                break;
            case frontend::ast::Operator::Type::ADDITION:
                name = "ADDITION";
                break;
            case frontend::ast::Operator::Type::SUBTRACTION:
                name = "SUBTRACTION";
                break;
            case frontend::ast::Operator::Type::MULTIPLICATION:
                name = "MULTIPLICATION";
                break;
            case frontend::ast::Operator::Type::DIVISION:
                name = "DIVISION";
                break;
            case frontend::ast::Operator::Type::REMAINDER:
                name = "REMAINDER";
                break;
            case frontend::ast::Operator::Type::BITWISE_NOT:
                name = "BITWISE_NOT";
                break;
            case frontend::ast::Operator::Type::BITWISE_AND:
                name = "BITWISE_AND";
                break;
            case frontend::ast::Operator::Type::BITWISE_OR:
                name = "BITWISE_OR";
                break;
            case frontend::ast::Operator::Type::BITWISE_XOR:
                name = "BITWISE_XOR";
                break;
            case frontend::ast::Operator::Type::BITWISE_LEFT_SHIFT:
                name = "BITWISE_LEFT_SHIFT";
                break;
            case frontend::ast::Operator::Type::BITWISE_RIGHT_SHIFT:
                name = "BITWISE_RIGHT_SHIFT";
                break;
            case frontend::ast::Operator::Type::LOGICAL_NOT:
                name = "LOGICAL_NOT";
                break;
            case frontend::ast::Operator::Type::LOGICAL_AND:
                name = "LOGICAL_AND";
                break;
            case frontend::ast::Operator::Type::LOGICAL_OR:
                name = "LOGICAL_OR";
                break;
        }
        return std::formatter<std::string_view>::format(name, ctx);
    }
};

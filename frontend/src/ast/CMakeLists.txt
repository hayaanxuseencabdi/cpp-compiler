set(
    AST_SOURCE

    ${CMAKE_CURRENT_SOURCE_DIR}/ast.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ast.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node.h

    PARENT_SCOPE
)

set(
    AST_TESTS

    ${CMAKE_CURRENT_SOURCE_DIR}/ast.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/node.test.cpp

    PARENT_SCOPE
)

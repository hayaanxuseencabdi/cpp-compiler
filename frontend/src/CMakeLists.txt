set(
    SOURCE

    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/token.h

    PARENT_SCOPE
)

set(
    TESTS

    ${CMAKE_CURRENT_SOURCE_DIR}/main.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.test.cpp

    PARENT_SCOPE
)

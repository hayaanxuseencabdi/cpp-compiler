set(
    SOURCE

    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp

    PARENT_SCOPE
)

set(
    TESTS

    ${CMAKE_CURRENT_SOURCE_DIR}/main.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.test.cpp

    PARENT_SCOPE
)

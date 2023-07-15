project(compiler-frontend)

add_subdirectory(src)

add_library(
   ${PROJECT_NAME}

   ${SOURCE}
)

target_link_libraries(
    ${PROJECT_NAME}

    PUBLIC Microsoft.GSL::GSL)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_executable(
    ${PROJECT_NAME}-tests

    ${TESTS}
)

target_link_libraries(
    ${PROJECT_NAME}-tests

    PRIVATE ${PROJECT_NAME} GTest::gtest_main GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-tests)

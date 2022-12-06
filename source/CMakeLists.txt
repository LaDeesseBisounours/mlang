cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(testparser VERSION 1.0 )
find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.0 REQUIRED)
find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories("./")

set(SRC_DIR ".")
set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(LEXER_OUT "${PARSER_DIR}/scanner.cpp")
set(PARSER_OUT "${PARSER_DIR}/parser.cpp")


flex_target(LEXER "${SRC_DIR}/scanner.l" "${LEXER_OUT}" )
bison_target(PARSER "${SRC_DIR}/parser.y" "${PARSER_OUT}") 
add_flex_bison_dependency(LEXER PARSER)
add_library(ParserLayer STATIC  "${SRC_DIR}/parser_handler.cpp" "${SRC_DIR}/command.cpp"  "${LEXER_OUT}" "${PARSER_OUT}")
add_executable(testparser "${SRC_DIR}/main.cpp")
target_include_directories(ParserLayer PRIVATE "${PARSER_DIR}")
target_include_directories(testparser PRIVATE "${PARSER_DIR}")
target_link_libraries(testparser ParserLayer)

set(TEST_DIR "test")
add_executable(test "${TEST_DIR}/testCase.cpp")
target_include_directories(test PRIVATE "${PARSER_DIR}")
target_link_libraries(test ParserLayer ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(test test)

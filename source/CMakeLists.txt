cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(testparser VERSION 1.0 )
find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.0 REQUIRED)
include_directories("./")

set(SRC_DIR ".")
set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(LEXER_OUT "${PARSER_DIR}/scanner.cpp")
set(PARSER_OUT "${PARSER_DIR}/parser.cpp")

flex_target(LEXER "${SRC_DIR}/scanner.l" "${LEXER_OUT}" )
bison_target(PARSER "${SRC_DIR}/parser.y" "${PARSER_OUT}") 
add_flex_bison_dependency(LEXER PARSER)
add_executable(testparser "${SRC_DIR}/main.cpp" "${SRC_DIR}/interpreter.cpp" "${SRC_DIR}/command.cpp"  "${LEXER_OUT}" "${PARSER_OUT}")
target_include_directories(testparser PRIVATE "${PARSER_DIR}")

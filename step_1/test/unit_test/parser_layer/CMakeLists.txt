add_executable(test "basic_parsing.cpp")
target_include_directories(test PRIVATE "${PARSER_DIR}")
target_link_libraries(test ParserLayer ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(test test)


# test_basic_parsing
add_executable(test_basic_parsing "basic_parsing.cpp")
target_include_directories(test_basic_parsing PRIVATE "${PARSER_DIR}")
target_link_libraries(test_basic_parsing ParserLayer ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME test_basic_parsing COMMAND test_basic_parsing)

# test_rules

add_executable(test_parsing_rules "parsing_rules.cpp")
target_include_directories(test_parsing_rules PRIVATE "${PARSER_DIR}")
target_link_libraries(test_parsing_rules ParserLayer ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME test_parsing_rules COMMAND test_parsing_rules)


add_executable(lexerUnitTest testKeyword.cpp ${UNIT_TEST_MAIN})
target_link_libraries(lexerUnitTest lexer ${CPPUNIT_LIBRARY})

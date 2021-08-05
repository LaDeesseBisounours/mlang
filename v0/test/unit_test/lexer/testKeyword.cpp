#include "testKeyword.hpp"
#include "lexer/lex.hpp"

#include <iostream>

using namespace lex;

CPPUNIT_TEST_SUITE_REGISTRATION(TestKeyword);
void TestKeyword::setUp() {}
void TestKeyword::tearDown() {}

void TestKeyword::emptyString() {
    lexer l;
    unsigned index = 0;
    std::string s("");
    struct token t = l.lex(s, index);
    CPPUNIT_ASSERT_EQUAL(token_type::EndOfFile, t.type);
}


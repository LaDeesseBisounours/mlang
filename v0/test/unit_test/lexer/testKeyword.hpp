#ifndef TESTKEYWORD_HPP
#define TESTKEYWORD_HPP

#include <cppunit/extensions/HelperMacros.h>

class TestKeyword : public CPPUNIT_NS::TestFixture {
    // establish the test suit
    CPPUNIT_TEST_SUITE(TestKeyword);

    CPPUNIT_TEST(emptyString);
    // finish test suit
    CPPUNIT_TEST_SUITE_END();

public:
    // overide setUp(), init data etc
    void setUp();
    // overide tearDown(), free allocated memory,etc
    void tearDown();

protected:
    // test method testInit
    void emptyString();
};

#endif /* TESTKEYWORD_HPP */

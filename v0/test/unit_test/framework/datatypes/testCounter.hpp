#ifndef TESTCOUNTER_HPP
#define TESTCOUNTER_HPP

#include "framework/basicTypes.hpp"
#include <cppunit/extensions/HelperMacros.h>

class TestCounter : public CPPUNIT_NS::TestFixture {
    // establish the test suit
    CPPUNIT_TEST_SUITE(TestCounter);

    CPPUNIT_TEST(testDefaultInit);
    CPPUNIT_TEST(testCustomInit);
    CPPUNIT_TEST(testIncrement);
    // finish test suit
    CPPUNIT_TEST_SUITE_END();

public:
    // overide setUp(), init data etc
    void setUp();
    // overide tearDown(), free allocated memory,etc
    void tearDown();

private:
    framework::Counter counter;

protected:
    // test method testInit
    void testDefaultInit();
    void testCustomInit();
    void testIncrement();
};

#endif /* TESTCOUNTER_HPP */

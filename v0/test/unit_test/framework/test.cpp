#include "test.hpp"
#include "framework/log.hpp"
#include <iostream>
CPPUNIT_TEST_SUITE_REGISTRATION( TestFramework );
void TestFramework::setUp()
{
    a = 5;
    b = 6;
}
void TestFramework::tearDown()
{
}
void TestFramework::testInit()
{
    unsigned num = framework::multiply(a, b);
    CPPUNIT_ASSERT(num == (a * b));
}

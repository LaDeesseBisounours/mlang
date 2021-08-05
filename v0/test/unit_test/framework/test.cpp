#include "test.hpp"
#include "framework/log.hpp"
#include <iostream>
CPPUNIT_TEST_SUITE_REGISTRATION( TestFramework );
void TestFramework::setUp()
{
    std::cout << "setUp" << std::endl;
    a = 5;
    b = 6;
}
void TestFramework::tearDown()
{
    std::cout << "tearDown" << std::endl;
}
void TestFramework::testInit()
{
    unsigned num = framework::multiply(a, b);
    CPPUNIT_ASSERT(num == (a * b));
}

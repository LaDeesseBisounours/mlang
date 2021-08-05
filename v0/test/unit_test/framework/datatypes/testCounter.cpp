#include "testCounter.hpp"
#include "framework/basicTypes.hpp"

#include <iostream>

using namespace framework;

CPPUNIT_TEST_SUITE_REGISTRATION(TestCounter);
void TestCounter::setUp() { new (&counter) Counter(); }
void TestCounter::tearDown() {}
void TestCounter::testDefaultInit() {
    CPPUNIT_ASSERT_EQUAL(static_cast<index>(0), counter.get());
}

void TestCounter::testCustomInit() {
    new (&counter) Counter(5);
    CPPUNIT_ASSERT_EQUAL(static_cast<index>(5), counter.get());
}
void TestCounter::testIncrement() {
    CPPUNIT_ASSERT_EQUAL(static_cast<index>(0), counter.get());
    CPPUNIT_ASSERT_EQUAL(static_cast<index>(1), (counter++).get());
    CPPUNIT_ASSERT_EQUAL(static_cast<index>(2), (counter++).get());
}

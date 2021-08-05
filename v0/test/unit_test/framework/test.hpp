#include <cppunit/extensions/HelperMacros.h>
/*
 * A test case that is designed to produce
 * example errors and failures
 *
 */
class TestFramework : public CPPUNIT_NS::TestFixture
{
    //establish the test suit of TestComplexNumber
    CPPUNIT_TEST_SUITE( TestFramework);
    //FIXME add test method testInit
    CPPUNIT_TEST( testInit);
    // finish the process
    CPPUNIT_TEST_SUITE_END();
    public:
    // overide setUp(), init data etc
    void setUp();
    //overide tearDown(), free allocated memory,etc
    void tearDown();
    protected:
    //test method testAdd
    void testAdd();
    //test method testEquals
    void testEquals();
    //test method testInit
    void testInit();
    private:
    unsigned a, b;
};

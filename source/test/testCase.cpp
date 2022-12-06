#include "parser_handler.h"
#include <sstream>
// TODO: Include your class to test here.
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MyTestSuite)
BOOST_AUTO_TEST_CASE(MyTestCase)
{

    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "function(1,2,3)";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    //float x = 9.5f;

    //BOOST_CHECK(x != 0.0f);
    //BOOST_CHECK_EQUAL((int)x, 9);
    //BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}
BOOST_AUTO_TEST_CASE(MyTestCase2)
{

    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "function(\"falsk jdf(((;(()";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() != 0);
    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    //float x = 9.5f;

    //BOOST_CHECK(x != 0.0f);
    //BOOST_CHECK_EQUAL((int)x, 9);
    //BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}
BOOST_AUTO_TEST_SUITE_END()

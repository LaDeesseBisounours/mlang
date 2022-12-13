#include "parser_handler.hpp"
#include <sstream>
// TODO: Include your class to test here.
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MyTestSuite)
BOOST_AUTO_TEST_CASE(MyTestCase)
{

    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "lel::function()";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
}
BOOST_AUTO_TEST_CASE(MyTestCase2)
{
    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "function().lol()";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() != 0);
}
BOOST_AUTO_TEST_SUITE_END()

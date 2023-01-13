#include "parser_handler.hpp"
#include <sstream>
// TODO: Include your class to test here.
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
using namespace ParserLayer;

BOOST_AUTO_TEST_SUITE(MyTestSuite)
BOOST_AUTO_TEST_CASE(TestSimpleLet)
{

    ParserLayer::ParserHandler i;
    std::stringstream ss;
    AST_Node* root = new AST_Node(AST_Node::AST_Type::LET_STATEMENT, {"i"},
            new AST_Node(AST_Node::AST_Type::IDENTIFIER, {"Integer"}),
            new AST_Node(AST_Node::AST_Type::ADD, {},
                new AST_Node(AST_Node::AST_Type::MULT, {},
                    new AST_Node(AST_Node::AST_Type::NUMBER, {"5"}),
                    new AST_Node(AST_Node::AST_Type::NUMBER, {"3"})
                    ),
                new AST_Node(AST_Node::AST_Type::NUMBER, {"6"})
                )
            );
    
    ss << "let Integer i = 5 * 3 + 6 ;";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
    BOOST_CHECK(i.get_generated_ast()[0] == root);
    delete root;
}

BOOST_AUTO_TEST_CASE(TestBadLet)
{
    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "let i : Integer;";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() != 0);

}
BOOST_AUTO_TEST_CASE(ExpressionWithIdentifier)
{
    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "i = 5 + lele::lol();";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
}

BOOST_AUTO_TEST_CASE(TestFunctionWithBody)
{
    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "def Integer myFunction( Integer* i, Integer[] arr) { let Integer i = 0; i = 5+ 8 * 7; } ";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
}
BOOST_AUTO_TEST_SUITE_END()

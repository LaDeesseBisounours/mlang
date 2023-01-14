#include <sstream>
#include <iostream>
#define BOOST_TEST_MODULE RuleVerification
#include "parser_handler.hpp"
#include <boost/test/unit_test.hpp>
using namespace ParserLayer;

BOOST_AUTO_TEST_SUITE(GrammarRules)


BOOST_AUTO_TEST_CASE(NUMBER)
{
    AST_Node* expected_ast = new AST_Node(AST_Node::AST_Type::NUMBE, {"5"});

    ParserLayer::ParserHandler i;
    std::stringstream ss;
    
    ss << "5;";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
    BOOST_CHECK(*i.get_generated_ast()[0] == *expected_ast);
}
BOOST_AUTO_TEST_CASE(IDENTIFIER)
{}
BOOST_AUTO_TEST_CASE(POSTFIX_FUNCTION_CALL)
{}
BOOST_AUTO_TEST_CASE(POSTFIX_ARRAY_DEREF)
{}
BOOST_AUTO_TEST_CASE(POSTFIX_DOT)
{}
BOOST_AUTO_TEST_CASE(POSTFIX_ARROW)
{}
BOOST_AUTO_TEST_CASE(POSTFIX_INCREMENT)
{}
BOOST_AUTO_TEST_CASE(POSTFIX_DECREMENT)
{}
BOOST_AUTO_TEST_CASE(UNARY_BASE_PLUS)
{}
BOOST_AUTO_TEST_CASE(UNARY_BASE_MINUS)
{}
BOOST_AUTO_TEST_CASE(UNARY_BASE_ABS)
{}
BOOST_AUTO_TEST_CASE(UNARY_BASE_BOOL_NOT)
{}
BOOST_AUTO_TEST_CASE(UNARY_REF_ASSIGN_REF)
{}
BOOST_AUTO_TEST_CASE(UNARY_REF_ASSIGN_DEREF)
{}
BOOST_AUTO_TEST_CASE(UNARY_PTR_MOVE)
{}
BOOST_AUTO_TEST_CASE(UNARY_PTR_COPY)
{}
BOOST_AUTO_TEST_CASE(MULT)
{}
BOOST_AUTO_TEST_CASE(DIV)
{}
BOOST_AUTO_TEST_CASE(MOD)
{}
BOOST_AUTO_TEST_CASE(ADD)
{}
BOOST_AUTO_TEST_CASE(SUB)
{}
BOOST_AUTO_TEST_CASE(DOUBLEARROW_LEFT)
{}
BOOST_AUTO_TEST_CASE(DOUBLEARROW_RIGHT)
{}
BOOST_AUTO_TEST_CASE(BOOL_LT)
{}
BOOST_AUTO_TEST_CASE(BOOL_LTE)
{}
BOOST_AUTO_TEST_CASE(BOOL_GT)
{}
BOOST_AUTO_TEST_CASE(BOOL_GTE)
{}
BOOST_AUTO_TEST_CASE(BOOL_AND)
{}
BOOST_AUTO_TEST_CASE(BOOL_OR)
{}
BOOST_AUTO_TEST_CASE(BOOL_INEQ)
{}
BOOST_AUTO_TEST_CASE(BOOL_EQ)
{}
BOOST_AUTO_TEST_CASE(ASSIGNMENT)
{}
BOOST_AUTO_TEST_CASE(EXPR)
{}
BOOST_AUTO_TEST_CASE(LET_STATEMENT)
{}
BOOST_AUTO_TEST_CASE(PARAMETER_LIST)
{}
BOOST_AUTO_TEST_CASE(EXPR_LIST)
{}
BOOST_AUTO_TEST_CASE(FUNCTION_PROTOTYPE)
{}
BOOST_AUTO_TEST_CASE(FUNCTION_STATEMENT)
{}
BOOST_AUTO_TEST_CASE(SINGLE_RANGE)
{}
BOOST_AUTO_TEST_CASE(RANGE_INTERSECTION)
{}
BOOST_AUTO_TEST_CASE(RANGE_UNION)
{}
BOOST_AUTO_TEST_CASE(RANGE)
{}
BOOST_AUTO_TEST_CASE(TYPE)
{}
BOOST_AUTO_TEST_CASE(TYPE_POINTER)
{}
BOOST_AUTO_TEST_CASE(TYPE_REFERENCE)
{}
BOOST_AUTO_TEST_CASE(TYPE_ARRAY)
{}
BOOST_AUTO_TEST_CASE(TYPE_RANGE)
{}
BOOST_AUTO_TEST_CASE(STATEMENT)
{}
BOOST_AUTO_TEST_CASE(STATEMENT_LIST)
{}
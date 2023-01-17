#include <iostream>
#include <sstream>
#define BOOST_TEST_MODULE RuleVerification
#include "parser_handler.hpp"
#include <boost/test/unit_test.hpp>
using namespace ParserLayer;

BOOST_AUTO_TEST_SUITE(GrammarRules)


BOOST_AUTO_TEST_CASE(NUMBER) {
    AST_Node* expected_ast =
        new AST_Node(AST_Node::AST_Type::STATEMENT_LIST, {},
                     new AST_Node(AST_Node::AST_Type::NUMBER, {"5"}), nullptr);

    ParserLayer::ParserHandler i;
    std::stringstream ss;

    ss << "5;";
    i.switchInputStream(&ss);

    BOOST_CHECK(i.parse() == 0);
    BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
    delete expected_ast;
}

BOOST_AUTO_TEST_CASE(IDENTIFIER) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(AST_Node::AST_Type::IDENTIFIER, {"i"}), nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "i;";
        i.switchInputStream(&ss);

        BOOST_CHECK(i.parse() == 0);
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
    {
        AST_Node* expected_ast =
            new AST_Node(AST_Node::AST_Type::STATEMENT_LIST, {},
                         new AST_Node(AST_Node::AST_Type::IDENTIFIER,
                                      {"lel", "lol", "lul", "__wtf"}),
                         nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "lel::lol::lul::__wtf;";
        i.switchInputStream(&ss);

        BOOST_CHECK(i.parse() == 0);
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
}

BOOST_AUTO_TEST_CASE(POSTFIX_FUNCTION_CALL) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(AST_Node::AST_Type::POSTFIX_FUNCTION_CALL, {},
                         new AST_Node(AST_Node::AST_Type::IDENTIFIER, {"fun"}),
                         nullptr),
            nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "fun();";
        i.switchInputStream(&ss);

        BOOST_CHECK(i.parse() == 0);
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_FUNCTION_CALL, {},
                new AST_Node(AST_Node::AST_Type::IDENTIFIER,
                             {"foo", "bar", "fun"}),
                new AST_Node(AST_Node::AST_Type::IDENTIFIER, {"somevar"})),
            nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "foo::bar::fun(somevar);";
        i.switchInputStream(&ss);

        BOOST_CHECK(i.parse() == 0);
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
}

BOOST_AUTO_TEST_CASE(POSTFIX_ARROW) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_ARROW, {"lel"},
                new AST_Node(AST_Node::AST_Type::POSTFIX_ARROW, {"lol"},
                             new AST_Node(AST_Node::AST_Type::IDENTIFIER,
                                          {"foo", "bar"}),
                             nullptr),
                nullptr),
            nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "foo::bar->lol->lel;";
        i.switchInputStream(&ss);

        BOOST_CHECK(i.parse() == 0);
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
}

BOOST_AUTO_TEST_CASE(POSTFIX_DOT) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_DOT, {"lel"},
                new AST_Node(AST_Node::AST_Type::POSTFIX_DOT, {"lol"},
                             new AST_Node(AST_Node::AST_Type::IDENTIFIER,
                                          {"foo", "bar"}),
                             nullptr),
                nullptr),
            nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "foo::bar.lol.lel;";
        i.switchInputStream(&ss);

        BOOST_CHECK(i.parse() == 0);
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE(POSTFIX_ARRAY_DEREF) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_DOT, {"lel"},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, {"lol"},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node(AST_Node::AST_Type::IDENTIFIER,
                                        {"foo", "bar"}),
                        new AST_Node(AST_Node::AST_Type::NUMBER, {"0"})),
                    nullptr),
                nullptr),
            nullptr);

        ParserLayer::ParserHandler i;
        std::stringstream ss;

        ss << "foo::bar[0]->lol.lel;";
        i.switchInputStream(&ss);
        std::cout << "value of ast " << i.get_generated_ast() << std::endl;
        BOOST_CHECK(i.parse() == 0);
        std::cout << "expected " << std::endl;
        std::cout << *expected_ast << std::endl;
        std::cout << "parsed " << std::endl;
        std::cout << *i.get_generated_ast() << std::endl;
        BOOST_CHECK(*i.get_generated_ast() == *expected_ast);
        delete expected_ast;
    }
}
// BOOST_AUTO_TEST_CASE(POSTFIX_INCREMENT)
//{}
// BOOST_AUTO_TEST_CASE(POSTFIX_DECREMENT)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_BASE_PLUS)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_BASE_MINUS)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_BASE_ABS)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_BASE_BOOL_NOT)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_REF_ASSIGN_REF)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_REF_ASSIGN_DEREF)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_PTR_MOVE)
//{}
// BOOST_AUTO_TEST_CASE(UNARY_PTR_COPY)
//{}
// BOOST_AUTO_TEST_CASE(MULT)
//{}
// BOOST_AUTO_TEST_CASE(DIV)
//{}
// BOOST_AUTO_TEST_CASE(MOD)
//{}
// BOOST_AUTO_TEST_CASE(ADD)
//{}
// BOOST_AUTO_TEST_CASE(SUB)
//{}
// BOOST_AUTO_TEST_CASE(DOUBLEARROW_LEFT)
//{}
// BOOST_AUTO_TEST_CASE(DOUBLEARROW_RIGHT)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_LT)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_LTE)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_GT)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_GTE)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_AND)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_OR)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_INEQ)
//{}
// BOOST_AUTO_TEST_CASE(BOOL_EQ)
//{}
// BOOST_AUTO_TEST_CASE(ASSIGNMENT)
//{}
// BOOST_AUTO_TEST_CASE(EXPR)
//{}
// BOOST_AUTO_TEST_CASE(LET_STATEMENT)
//{}
// BOOST_AUTO_TEST_CASE(PARAMETER_LIST)
//{}
// BOOST_AUTO_TEST_CASE(EXPR_LIST)
//{}
// BOOST_AUTO_TEST_CASE(FUNCTION_PROTOTYPE)
//{}
// BOOST_AUTO_TEST_CASE(FUNCTION_STATEMENT)
//{}
// BOOST_AUTO_TEST_CASE(SINGLE_RANGE)
//{}
// BOOST_AUTO_TEST_CASE(RANGE_INTERSECTION)
//{}
// BOOST_AUTO_TEST_CASE(RANGE_UNION)
//{}
// BOOST_AUTO_TEST_CASE(RANGE)
//{}
// BOOST_AUTO_TEST_CASE(TYPE)
//{}
// BOOST_AUTO_TEST_CASE(TYPE_POINTER)
//{}
// BOOST_AUTO_TEST_CASE(TYPE_REFERENCE)
//{}
// BOOST_AUTO_TEST_CASE(TYPE_ARRAY)
//{}
// BOOST_AUTO_TEST_CASE(TYPE_RANGE)
//{}
// BOOST_AUTO_TEST_CASE(STATEMENT)
//{}
// BOOST_AUTO_TEST_CASE(STATEMENT_LIST)
//{}
BOOST_AUTO_TEST_SUITE_END()

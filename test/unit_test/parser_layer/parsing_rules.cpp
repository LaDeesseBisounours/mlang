#include <iostream>
#include <sstream>
#define BOOST_TEST_MODULE RuleVerification
#include "parser_handler.hpp"
#include <boost/test/unit_test.hpp>
using namespace ParserLayer;

class AST_Tester {
    ParserLayer::ParserHandler _ph;
    const AST_Node* _expected_ast;
    std::stringstream _ss;
    public:
    enum class EXPECTED {
        FAILURE,
        SUCCESS
    };
    AST_Tester(const char* to_parse, const AST_Node* expected_ast) {
        _ss << to_parse;
        _ph.switchInputStream(&_ss);
        _expected_ast = expected_ast;
    }
    ~AST_Tester() {
        if (_expected_ast != nullptr) {
            delete _expected_ast;
        }
    }

    void operator()(EXPECTED e) {
        if (e == EXPECTED::FAILURE) {
            BOOST_CHECK( _ph.parse() != 0 );
            BOOST_CHECK(_ph.get_generated_ast() == nullptr);
                BOOST_CHECK(_expected_ast == nullptr);
        } else {
            BOOST_CHECK( _ph.parse() == 0 );
            if (_expected_ast == nullptr or _ph.get_generated_ast() == nullptr) {
                BOOST_CHECK(_expected_ast == nullptr);
                BOOST_CHECK(_ph.get_generated_ast() == nullptr);
            } else {
                BOOST_CHECK( *_ph.get_generated_ast() == *_expected_ast );
            }
        }
    }
};

BOOST_AUTO_TEST_SUITE( GrammarRules )


BOOST_AUTO_TEST_CASE( NUMBER ) {
    AST_Node* expected_ast = new AST_Node(
        AST_Node::AST_Type::STATEMENT_LIST, {},
        new AST_Node( AST_Node::AST_Type::NUMBER, { "5" } ), nullptr );

    AST_Tester( "5;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
}

BOOST_AUTO_TEST_CASE( IDENTIFIER ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node( AST_Node::AST_Type::IDENTIFIER, { "i" } ), nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "i;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );

        ////BOOST_CHECK( i.parse() == 0 );
        ////BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        AST_Node* expected_ast =
            new AST_Node( AST_Node::AST_Type::STATEMENT_LIST, {},
                          new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                        { "lel", "lol", "lul", "__wtf" } ),
                          nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "lel::lol::lul::__wtf;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );

        ////BOOST_CHECK( i.parse() == 0 );
        ////BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}

BOOST_AUTO_TEST_CASE( POSTFIX_FUNCTION_CALL ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_FUNCTION_CALL, {},
                new AST_Node( AST_Node::AST_Type::IDENTIFIER, { "fun" } ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "fun();", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );

        ////BOOST_CHECK( i.parse() == 0 );
        ////BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_FUNCTION_CALL, {},
                new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                              { "foo", "bar", "fun" } ),
                new AST_Node( AST_Node::AST_Type::IDENTIFIER, { "somevar" } ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "foo::bar::fun(somevar);", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );

        ////BOOST_CHECK( i.parse() == 0 );
        ////BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}

BOOST_AUTO_TEST_CASE( POSTFIX_ARROW ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_ARROW, { "lel" },
                new AST_Node( AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                              new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                            { "foo", "bar" } ),
                              nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "foo::bar->lol->lel;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );

        ////BOOST_CHECK( i.parse() == 0 );
        ////BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}

BOOST_AUTO_TEST_CASE( POSTFIX_DOT ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_DOT, { "lel" },
                new AST_Node( AST_Node::AST_Type::POSTFIX_DOT, { "lol" },
                              new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                            { "foo", "bar" } ),
                              nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "foo::bar.lol.lel;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );

        ////BOOST_CHECK( i.parse() == 0 );
        ////BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( POSTFIX_ARRAY_DEREF ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_DOT, { "lel" },
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "foo::bar[0]->lol.lel;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( POSTFIX_INCREMENT ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_INCREMENT, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_DOT, { "lel" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "foo::bar[0]->lol.lel++;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( POSTFIX_DECREMENT ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::POSTFIX_DECREMENT, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_DOT, { "lel" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_DECREMENT, {},
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                            new AST_Node(
                                AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                                new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                              { "foo", "bar" } ),
                                new AST_Node( AST_Node::AST_Type::NUMBER,
                                              { "0" } ) ),
                            nullptr ),
                        nullptr ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "foo::bar[0]->lol--.lel--;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_BASE_PLUS ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_BASE_PLUS, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "+ foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_BASE_MINUS ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_BASE_MINUS, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //////std::stringstream ss;

        AST_Tester( "-foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_BASE_ABS ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_BASE_ABS, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "abs foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_BASE_BOOL_NOT ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_BASE_BOOL_NOT, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "not foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_REF_ASSIGN_REF ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_REF_ASSIGN_REF, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( " &foo :: bar [ 0 ] -> lol ;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_REF_ASSIGN_DEREF ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( " *foo :: bar [ 0 ] -> lol ;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( UNARY_PTR_MOVE ) {
    // TODO make test correct
    // TODO add failed tests based on unary priority operators
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_PTR_MOVE, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( " move foo :: bar [ 0 ] -> lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_PTR_MOVE, {},
                new AST_Node(
                    AST_Node::AST_Type::UNARY_REF_ASSIGN_REF, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( " move & foo :: bar [ 0 ] -> lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( " & move foo :: bar [ 0 ] -> lol;", nullptr)(AST_Tester::EXPECTED::FAILURE);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() != 0 );
    }
}
BOOST_AUTO_TEST_CASE( UNARY_PTR_COPY ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_PTR_COPY, {},
                new AST_Node(
                    AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                        new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                      { "foo", "bar" } ),
                        new AST_Node( AST_Node::AST_Type::NUMBER, { "0" } ) ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "copy foo :: bar [ 0 ] -> lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        ////ParserLayer::ParserHandler i;
        ////std::stringstream ss;

        AST_Tester( "* copy foo :: bar [ 0 ] -> lol;", nullptr)(AST_Tester::EXPECTED::FAILURE);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() != 0 );
    }

    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::UNARY_PTR_COPY, {},
                new AST_Node(
                    AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ),
                nullptr ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "copy *foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( MULT ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::MULT, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 * *foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( DIV ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::DIV, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 / *foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( MOD ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::MOD, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 % *foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( ADD ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::ADD, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::UNARY_BASE_PLUS, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 + +foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::ADD, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::MULT, {},
                    new AST_Node(
                        AST_Node::AST_Type::UNARY_BASE_PLUS, {},
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                            new AST_Node(
                                AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                                new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                              { "foo", "bar" } ),
                                new AST_Node( AST_Node::AST_Type::NUMBER,
                                              { "0" } ) ),
                            nullptr ),
                        nullptr ),
                    new AST_Node( AST_Node::AST_Type::IDENTIFIER, { "i" } ) ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 + +foo::bar[0]->lol * i;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE( SUB ) {
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::SUB, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::UNARY_BASE_MINUS, {},
                    new AST_Node(
                        AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                            new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                          { "foo", "bar" } ),
                            new AST_Node( AST_Node::AST_Type::NUMBER,
                                          { "0" } ) ),
                        nullptr ),
                    nullptr ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 - -foo::bar[0]->lol;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::SUB, {},
                new AST_Node(
                    AST_Node::AST_Type::MULT, {},
                    new AST_Node(
                        AST_Node::AST_Type::UNARY_BASE_MINUS, {},
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                            new AST_Node(
                                AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                                new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                              { "foo", "bar" } ),
                                new AST_Node( AST_Node::AST_Type::NUMBER,
                                              { "0" } ) ),
                            nullptr ),
                        nullptr ),
                    new AST_Node( AST_Node::AST_Type::IDENTIFIER, { "i" } ) ),
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "-foo::bar[0]->lol * i - 8 ;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
    {
        AST_Node* expected_ast = new AST_Node(
            AST_Node::AST_Type::STATEMENT_LIST, {},
            new AST_Node(
                AST_Node::AST_Type::SUB, {},
                new AST_Node( AST_Node::AST_Type::NUMBER, { "8" } ),
                new AST_Node(
                    AST_Node::AST_Type::MULT, {},
                    new AST_Node(
                        AST_Node::AST_Type::UNARY_BASE_MINUS, {},
                        new AST_Node(
                            AST_Node::AST_Type::POSTFIX_ARROW, { "lol" },
                            new AST_Node(
                                AST_Node::AST_Type::POSTFIX_ARRAY_DEREF, {},
                                new AST_Node( AST_Node::AST_Type::IDENTIFIER,
                                              { "foo", "bar" } ),
                                new AST_Node( AST_Node::AST_Type::NUMBER,
                                              { "0" } ) ),
                            nullptr ),
                        nullptr ),
                    new AST_Node( AST_Node::AST_Type::IDENTIFIER, { "i" } ) ) ),
            nullptr );

        ////ParserLayer::ParserHandler i;
        //std::stringstream ss;

        AST_Tester( "8 - -foo::bar[0]->lol * i;", expected_ast)(AST_Tester::EXPECTED::SUCCESS);
        //i.switchInputStream( &ss );
        //BOOST_CHECK( i.parse() == 0 );
        //BOOST_CHECK( *i.get_generated_ast() == *expected_ast );
        //delete expected_ast;
    }
}
BOOST_AUTO_TEST_CASE(DOUBLEARROW_LEFT)
{}
BOOST_AUTO_TEST_CASE(DOUBLEARROW_RIGHT)
{}
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

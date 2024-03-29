/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Krzysztof Narkiewicz <krzysztof.narkiewicz@ezaquarii.com>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define api.parser.class { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { ParserLayer }
%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>
    #include "ast.hpp"

    using namespace std;

    namespace ParserLayer {
        class Scanner;
        class ParserHandler;
    }
}

// Bison calls yylex() function that must be provided by us to suck tokens
// from the scanner. This block will be placed at the beginning of IMPLEMENTATION file (cpp).
// We define this function here (function! not method).
// This function is called only inside Bison, so we make it static to limit symbol visibility for the linker
// to avoid potential linking conflicts.
%code top
{
    #include <iostream>
    #include "scanner.hpp"
    #include "parser.hpp"
    #include "parser_handler.hpp"
    #include "location.hh"
    
    // yylex() arguments are defined in parser.y
    static ParserLayer::Parser::symbol_type yylex(ParserLayer::Scanner &scanner, ParserLayer::ParserHandler &) {
        return scanner.get_next_token();
    }
    
    // you can accomplish the same thing by inlining the code using preprocessor
    // x and y are same as in above static function
    // #define yylex(x, y) scanner.get_next_token()
    
    using namespace ParserLayer;
}

%lex-param { ParserLayer::Scanner &scanner }
%lex-param { ParserLayer::ParserHandler &driver }
%parse-param { ParserLayer::Scanner &scanner }
%parse-param { ParserLayer::ParserHandler &driver }
%locations
%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOKEN_}

//%token END 0 "end of file"
//%token <std::string> STRING  "string";
//%token <uint64_t> INT_NUMBER "number";
//%token <double> FLOAT_NUMBER
//%token LEFTPAR "leftpar";
//%token RIGHTPAR "rightpar";
//%token SEMICOLON "semicolon";
//%token COMMA "comma";

%token END 0 "end of file"
%token <std::string> NAME_ID  "string";
%token <std::string> NUMBER "number"; //string and we parse it later
%token TRUE;
%token FALSE;

%token PARENTHESES_OPEN PARENTHESES_CLOSE
%token BRACKETS_OPEN BRACKETS_CLOSE
%token CURLY_BRACKETS_OPEN CURLY_BRACKETS_CLOSE
%token SINGLE_APOSTROPHE DOUBLE_APOSTROPHE

%token COLON SEMICOLON //: ;
%token DOT ARROW
%token COMMA
%token INCREMENT DECREMENT

%token BOOL_NOT
%token ASSIGN_MOVE ASSIGN_COPY ASSIGN_DEREF ASSIGN_REF

//key words
%token CLASS STRUCT FUNCTION LET ALIAS AS

//binary ops
%token PLUS MINUS ABS DIVISION MOD EQUAL
%token DOUBLE_ARR_LEFT DOUBLE_ARR_RIGHT

//bool bin ops
%token BOOL_LT BOOL_LTE BOOL_GT BOOL_GTE
%token BOOL_AND BOOL_OR
%token BOOL_EQ BOOL_INEQ

//ranges
%token RANGE UNION INTERSECTION


//%type< ParserLayer::AST_Node > command;
//%type< std::vector<uint64_t> > arguments;

%type<AST_Node*> r_parameter_list;
%type<AST_Node*> r_type;
%type<AST_Node*> r_range;
%type<AST_Node*> r_range_parentheses;
%type<AST_Node*> r_range_union;
%type<AST_Node*> r_range_intersection;
%type<AST_Node*> r_single_range;
%type<AST_Node*> r_statement_list;
%type<AST_Node*> r_statement;
%type<AST_Node*> r_function_statement;
%type<AST_Node*> r_function_base;
%type<AST_Node*> r_let_statement;
%type<AST_Node*> r_expr_list;
%type<AST_Node*> r_expr;
%type<AST_Node*> r_assignment_expr;
%type<AST_Node*> r_bool_eq;
%type<AST_Node*> r_bool_or;
%type<AST_Node*> r_bool_and;
%type<AST_Node*> r_bool_ineq;
%type<AST_Node*> r_doublearrow_expr;
%type<AST_Node*> r_add_expr;
%type<AST_Node*> r_mult_expr;
%type<AST_Node*> r_unary_ptr;
%type<AST_Node*> r_unary_ref;
%type<AST_Node*> r_unary_base;
%type<AST_Node*> r_postfix_expr;
%type<AST_Node*> r_parentheses_expr;
%type<AST_Node*> r_primary_expr;
%type<AST_Node*> r_identifier;

%start r_top_level_statement

// operation order, each operation h
// basic_expr
// parentheses_op
// unary operator
// mult_operator
// add_operator
// bool_operator

%%

r_identifier
    : NAME_ID
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::IDENTIFIER);
        res->pushContent($1);
        $$ = res;
    }
    | r_identifier COLON COLON NAME_ID
    {
        AST_Node* res = $1;
        res->pushContent($4);
        $$ = res;
    }
    ;


r_primary_expr
    : r_identifier
    { $$ = $1; }
    | NUMBER
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::NUMBER);
        res->pushContent($1);
        $$ = res;
    }
    | TRUE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TRUE);
        $$ = res;
    }
    | FALSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::FALSE);
        $$ = res;
    }
    // | STRING_LITERAL
    ;

r_parentheses_expr
    : r_primary_expr
    { $$ = $1; }
    | PARENTHESES_OPEN r_expr PARENTHESES_CLOSE
    { $$ = $2; }
    ;

r_postfix_expr
    : r_parentheses_expr 
    { $$ = $1; }
    | r_postfix_expr PARENTHESES_OPEN PARENTHESES_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_FUNCTION_CALL);
        res->setLeft($1);
        $$ = res;
    }
    | r_postfix_expr PARENTHESES_OPEN r_expr_list PARENTHESES_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_FUNCTION_CALL);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_postfix_expr BRACKETS_OPEN r_expr BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_ARRAY_DEREF);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_postfix_expr DOT NAME_ID
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_DOT);
        res->setLeft($1);
        res->pushContent($3);
        $$ = res;
    }
    | r_postfix_expr ARROW NAME_ID
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_ARROW);
        res->setLeft($1);
        res->pushContent($3);
        $$ = res;
    }
    | r_postfix_expr INCREMENT
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_INCREMENT);
        res->setLeft($1);
        $$ = res;
    }
    | r_postfix_expr DECREMENT
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::POSTFIX_DECREMENT);
        res->setLeft($1);
        $$ = res;
    }
    ;

r_unary_base
    : r_postfix_expr
    { $$ = $1; }
    | PLUS r_unary_base 
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_BASE_PLUS);
        res->setLeft($2);
        $$ = res;
    }
    | MINUS r_unary_base 
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_BASE_MINUS);
        res->setLeft($2);
        $$ = res;
    }
    | ABS r_unary_base
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_BASE_ABS);
        res->setLeft($2);
        $$ = res;
    }
    | BOOL_NOT r_unary_base
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_BASE_BOOL_NOT);
        res->setLeft($2);
        $$ = res;
    }
    ;

r_unary_ref
    : r_unary_base
    { $$ = $1; }
    | ASSIGN_REF  r_unary_ref
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_REF_ASSIGN_REF);
        res->setLeft($2);
        $$ = res;
    }
    | ASSIGN_DEREF r_unary_ref
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF);
        res->setLeft($2);
        $$ = res;
    }
    ;

r_unary_ptr
    : r_unary_ref
    { $$ = $1; }
    | ASSIGN_MOVE r_unary_ptr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_PTR_MOVE);
        res->setLeft($2);
        $$ = res;
    }
    | ASSIGN_COPY r_unary_ptr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::UNARY_PTR_COPY);
        res->setLeft($2);
        $$ = res;
    }
    ;

r_mult_expr
    : r_unary_ptr
    { $$ = $1; }
    | r_mult_expr ASSIGN_DEREF r_unary_ptr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::MULT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_mult_expr DIVISION r_unary_ptr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::DIV);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_mult_expr MOD r_unary_ptr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::MOD);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_add_expr
    : r_mult_expr
    { $$ = $1; }
    | r_add_expr PLUS r_mult_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::ADD);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_add_expr MINUS r_mult_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SUB);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;


r_bool_ineq
    : r_add_expr 
    { $$ = $1; }
    | r_bool_ineq BOOL_LT r_add_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_LT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_bool_ineq BOOL_LTE r_add_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_LTE);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_bool_ineq BOOL_GT r_add_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_GT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_bool_ineq BOOL_GTE r_add_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_GTE);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;
r_bool_eq
    : r_bool_ineq
    { $$ = $1; }
    | r_bool_eq BOOL_EQ r_bool_ineq
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_EQ);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_bool_eq BOOL_INEQ r_bool_ineq
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_INEQ);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_bool_and
    : r_bool_eq
    { $$ = $1; }
    | r_bool_and BOOL_AND r_bool_eq
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_AND);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_bool_or
    : r_bool_and
    { $$ = $1; }
    | r_bool_or BOOL_OR r_bool_and
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::BOOL_OR);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_doublearrow_expr
    : r_bool_or
    { $$ = $1; }
    | r_doublearrow_expr DOUBLE_ARR_LEFT r_bool_or
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::DOUBLEARROW_LEFT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    | r_doublearrow_expr DOUBLE_ARR_RIGHT r_bool_or
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::DOUBLEARROW_RIGHT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_assignment_expr
    : r_unary_ref EQUAL r_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::ASSIGNMENT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_expr
    : r_doublearrow_expr
    { $$ = $1; }
    | r_assignment_expr
    { $$ = $1; }
    ;

r_expr_list
    : r_expr
    { $$ = $1; }
    | r_expr_list COMMA r_expr
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

//====let======================================================================

r_let_statement
    : LET r_type NAME_ID SEMICOLON
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::LET_STATEMENT);
        res->setLeft($2);
        res->pushContent($3);
        $$ = res;
    }
    | LET r_type NAME_ID EQUAL r_expr SEMICOLON
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::LET_STATEMENT);
        res->setLeft($2);
        res->pushContent($3);
        res->setRight($5);
        $$ = res;
    }
    ;

//====function=================================================================
r_function_base
    : FUNCTION r_type r_identifier PARENTHESES_OPEN PARENTHESES_CLOSE 
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_PROTOTYPE);
        res->setLeft($2);
        res->setRight($3);
        $$ = res;
    }
    | FUNCTION r_type r_identifier PARENTHESES_OPEN r_parameter_list PARENTHESES_CLOSE 
    {
        AST_Node* resbase = new AST_Node(AST_Node::AST_Type::FUNCTION_PROTOTYPE);
        AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_PROTOTYPE);
        resbase->setLeft($2);
        resbase->setRight($3);
        res->setLeft(resbase);
        res->setRight($5);
        $$ = res;
    }
    ;

r_function_statement
    : r_function_base SEMICOLON
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_STATEMENT);
        res->setLeft($1);
        $$ = res;
    }
    | r_function_base CURLY_BRACKETS_OPEN CURLY_BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_STATEMENT);
        res->setLeft($1);
        $$ = res;
    }
    | r_function_base CURLY_BRACKETS_OPEN r_statement_list CURLY_BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_STATEMENT);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

//====statement================================================================
r_statement
    : r_let_statement
    { $$ = $1; }
    | r_expr SEMICOLON
    { $$ = $1; }
    | r_function_statement
    { $$ = $1; }
    ;

r_statement_list
    : r_statement
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::STATEMENT_LIST);
        res->setLeft($1);
        $$ = res;
    }
    | r_statement_list r_statement
    {
        AST_Node* prev = $1;
        AST_Node* next = $2;
        if (prev->getRight() == nullptr)
        {
            prev->setRight(next);
            $$ = prev;
        }
        else
        {
            AST_Node* res = new AST_Node(AST_Node::AST_Type::STATEMENT_LIST);
            res->setLeft(prev);
            res->setRight(next);
            $$ = res;
        }
    }
    ;

r_top_level_statement
    : r_statement_list
    {
        driver.setAST_Node($1);
    }
    ;
//====range====================================================================
r_single_range
    : BRACKETS_CLOSE r_add_expr SEMICOLON r_add_expr BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setLeft($2);
        res->setRight($4);
        res->pushContent("]");
        res->pushContent("]");
        $$ = res;
    }
    | BRACKETS_CLOSE r_add_expr SEMICOLON r_add_expr BRACKETS_OPEN
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setLeft($2);
        res->setRight($4);
        res->pushContent("]");
        res->pushContent("[");
        $$ = res;
    }
    | BRACKETS_OPEN r_add_expr SEMICOLON r_add_expr BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setLeft($2);
        res->setRight($4);
        res->pushContent("[");
        res->pushContent("]");
        $$ = res;
    }
    | BRACKETS_OPEN r_add_expr SEMICOLON r_add_expr BRACKETS_OPEN
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setLeft($2);
        res->setRight($4);
        res->pushContent("[");
        res->pushContent("[");
        $$ = res;
    }
    //unbound
    | BRACKETS_CLOSE SEMICOLON r_add_expr BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setRight($3);
        res->pushContent("]");
        res->pushContent("]");
        $$ = res;
    }
    | BRACKETS_CLOSE SEMICOLON r_add_expr BRACKETS_OPEN
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setRight($3);
        res->pushContent("]");
        res->pushContent("[");
        $$ = res;
    }
    | BRACKETS_CLOSE r_add_expr SEMICOLON BRACKETS_OPEN
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setLeft($2);
        res->pushContent("]");
        res->pushContent("[");
        $$ = res;
    }
    | BRACKETS_OPEN r_add_expr SEMICOLON BRACKETS_OPEN
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::SINGLE_RANGE);
        res->setLeft($2);
        res->pushContent("[");
        res->pushContent("[");
        $$ = res;
    }
    ;

r_range_parentheses
    : r_single_range
    { $$ = $1; }
    | PARENTHESES_OPEN r_range_union PARENTHESES_CLOSE
    { $$ = $2; }
    ;

r_range_intersection
    : r_range_parentheses
    { $$ = $1; }
    | r_range_intersection INTERSECTION r_single_range
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::RANGE_INTERSECTION);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_range_union
    : r_range_intersection
    { $$ = $1; }
    | r_range_union UNION r_range_intersection
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::RANGE_UNION);
        res->setLeft($1);
        res->setRight($3);
        $$ = res;
    }
    ;

r_range
    : RANGE r_range_union
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::RANGE);
        res->setLeft($2);
        $$ = res;
    }
    ;

//====type=====================================================================
r_type
    : r_identifier
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TYPE);
        res->setLeft($1);
        $$ = res;
    }
    | r_type ASSIGN_DEREF // * operator
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TYPE_POINTER);
        res->setLeft($1);
        $$ = res;
    }
    | r_type ASSIGN_REF
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TYPE_REFERENCE);
        res->setLeft($1);
        $$ = res;
    }
    | r_type BRACKETS_OPEN BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TYPE_ARRAY);
        res->setLeft($1);
        $$ = res;
    }
    | r_type BRACKETS_OPEN NUMBER BRACKETS_CLOSE
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TYPE_ARRAY);
        res->setLeft($1);
        res->pushContent($3);
        $$ = res;
    }
    | r_type r_range
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::TYPE_RANGE);
        res->setLeft($1);
        res->setRight($2);
        $$ = res;
    }
    ;


//====parameters===============================================================
r_parameter_list
    : r_type NAME_ID
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::PARAMETER_LIST);
        res->setLeft($1);
        res->pushContent($2);
        $$ = res;
    }
    | r_parameter_list COMMA r_type NAME_ID
    {
        AST_Node* res = new AST_Node(AST_Node::AST_Type::PARAMETER_LIST);
        res->setLeft($1);
        res->pushContent($4);
        res->setRight($3);
        $$ = res;
    }
    ;

//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================




%%

// Bison expects us to provide implementation - otherwise linker complains
void ParserLayer::Parser::error(const location & , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
    cout << "Error: " << message << endl << "Error location: " << driver.location() << endl;
}

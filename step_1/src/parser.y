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
    static ParserLayer::Parser::symbol_type yylex(ParserLayer::Scanner &scanner, ParserLayer::ParserHandler &driver) {
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

%token PARENTHESES_OPEN PARENTHESES_CLOSE
%token BRACKETS_OPEN BRACKETS_CLOSE
%token CURLY_BRACKETS_OPEN CURLY_BRACKETS_CLOSE
%token SINGLE_APOSTROPHE DOUBLE_APOSTROPHE

%token PROPERTY_SIGN
%token COLON SEMICOLON //: ;
%token DOT ARROW
%token COMMA
%token INCREMENT DECREMENT

%token BOOL_NOT
%token ASSIGN_MOVE ASSIGN_COPY ASSIGN_DEREF ASSIGN_REF

//key words
%token CLASS STRUCT FUNCTION LET ALIAS AS

//binary ops
%token PLUS MINUS ABS MULT DIVISION MOD
%token DOUBLE_ARR_LEFT DOUBLE_ARR_RIGHT

//bool bin ops
%token BOOL_LT BOOL_LTE BOOL_GT BOOL_GTE
%token BOOL_AND BOOL_OR
%token BOOL_EQ BOOL_INEQ

//ranges
%token RANGE UNION INTERSECTION


//%type< ParserLayer::AST_Node > command;
//%type< std::vector<uint64_t> > arguments;

%type< AST_Node* > r_expr ;
%type< AST_Node* > r_function_call ;
%type< AST_Node* > r_dereference ;
%type< AST_Node* > r_parameter_list ;
%type< AST_Node* > r_identifier ;
%type< AST_Node* > r_name_property ;
%type< AST_Node* > r_block_property ;
%type< AST_Node* > r_single_property ;
%type< AST_Node* > r_any_property ;
%start r_expr

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
    | r_namespace_id COLON COLON NAME_ID
    ;


r_primary_expr
    : r_identifier
    | NUMBER
    // | STRING_LITERAL
    ;

r_parentheses_expr
    : r_primary_expr
    | PARENTHESES_OPEN expr PARENTHESES_CLOSE
    ;

r_postfix_expr
    : r_parentheses_expr 
    | r_postfix_expr PARENTHESES_OPEN PARENTHESES_CLOSE
    | r_postfix_expr PARENTHESES_OPEN expr_list PARENTHESES_CLOSE
    | r_postfix_expr BRACKETS_OPEN expr BRACKETS_CLOSE
    | r_postfix_expr DOT NAME_ID
    | r_postfix_expr ARROW NAME_ID
    | r_postfix_expr INCREMENT
    | r_postfix_expr DECREMENT
    ;

r_unary_base
    : r_postfix_expr
    | PLUS r_unary_base 
    | MINUS r_unary_base 
    | ABS r_unary_base
    | BOOL_NOT r_unary_base
    ;

r_unary_ref
    : r_unary_base
    | ASSIGN_REF  r_unary_ref
    | ASSIGN_DEREF r_unary_ref
    ;

r_unary_ptr
    : r_unary_ref
    | ASSIGN_MOVE r_unary_ptr
    | ASSIGN_COPY r_unary_ptr
    ;

r_mult_expr
    : r_unary_ptr
    | r_mult_expr MULT r_unary_ptr
    | r_mult_expr DIV r_unary_ptr
    | r_mult_expr MOD r_unary_ptr
    ;

r_add_expr
    : r_mult_expr
    | r_add_expr PLUS r_mult_expr
    | r_add_expr MINUS r_mult_expr
    ;

r_doublearrow_expr
    : r_add_expr
    | r_doublearrow_expr DOUBLE_ARR_LEFT r_add_expr
    | r_doublearrow_expr DOUBLE_ARR_RIGHT r_add_expr
    ;

r_bool_ineq
    : r_doublearrow_expr 
    | r_bool_ineq BOOL_LT r_doublearrow_expr
    | r_bool_ineq BOOL_LTE r_doublearrow_expr
    | r_bool_ineq BOOL_GT r_doublearrow_expr
    | r_bool_ineq BOOL_GTE r_doublearrow_expr
    ;

r_bool_and
    : r_bool_ineq
    | r_bool_and BOOL_AND r_bool_ineq
    ;

r_bool_or
    : r_bool_and
    | r_bool_or BOOL_OR r_bool_and
    ;

r_bool_eq
    : r_bool_or
    | r_bool_eq BOOL_EQ r_bool_or
    | r_bool_eq BOOL_INEQ r_bool_or
    ;

r_simple_range
    : BRACKETS_CLOSE r_expr SEMICOLON r_expr BRACKETS_CLOSE
    | BRACKETS_CLOSE r_expr SEMICOLON r_expr BRACKETS_OPEN
    | BRACKETS_OPEN r_expr SEMICOLON r_expr BRACKETS_CLOSE
    | BRACKETS_OPEN r_expr SEMICOLON r_expr BRACKETS_OPEN
    //unbound
    | BRACKETS_CLOSE SEMICOLON r_expr BRACKETS_CLOSE
    | BRACKETS_CLOSE SEMICOLON r_expr BRACKETS_OPEN
    | BRACKETS_CLOSE r_expr SEMICOLON BRACKETS_OPEN
    | BRACKETS_OPEN r_expr SEMICOLON BRACKETS_OPEN
    ;
r_range_intersection
    : r_simple_range
    | r_range_intersection INTERSECTION r_simple_range
    ;

r_range_union
    : r_range_intersection
    | r_range_union UNION r_range_intersection
    ;

r_range
    : RANGE r_range_union
    ;


r_expr
    : r_bool_eq
    | r_range
    ;






expr;
expr_list;

//==============v0=============================================================
//r_var_assignement
//    : r_type NAME_ID SEMICOLON 
//    | r_type NAME_ID EQUAL r_expr SEMICOLON 
//
//r_type
//    : r_type_np
//    | r_type r_any_property
//    ;
//
//r_type_np
//    : r_identifier
//    | r_type_np POINTER
//    | r_type_np REFERENCE
//    | r_type_np BRACKETS_OPEN BRACKETS_CLOSE
//    | r_type_np BRACKETS_OPEN NUMBER BRACKETS_CLOSE
//    ;
//
//
//r_any_property
//    : r_block_property
//    {   $$ = $1;    }
//    | r_name_property
//    {   $$ = $1;    }
//    | r_single_property
//    {   $$ = $1;    }
//    | r_list_property
//    {   $$ = $1;    }
//    ;
//r_block_property
//    :  PROPERTY_SIGN NAME_ID CURLY_BRACKETS_OPEN r_expr_list CURLY_BRACKETS_CLOSE
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::BLOCK_PROPERTY);
//       res.pushContent($2);
//       res->setLeft($4);
//       $$ = res;
//    }
//    ;
//
//r_list_property
//    : PROPERTY_SIGN NAME_ID BRACKETS_OPEN r_expr_list BRACKETS_CLOSE
//    ;
//r_name_property
//   :  PROPERTY_SIGN NAME_ID 
//   {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::NAME_PROPERTY);
//       res->pushContent($2);
//       $$ = res;
//   }
//   ;
//
//r_single_property
//    : PROPERTY_SIGN NAME_ID PARENTHESES_OPEN r_expr PARENTHESES_CLOSE
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::NAME_PROPERTY);
//       res->pushContent($2);
//       res->setLeft($4);
//       $$ = res;
//    }
//    ;
//
//r_expr_list
//    : r_expr SEMICOLON
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
//       res->setLeft($1);
//       $$ = res;
//    }
//    | r_expr_list r_expr SEMICOLON
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
//       res->setLeft($1);
//       res->setRight($2);
//       $$ = res;
//    }
//    ;
//
//r_expr
//    : r_identifier
//    {   $$ = $1;    }
//    | r_dereference
//    {   $$ = $1;    }
//    | r_function_call
//    {   $$ = $1;    }
//    | r_expr_list
//    {   $$ = $1;    }
//    | r_any_property
//    {   $$ = $1;    }
//    ;
//
//r_parameter_list
//    : r_expr
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
//       res->setLeft($1);
//       $$ = res;
//    }
//    | r_parameter_list COMMA r_expr
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
//       res->setLeft($1);
//       res->setRight($3);
//       $$ = res;
//    }
//    ;
//
//r_function_call
//    : r_expr PARENTHESES_OPEN PARENTHESES_CLOSE
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_CALL);
//       res->setLeft($1);
//       $$ = res;
//    }
//    | r_expr PARENTHESES_OPEN r_parameter_list PARENTHESES_CLOSE
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_CALL);
//       res->setLeft($1);
//       res->setRight($3);
//       $$ = res;
//    }
//    ;
//
//r_dereference
//    : r_expr
//    {
//        $$ = $1;
//    }
//    | r_dereference DOT NAME_ID
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::DOT_DEREFERENCE);
//       res->setLeft($1);
//       res->pushContent($3);
//       $$ = res;
//    }
//    | r_dereference ARROW NAME_ID
//    {
//       AST_Node* res = new AST_Node(AST_Node::AST_Type::ARROW_DEREFERENCE);
//       res->setLeft($1);
//       res->pushContent($3);
//       $$ = res;
//    }
//    ;
//
//r_namespace_id 
//    : NAME_ID
//    {
//        std::string nameid($1);
//        AST_Node* res = new AST_Node(AST_Node::AST_Type::IDENTIFIER);
//        res->pushContent(nameid);
//        $$ = res;
//    }
//    | r_identifier COLON COLON NAME_ID
//    {
//        const std::string nameid($4);
//        AST_Node *arg = $1;
//        arg->pushContent(nameid);
//        $$ = arg;
//    }
//    ;
//=============================================================================

//program :   {
//                cout << "*** RUN ***" << endl;
//                cout << "Type function with list of parmeters. Parameter list can be empty" << endl
//                     << "or contain positive integers only. Examples: " << endl
//                     << " * function()" << endl
//                     << " * function(1,2,3)" << endl
//                     << "Terminate listing with ; to see parsed AST" << endl
//                     << "Terminate parser with Ctrl-D" << endl;
//                
//                cout << endl << "prompt> ";
//                
//                driver.clear();
//            }
//        | program command
//            {
//                const AST_Node &cmd = $2;
//                cout << "command parsed, updating AST" << endl;
//                driver.addAST_Node(cmd);
//                cout << endl << "prompt> ";
//            }
//        | program SEMICOLON
//            {
//                cout << "*** STOP RUN ***" << endl;
//                cout << driver.str() << endl;
//            }
//        ;
//
//
//command : STRING LEFTPAR RIGHTPAR
//        {
//            string &id = $1;
//            cout << "ID: " << id << endl;
//            $$ = AST_Node(id);
//        }
//    | STRING LEFTPAR arguments RIGHTPAR
//        {
//            string &id = $1;
//            const std::vector<uint64_t> &args = $3;
//            cout << "function: " << id << ", " << args.size() << endl;
//            $$ = AST_Node(id, args);
//        }
//    ;
//
//arguments : NUMBER
//        {
//            uint64_t number = $1;
//            $$ = std::vector<uint64_t>();
//            $$.push_back(number);
//            cout << "first argument: " << number << endl;
//        }
//    | arguments COMMA NUMBER
//        {
//            uint64_t number = $3;
//            std::vector<uint64_t> &args = $1;
//            args.push_back(number);
//            $$ = args;
//            cout << "next argument: " << number << ", arg list size = " << args.size() << endl;
//        }
//    ;
//    
%%

// Bison expects us to provide implementation - otherwise linker complains
void ParserLayer::Parser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
    cout << "Error: " << message << endl << "Error location: " << driver.location() << endl;
}

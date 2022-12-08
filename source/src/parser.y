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


//%type< ParserLayer::AST_Node > command;
//%type< std::vector<uint64_t> > arguments;

%type< AST_Node* > r_expr ;
%type< AST_Node* > r_function_call ;
%type< AST_Node* > r_dereference ;
%type< AST_Node* > r_parameter_list ;
%type< AST_Node* > r_identifier ;
%start r_expr

%%

r_expr
    : r_function_call
    {   $$ = $1;    }
    | r_dereference
    {   $$ = $1;    }
    ;

r_parameter_list
    : r_expr
    {
       AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
       res->setLeft($1);
       $$ = res;
    }
    | r_parameter_list COMMA r_expr
    {
       AST_Node* res = new AST_Node(AST_Node::AST_Type::EXPR_LIST);
       res->setLeft($1);
       res->setRight($3);
       $$ = res;
    }
    ;

r_function_call
    : r_expr PARENTHESES_OPEN PARENTHESES_CLOSE
    {
       AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_CALL);
       res->setLeft($1);
       $$ = res;
    }
    | r_expr PARENTHESES_OPEN r_parameter_list PARENTHESES_CLOSE
    {
       AST_Node* res = new AST_Node(AST_Node::AST_Type::FUNCTION_CALL);
       res->setLeft($1);
       res->setRight($3);
       $$ = res;
    }
    ;

r_dereference
    : r_identifier
    {
        $$ = $1;
    }
    | r_dereference DOT NAME_ID
    {
       AST_Node* res = new AST_Node(AST_Node::AST_Type::DOT_DEREFERENCE);
       res->setLeft($1);
       res->pushContent($3);
       $$ = res;
    }
    | r_dereference ARROW NAME_ID
    {
       AST_Node* res = new AST_Node(AST_Node::AST_Type::ARROW_DEREFERENCE);
       res->setLeft($1);
       res->pushContent($3);
       $$ = res;
    }
    ;

r_identifier 
    : NAME_ID
    {
        std::string nameid($1);
        AST_Node* res = new AST_Node(AST_Node::AST_Type::IDENTIFIER);
        res->pushContent(nameid);
        $$ = res;
    }
    | r_identifier COLON COLON NAME_ID
    {
        const std::string nameid($4);
        AST_Node *arg = $1;
        arg->pushContent(nameid);
        $$ = arg;
    }
    ;

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

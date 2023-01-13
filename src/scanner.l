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

%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.hpp"
	#include "parser_handler.hpp"
	#include "parser.hpp"
	#include "location.hh"

	using namespace std;

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
	// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
	#define yyterminate() ParserLayer::Parser::make_END(ParserLayer::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="ParserLayer_"

%%



[0-9][0-9]* {
                cout << "Scanner: decimal number: " << yytext << endl;
                return ParserLayer::Parser::make_NUMBER(yytext, ParserLayer::location());
            }


'           {
                cout << "Scanner: '" << endl;
                return ParserLayer::Parser::make_SINGLE_APOSTROPHE(ParserLayer::location());
            }


\"          {
                cout << "Scanner: '\"'" << endl;
                return ParserLayer::Parser::make_DOUBLE_APOSTROPHE(ParserLayer::location());
            }


\(          {
                cout << "Scanner: '('" << endl;
                return ParserLayer::Parser::make_PARENTHESES_OPEN(ParserLayer::location());
            }


            
\)          { 
                cout << "Scanner: ')'" << endl;
                return ParserLayer::Parser::make_PARENTHESES_CLOSE(ParserLayer::location());
            }


            
;           {
                cout << "Scanner: ';'" << endl;
                return ParserLayer::Parser::make_SEMICOLON(ParserLayer::location());
            }


:           {
                cout << "Scanner: ':'" << endl;
                return ParserLayer::Parser::make_COLON(ParserLayer::location());
            }


            
,           {
                cout << "Scanner: ','" << endl;
                return ParserLayer::Parser::make_COMMA(ParserLayer::location());
            }


            
\[          {
                cout << "Scanner: '['" << endl;
                return ParserLayer::Parser::make_BRACKETS_OPEN(ParserLayer::location());
            }


\]          {
                cout << "Scanner: ']'" << endl;
                return ParserLayer::Parser::make_BRACKETS_CLOSE(ParserLayer::location());
            }


\{          {
                cout << "Scanner: '{'" << endl;
                return ParserLayer::Parser::make_CURLY_BRACKETS_OPEN(ParserLayer::location());
            }


\}          {
                cout << "Scanner: '}'" << endl;
                return ParserLayer::Parser::make_CURLY_BRACKETS_CLOSE(ParserLayer::location());
            }


\.          {
                cout << "Scanner: '.'" << endl;
                return ParserLayer::Parser::make_DOT(ParserLayer::location());
            }


->          {
                cout << "Scanner: '->'" << endl;
                return ParserLayer::Parser::make_ARROW(ParserLayer::location());
            }


\+\+          {
                cout << "Scanner: '++'" << endl;
                return ParserLayer::Parser::make_INCREMENT(ParserLayer::location());
            }


--          {
                cout << "Scanner: '--'" << endl;
                return ParserLayer::Parser::make_DECREMENT(ParserLayer::location());
            }


not         {
                cout << "Scanner: 'not'" << endl;
                return ParserLayer::Parser::make_BOOL_NOT(ParserLayer::location());
            }


move        {
                cout << "Scanner: 'move'" << endl;
                return ParserLayer::Parser::make_ASSIGN_MOVE(ParserLayer::location());
            }


copy        {
                cout << "Scanner: 'copy'" << endl;
                return ParserLayer::Parser::make_ASSIGN_COPY(ParserLayer::location());
            }


\*          {
                cout << "Scanner: '*'" << endl;
                return ParserLayer::Parser::make_ASSIGN_DEREF(ParserLayer::location());
            }


&           {
                cout << "Scanner: '&'" << endl;
                return ParserLayer::Parser::make_ASSIGN_REF(ParserLayer::location());
            }



def         {
                cout << "Scanner: 'def'" << endl;
                return ParserLayer::Parser::make_FUNCTION(ParserLayer::location());
            }


let         {
                cout << "Scanner: 'let'" << endl;
                return ParserLayer::Parser::make_LET(ParserLayer::location());
            }
                                    


\+          {
                cout << "Scanner: '+'" << endl;
                return ParserLayer::Parser::make_PLUS(ParserLayer::location());
            }


-           {
                cout << "Scanner: '-'" << endl;
                return ParserLayer::Parser::make_MINUS(ParserLayer::location());
            }


abs         {
                cout << "Scanner: 'abs'" << endl;
                return ParserLayer::Parser::make_ABS(ParserLayer::location());
            }



\/          {
                cout << "Scanner: '/'" << endl;
                return ParserLayer::Parser::make_DIVISION(ParserLayer::location());
            }


%           {
                cout << "Scanner: '%'" << endl;
                return ParserLayer::Parser::make_MOD(ParserLayer::location());
            }


>>          {
                cout << "Scanner: '>>'" << endl;
                return ParserLayer::Parser::make_DOUBLE_ARR_RIGHT(ParserLayer::location());
            }


\<\<        {
                cout << "Scanner: '<<'" << endl;
                return ParserLayer::Parser::make_DOUBLE_ARR_LEFT(ParserLayer::location());
            }


gt          {
                cout << "Scanner: 'gt'" << endl;
                return ParserLayer::Parser::make_BOOL_GT(ParserLayer::location());
            }


gte         {
                cout << "Scanner: 'gte'" << endl;
                return ParserLayer::Parser::make_BOOL_GTE(ParserLayer::location());
            }


lt          {
                cout << "Scanner: 'lt'" << endl;
                return ParserLayer::Parser::make_BOOL_LT(ParserLayer::location());
            }


lte         {
                cout << "Scanner: 'lte'" << endl;
                return ParserLayer::Parser::make_BOOL_LTE(ParserLayer::location());
            }


and         {
                cout << "Scanner: 'and'" << endl;
                return ParserLayer::Parser::make_BOOL_AND(ParserLayer::location());
            }


or          {
                cout << "Scanner: 'or'" << endl;
                return ParserLayer::Parser::make_BOOL_OR(ParserLayer::location());
            }


==          {
                cout << "Scanner: '=='" << endl;
                return ParserLayer::Parser::make_BOOL_EQ(ParserLayer::location());
            }


!=          {
                cout << "Scanner: '!='" << endl;
                return ParserLayer::Parser::make_BOOL_INEQ(ParserLayer::location());
            }


=          {
                cout << "Scanner: '='" << endl;
                return ParserLayer::Parser::make_EQUAL(ParserLayer::location());
            }


range       {
                cout << "Scanner: 'range'" << endl;
                return ParserLayer::Parser::make_RANGE(ParserLayer::location());
            }


inter       {
                cout << "Scanner: 'inter'" << endl;
                return ParserLayer::Parser::make_INTERSECTION(ParserLayer::location());
            }


union       {
                cout << "Scanner: 'union'" << endl;
                return ParserLayer::Parser::make_UNION(ParserLayer::location());
            }


class       {
                cout << "Scanner: 'class'" << endl;
                return ParserLayer::Parser::make_CLASS(ParserLayer::location());
            }


struct      {
                cout << "Scanner: 'struct'" << endl;
                return ParserLayer::Parser::make_STRUCT(ParserLayer::location());
            }


alias       {
                cout << "Scanner: 'alias'" << endl;
                return ParserLayer::Parser::make_ALIAS(ParserLayer::location());
            }


as          {
                cout << "Scanner: 'as'" << endl;
                return ParserLayer::Parser::make_AS(ParserLayer::location());
            }



[\n\t ]     {
                //cout << "Scanner: whitechar (ignored)" << endl;
            }




[a-zA-Z_]+  { 
                cout << "Scanner: identifier [" << yytext << "]" << endl;
                return ParserLayer::Parser::make_NAME_ID(yytext, ParserLayer::location( /* put location data here if you want */ )); 
            }
.           { 
                cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }


%%


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
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="ParserLayer_"

%%



[0-9][0-9]* {
                //cout << "Scanner: decimal number: " << yytext << endl;
                return ParserLayer::Parser::make_NUMBER(yytext, ParserLayer::location());
            }


'           {
                //cout << "Scanner: '" << endl;
                return ParserLayer::Parser::make_SINGLE_APOSTROPHE(ParserLayer::location());
            }


\"          {
                //cout << "Scanner: '\"'" << endl;
                return ParserLayer::Parser::make_DOUBLE_APOSTROPHE(ParserLayer::location());
            }


\(          {
                //cout << "Scanner: '('" << endl;
                return ParserLayer::Parser::make_PARENTHESES_OPEN(ParserLayer::location());
            }


            
\)          { 
                //cout << "Scanner: ')'" << endl;
                return ParserLayer::Parser::make_PARENTHESES_CLOSE(ParserLayer::location());
            }


            
;           {
                //cout << "Scanner: ';'" << endl;
                return ParserLayer::Parser::make_SEMICOLON(ParserLayer::location());
            }


:           {
                //cout << "Scanner: ':'" << endl;
                return ParserLayer::Parser::make_COLON(ParserLayer::location());
            }


            
,           {
                //cout << "Scanner: ','" << endl;
                return ParserLayer::Parser::make_COMMA(ParserLayer::location());
            }


            
\[          {
                //cout << "Scanner: '['" << endl;
                return ParserLayer::Parser::make_BRACKETS_OPEN(ParserLayer::location());
            }


\]          {
                //cout << "Scanner: ']'" << endl;
                return ParserLayer::Parser::make_BRACKETS_CLOSE(ParserLayer::location());
            }


\{          {
                //cout << "Scanner: '{'" << endl;
                return ParserLayer::Parser::make_CURLY_BRACKETS_OPEN(ParserLayer::location());
            }


\}          {
                //cout << "Scanner: '}'" << endl;
                return ParserLayer::Parser::make_CURLY_BRACKETS_CLOSE(ParserLayer::location());
            }


\.          {
                //cout << "Scanner: '.'" << endl;
                return ParserLayer::Parser::make_DOT(ParserLayer::location());
            }


->          {
                //cout << "Scanner: '->'" << endl;
                return ParserLayer::Parser::make_ARROW(ParserLayer::location());
            }


\+\+          {
                //cout << "Scanner: '++'" << endl;
                return ParserLayer::Parser::make_INCREMENT(ParserLayer::location());
            }


--          {
                //cout << "Scanner: '--'" << endl;
                return ParserLayer::Parser::make_DECREMENT(ParserLayer::location());
            }

true        {
                return ParserLayer::Parser::make_TRUE(ParserLayer::location());
            }
false       {
                return ParserLayer::Parser::make_FALSE(ParserLayer::location());
            }

not         {
                //cout << "Scanner: 'not'" << endl;
                return ParserLayer::Parser::make_BOOL_NOT(ParserLayer::location());
            }


move        {
                //cout << "Scanner: 'move'" << endl;
                return ParserLayer::Parser::make_ASSIGN_MOVE(ParserLayer::location());
            }


copy        {
                //cout << "Scanner: 'copy'" << endl;
                return ParserLayer::Parser::make_ASSIGN_COPY(ParserLayer::location());
            }


\*          {
                //cout << "Scanner: '*'" << endl;
                return ParserLayer::Parser::make_ASSIGN_DEREF(ParserLayer::location());
            }


&           {
                //cout << "Scanner: '&'" << endl;
                return ParserLayer::Parser::make_ASSIGN_REF(ParserLayer::location());
            }



def         {
                //cout << "Scanner: 'def'" << endl;
                return ParserLayer::Parser::make_FUNCTION(ParserLayer::location());
            }


let         {
                //cout << "Scanner: 'let'" << endl;
                return ParserLayer::Parser::make_LET(ParserLayer::location());
            }
                                    


\+          {
                //cout << "Scanner: '+'" << endl;
                return ParserLayer::Parser::make_PLUS(ParserLayer::location());
            }


-           {
                //cout << "Scanner: '-'" << endl;
                return ParserLayer::Parser::make_MINUS(ParserLayer::location());
            }


abs         {
                //cout << "Scanner: 'abs'" << endl;
                return ParserLayer::Parser::make_ABS(ParserLayer::location());
            }



\/          {
                //cout << "Scanner: '/'" << endl;
                return ParserLayer::Parser::make_DIVISION(ParserLayer::location());
            }


%           {
                //cout << "Scanner: '%'" << endl;
                return ParserLayer::Parser::make_MOD(ParserLayer::location());
            }


>>          {
                //cout << "Scanner: '>>'" << endl;
                return ParserLayer::Parser::make_DOUBLE_ARR_RIGHT(ParserLayer::location());
            }


\<\<        {
                //cout << "Scanner: '<<'" << endl;
                return ParserLayer::Parser::make_DOUBLE_ARR_LEFT(ParserLayer::location());
            }


gt          {
                //cout << "Scanner: 'gt'" << endl;
                return ParserLayer::Parser::make_BOOL_GT(ParserLayer::location());
            }


gte         {
                //cout << "Scanner: 'gte'" << endl;
                return ParserLayer::Parser::make_BOOL_GTE(ParserLayer::location());
            }


lt          {
                //cout << "Scanner: 'lt'" << endl;
                return ParserLayer::Parser::make_BOOL_LT(ParserLayer::location());
            }


lte         {
                //cout << "Scanner: 'lte'" << endl;
                return ParserLayer::Parser::make_BOOL_LTE(ParserLayer::location());
            }


and         {
                //cout << "Scanner: 'and'" << endl;
                return ParserLayer::Parser::make_BOOL_AND(ParserLayer::location());
            }


or          {
                //cout << "Scanner: 'or'" << endl;
                return ParserLayer::Parser::make_BOOL_OR(ParserLayer::location());
            }


==          {
                //cout << "Scanner: '=='" << endl;
                return ParserLayer::Parser::make_BOOL_EQ(ParserLayer::location());
            }


!=          {
                //cout << "Scanner: '!='" << endl;
                return ParserLayer::Parser::make_BOOL_INEQ(ParserLayer::location());
            }


=          {
                //cout << "Scanner: '='" << endl;
                return ParserLayer::Parser::make_EQUAL(ParserLayer::location());
            }


range       {
                //cout << "Scanner: 'range'" << endl;
                return ParserLayer::Parser::make_RANGE(ParserLayer::location());
            }


inter       {
                //cout << "Scanner: 'inter'" << endl;
                return ParserLayer::Parser::make_INTERSECTION(ParserLayer::location());
            }


union       {
                //cout << "Scanner: 'union'" << endl;
                return ParserLayer::Parser::make_UNION(ParserLayer::location());
            }


class       {
                //cout << "Scanner: 'class'" << endl;
                return ParserLayer::Parser::make_CLASS(ParserLayer::location());
            }


struct      {
                //cout << "Scanner: 'struct'" << endl;
                return ParserLayer::Parser::make_STRUCT(ParserLayer::location());
            }


alias       {
                //cout << "Scanner: 'alias'" << endl;
                return ParserLayer::Parser::make_ALIAS(ParserLayer::location());
            }


as          {
                //cout << "Scanner: 'as'" << endl;
                return ParserLayer::Parser::make_AS(ParserLayer::location());
            }



[\n\t ]     {
                ////cout << "Scanner: whitechar (ignored)" << endl;
            }




[a-zA-Z_]+  { 
                //cout << "Scanner: identifier [" << yytext << "]" << endl;
                return ParserLayer::Parser::make_NAME_ID(yytext, ParserLayer::location( /* put location data here if you want */ )); 
            }
.           { 
                //cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }

%%

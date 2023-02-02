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

#include "ast.hpp"

#include <iostream>
#include <sstream>

using namespace ParserLayer;
using namespace std;

AST_Node::AST_Node( AST_Node::AST_Type t )
    : _type( t ), _content(), _left( nullptr ), _right( nullptr ) {}
AST_Node::AST_Node( AST_Node& other )
    : _type( other.getType() ), _content(), _left( other.getLeft() ),
      _right( other.getLeft() ) {
    copy( other._content.begin(), other._content.end(),
          std::back_inserter( this->_content ) );
    cout << "copy constructor called" << endl;
}

AST_Node::AST_Node( AST_Node&& other )
    : _type( other.getType() ), _content( move( other._content ) ),
      _left( other.getLeft() ), _right( other.getLeft() ) {
    cout << "move constructor called" << endl;
}

AST_Node::AST_Node( AST_Node::AST_Type t, vector<string> vec )
    : _type( t ), _content( vec ), _left( nullptr ), _right( nullptr ) {}

AST_Node::AST_Node( AST_Node::AST_Type t, const vector<string> vec,
                    AST_Node* left, AST_Node* right )
    : _type( t ), _content( vec ), _left( left ), _right( right ) {}

AST_Node::~AST_Node() {
    delete _left;
    delete _right;
}

void AST_Node::setLeft( AST_Node* n ) noexcept {
    if ( _left != nullptr )
        delete _left;

    _left = n;
}
void AST_Node::setRight( AST_Node* n ) noexcept {
    if ( _right != nullptr )
        delete _right;
    _right = n;
}
AST_Node* AST_Node::getLeft( void ) const noexcept { return _left; }
AST_Node* AST_Node::getRight( void ) const noexcept { return _right; }

AST_Node::AST_Type AST_Node::getType() const noexcept { return _type; }

const vector<string>& AST_Node::getContent() const noexcept { return _content; }

void AST_Node::pushContent( string s ) { _content.push_back( s ); }

void printAST( std::ostream& os, const std::string& prefix,
               const AST_Node* node, bool isLeft ) {
    if ( node != nullptr ) {
        os << prefix;

        os << ( isLeft ? "├──" : "└──" );

        // print the value of the node
        os << node->getType();
        if ( node->getContent().empty() ) {
            os << endl;
        } else {
            os << "=";
            for ( auto& str : node->getContent() ) {
                os << str << ",";
            }
            os << endl;
        }

        // enter the next tree level - left and right branch
        std::string new_prefix;
        if ( isLeft ) {
            new_prefix = prefix + "|   ";
        } else {
            new_prefix = prefix + "    ";
        }

        if ( node->getRight() != nullptr ) {
            printAST( os, new_prefix, node->getLeft(), true );
            printAST( os, new_prefix, node->getRight(), false );
        } else {
            printAST( os, new_prefix, node->getLeft(), false );
        }
    }
}

// pass the root node of your binary tree

namespace ParserLayer {
bool compare_pointers( const AST_Node* l, const AST_Node* r ) {
    if ( l == nullptr ) {
        if ( r == nullptr )
            return true;
        else
            return false;
    }
    if ( r == nullptr )
        return false;

    return *l == *r;
}
bool operator==( const AST_Node& lhs, const AST_Node& rhs ) {
    if ( lhs._type != rhs._type )
        return false;
    if ( lhs._content != rhs._content )
        return false;
    return compare_pointers( lhs._left, rhs._left ) and
           compare_pointers( lhs._right, rhs._right );
}
ostream& operator<<( ostream& os, const AST_Node& node ) {

    printAST( os, "", &node, false );
    return os;
}

ostream& operator<<( ostream& os, const AST_Node::AST_Type& t ) {
    string s;
    switch ( t ) {
    case AST_Node::AST_Type::TRUE:
        s = "TRUE";
        break;
    case AST_Node::AST_Type::FALSE:
        s = "FALSE";
        break;
    case AST_Node::AST_Type::NUMBER:
        s = "NUMBER";
        break;
    case AST_Node::AST_Type::IDENTIFIER:
        s = "IDENTIFIER";
        break;
    case AST_Node::AST_Type::POSTFIX_FUNCTION_CALL:
        s = "POSTFIX_FUNCTION_CALL";
        break;
    case AST_Node::AST_Type::POSTFIX_ARRAY_DEREF:
        s = "POSTFIX_ARRAY_DEREF";
        break;
    case AST_Node::AST_Type::POSTFIX_DOT:
        s = "POSTFIX_DOT";
        break;
    case AST_Node::AST_Type::POSTFIX_ARROW:
        s = "POSTFIX_ARROW";
        break;
    case AST_Node::AST_Type::POSTFIX_INCREMENT:
        s = "POSTFIX_INCREMENT";
        break;
    case AST_Node::AST_Type::POSTFIX_DECREMENT:
        s = "POSTFIX_DECREMENT";
        break;
    case AST_Node::AST_Type::UNARY_BASE_PLUS:
        s = "UNARY_BASE_PLUS";
        break;
    case AST_Node::AST_Type::UNARY_BASE_MINUS:
        s = "UNARY_BASE_MINUS";
        break;
    case AST_Node::AST_Type::UNARY_BASE_ABS:
        s = "UNARY_BASE_ABS";
        break;
    case AST_Node::AST_Type::UNARY_BASE_BOOL_NOT:
        s = "UNARY_BASE_BOOL_NOT";
        break;
    case AST_Node::AST_Type::UNARY_REF_ASSIGN_REF:
        s = "UNARY_REF_ASSIGN_REF";
        break;
    case AST_Node::AST_Type::UNARY_REF_ASSIGN_DEREF:
        s = "UNARY_REF_ASSIGN_DEREF";
        break;
    case AST_Node::AST_Type::UNARY_PTR_MOVE:
        s = "UNARY_PTR_MOVE";
        break;
    case AST_Node::AST_Type::UNARY_PTR_COPY:
        s = "UNARY_PTR_COPY";
        break;
    case AST_Node::AST_Type::MULT:
        s = "MULT";
        break;
    case AST_Node::AST_Type::DIV:
        s = "DIV";
        break;
    case AST_Node::AST_Type::MOD:
        s = "MOD";
        break;
    case AST_Node::AST_Type::ADD:
        s = "ADD";
        break;
    case AST_Node::AST_Type::SUB:
        s = "SUB";
        break;
    case AST_Node::AST_Type::DOUBLEARROW_LEFT:
        s = "DOUBLEARROW_LEFT";
        break;
    case AST_Node::AST_Type::DOUBLEARROW_RIGHT:
        s = "DOUBLEARROW_RIGHT";
        break;
    case AST_Node::AST_Type::BOOL_LT:
        s = "BOOL_LT";
        break;
    case AST_Node::AST_Type::BOOL_LTE:
        s = "BOOL_LTE";
        break;
    case AST_Node::AST_Type::BOOL_GT:
        s = "BOOL_GT";
        break;
    case AST_Node::AST_Type::BOOL_GTE:
        s = "BOOL_GTE";
        break;
    case AST_Node::AST_Type::BOOL_AND:
        s = "BOOL_AND";
        break;
    case AST_Node::AST_Type::BOOL_OR:
        s = "BOOL_OR";
        break;
    case AST_Node::AST_Type::BOOL_INEQ:
        s = "BOOL_INEQ";
        break;
    case AST_Node::AST_Type::BOOL_EQ:
        s = "BOOL_EQ";
        break;
    case AST_Node::AST_Type::ASSIGNMENT:
        s = "ASSIGNMENT";
        break;
    case AST_Node::AST_Type::EXPR:
        s = "EXPR";
        break;
    case AST_Node::AST_Type::LET_STATEMENT:
        s = "LET_STATEMENT";
        break;
    case AST_Node::AST_Type::PARAMETER_LIST:
        s = "PARAMETER_LIST";
        break;
    case AST_Node::AST_Type::EXPR_LIST:
        s = "EXPR_LIST";
        break;
    case AST_Node::AST_Type::FUNCTION_PROTOTYPE:
        s = "FUNCTION_PROTOTYPE";
        break;
    case AST_Node::AST_Type::FUNCTION_STATEMENT:
        s = "FUNCTION_STATEMENT";
        break;
    case AST_Node::AST_Type::SINGLE_RANGE:
        s = "SINGLE_RANGE";
        break;
    case AST_Node::AST_Type::RANGE_INTERSECTION:
        s = "RANGE_INTERSECTION";
        break;
    case AST_Node::AST_Type::RANGE_UNION:
        s = "RANGE_UNION";
        break;
    case AST_Node::AST_Type::RANGE:
        s = "RANGE";
        break;
    case AST_Node::AST_Type::TYPE:
        s = "TYPE";
        break;
    case AST_Node::AST_Type::TYPE_POINTER:
        s = "TYPE_POINTER";
        break;
    case AST_Node::AST_Type::TYPE_REFERENCE:
        s = "TYPE_REFERENCE";
        break;
    case AST_Node::AST_Type::TYPE_ARRAY:
        s = "TYPE_ARRAY";
        break;
    case AST_Node::AST_Type::TYPE_RANGE:
        s = "TYPE_RANGE";
        break;
    case AST_Node::AST_Type::STATEMENT:
        s = "STATEMENT";
        break;
    case AST_Node::AST_Type::STATEMENT_LIST:
        s = "STATEMENT_LIST";
        break;
    }
    os << s;
    return os;
}

} // namespace ParserLayer

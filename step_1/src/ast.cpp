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

AST_Node::AST_Node(AST_Node::AST_Type t) :
    _type(t),
    _content(),
    _left(nullptr),
    _right(nullptr)
{
}
AST_Node::AST_Node(AST_Node& other)
    :
    _type(other.getType()),
    _content(),
    _left(other.getLeft()),
    _right(other.getLeft())
{             
    copy(other._content.begin(), other._content.end(), std::back_inserter(this->_content));
    cout << "copy constructor called" << endl;
}

AST_Node::AST_Node(AST_Node&& other)
    :
    _type(other.getType()),
    _content(move(other._content)),
    _left(other.getLeft()),
    _right(other.getLeft())
{
    cout << "move constructor called" << endl;
}

AST_Node::AST_Node(AST_Node::AST_Type t, vector<string> &vec) :
    _type(t),
    _content(vec),
    _left(nullptr),
    _right(nullptr)
{
    cout << "common constructor called" << endl;
}



AST_Node::~AST_Node()
{
}


void AST_Node::setLeft(AST_Node *n) noexcept
{
    _left = n;
}
void AST_Node::setRight(AST_Node *n) noexcept
{
    _right = n;
}
AST_Node* AST_Node::getLeft(void) const noexcept
{
    return _left;
}
AST_Node* AST_Node::getRight(void) const noexcept
{
    return _right;
}

AST_Node::AST_Type AST_Node::getType() const noexcept
{
    return _type;
}

vector<string>& AST_Node::getContent() 
{
    return _content;
}

void AST_Node::pushContent(string s)
{
    _content.push_back(s);

}
    

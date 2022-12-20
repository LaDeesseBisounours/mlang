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

#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <variant>
#include <stdint.h>

using namespace std;
namespace ParserLayer {


/**
 * AST node. If you can call it AST at all...
 * It keeps function name and a list of arguments.
 */
class AST_Node
{
public:
    enum class AST_Type {
        IDENTIFIER, //Lel::lol::myId
        DOT_DEREFERENCE, // mystruct.lol
        ARROW_DEREFERENCE, // mystruct->lol
        PRPERTY_LIST,
        PROPERTY_SINGLE,
        EXPR,
        EXPR_LIST,
        FUNCTION_CALL,
        NAME_PROPERTY,
        BLOCK_PROPERTY,
        SINGLE_PROPERTY
    };
public:
    AST_Node(AST_Type t);
    AST_Node(AST_Type t, vector<string> &vec);
    AST_Node(AST_Node& other);
    AST_Node(AST_Node&& other);
    ~AST_Node();
    
    //accessors
    void setLeft(AST_Node *n) noexcept;
    void setRight(AST_Node *n) noexcept;
    AST_Node* getLeft() const noexcept;
    AST_Node* getRight() const noexcept;
    AST_Node::AST_Type getType() const noexcept;
    vector<string>& getContent();
    void pushContent(std::string s);
    
private:
    AST_Node::AST_Type _type;
    vector<string> _content;
    AST_Node* _left;
    AST_Node* _right;
};

}

#endif // AST_HPP
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

#include "parser_handler.hpp"
#include "ast.hpp"

#include <sstream>

using namespace ParserLayer;

ParserHandler::ParserHandler() :
    m_scanner(*this),
    m_parser(m_scanner, *this),
    top_level_list(),
    m_location(0)
{

}

int ParserHandler::parse() {
    m_location = 0;
    return m_parser.parse();
}

void ParserHandler::clear() {
    m_location = 0;
}

std::string ParserHandler::str() const {
    std::stringstream s;
    //s << "ParserHandler: " << m_commands.size() << " commands received from command line." << endl;
    //for(int i = 0; i < m_commands.size(); i++) {
    //    s << " * " << m_commands[i].str() << endl;
    //}
    s << "hello" << endl;
    return s.str();
}

void ParserHandler::switchInputStream(std::istream *is) {
    m_scanner.switch_streams(is, NULL);
}

void ParserHandler::addAST_Node(AST_Node *node)
{
    top_level_list.push_back(node);
}

void ParserHandler::increaseLocation(unsigned int loc) {
    m_location += loc;
    cout << "increaseLocation(): " << loc << ", total = " << m_location << endl;
}
const std::vector<AST_Node*>& ParserHandler::get_generated_ast() const {
    return top_level_list;
}

unsigned int ParserHandler::location() const {
    return m_location;
}

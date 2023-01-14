#include "parser_handler.hpp"
#include "ast.hpp"

#include <sstream>

using namespace ParserLayer;

ParserHandler::ParserHandler()
    : m_scanner(*this), m_parser(m_scanner, *this), top_level_list(),
      m_location(0) {}

ParserHandler::~ParserHandler() {
    for (AST_Node *node : top_level_list) {
        delete node;
    }
}

int ParserHandler::parse() {
    m_location = 0;
    return m_parser.parse();
}

void ParserHandler::clear() { m_location = 0; }

std::string ParserHandler::str() const {
    std::stringstream s;
    // s << "ParserHandler: " << m_commands.size() << " commands received from
    // command line." << endl; for(int i = 0; i < m_commands.size(); i++) {
    //     s << " * " << m_commands[i].str() << endl;
    // }
    s << "hello" << endl;
    return s.str();
}

void ParserHandler::switchInputStream(std::istream *is) {
    m_scanner.switch_streams(is, NULL);
}

void ParserHandler::addAST_Node(AST_Node *node) {
    top_level_list.push_back(node);
}

void ParserHandler::increaseLocation(unsigned int loc) { m_location += loc; }
const std::vector<AST_Node *> &ParserHandler::get_generated_ast() const {
    return top_level_list;
}

unsigned int ParserHandler::location() const { return m_location; }

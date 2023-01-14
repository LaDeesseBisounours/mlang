#include "parser_handler.hpp"
#include "ast.hpp"

#include <sstream>

using namespace ParserLayer;

ParserHandler::ParserHandler()
    : m_scanner(*this), m_parser(m_scanner, *this), top_level_ast(nullptr),
      m_location(0) {}

ParserHandler::~ParserHandler() { delete top_level_ast; }

int ParserHandler::parse() {
    m_location = 0;
    return m_parser.parse();
}

void ParserHandler::clear() { m_location = 0; }

void ParserHandler::switchInputStream(std::istream *is) {
    m_scanner.switch_streams(is, NULL);
}

void ParserHandler::setAST_Node(AST_Node *node) { top_level_ast = node; }

void ParserHandler::increaseLocation(unsigned int loc) { m_location += loc; }
const AST_Node *ParserHandler::get_generated_ast() const {
    return top_level_ast;
}

unsigned int ParserHandler::location() const { return m_location; }

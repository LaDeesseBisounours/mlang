#pragma once

#include <vector>

#include "scanner.hpp"

// autogenerated by Bison, don't panic
// if your IDE can't resolve it - call make first
#include "parser.hpp"

namespace ParserLayer {

// forward declare our simplistic AST node class so we
// can declare container for it without the header
class AST_Node;

/**
 * This class is the interface for our scanner/lexer. The end user
 * is expected to use this. It drives scanner/lexer, keeps
 * parsed AST and generally is a good place to store additional
 * context data. Both parser and lexer have access to it via internal
 * references.
 *
 * I know that the AST is a bit too strong word for a simple
 * vector with nodes, but this is only an example. Get off me.
 */
class ParserHandler {
  private:
    Scanner m_scanner;
    Parser m_parser;
    std::vector<AST_Node *> top_level_list; // Example AST
    unsigned int m_location;                // Used by scanner

  public:
    ParserHandler();
    ~ParserHandler();

    /**
     * Run parser. Results are stored inside.
     * \returns 0 on success, 1 on failure
     */
    int parse();

    /**
     * Clear AST
     */
    void clear();

    /**
     * Print AST
     */
    std::string str() const;

    /**
     * Get AST
     */
    const std::vector<AST_Node *> &get_generated_ast() const;

    /**
     * Switch scanner input stream. Default is standard input (std::cin).
     * It will also reset AST.
     */
    void switchInputStream(std::istream *is);

    /**
     * This is needed so that Scanner and Parser can call some
     * methods that we want to keep hidden from the end user.
     */
    friend class Parser;
    friend class Scanner;

  private:
    // Used internally by Parser to insert AST nodes.
    void addAST_Node(AST_Node *node);

    // Used internally by Scanner YY_USER_ACTION to update location indicator
    void increaseLocation(unsigned int loc);

    // Used to get last Scanner location. Used in error messages.
    unsigned int location() const;
};

} // namespace ParserLayer

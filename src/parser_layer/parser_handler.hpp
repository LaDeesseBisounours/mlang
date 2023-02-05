#pragma once

#include <vector>

#include "scanner.hpp"

#include "parser.hpp"

namespace ParserLayer {

class AST_Node;

class ParserHandler {
  private:
    Scanner m_scanner;
    Parser m_parser;
    AST_Node *top_level_ast; // Example AST
    unsigned int m_location; // Used by scanner

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
     * Get AST
     */
    const AST_Node *get_generated_ast() const;

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
    void setAST_Node(AST_Node *node);

    // Used internally by Scanner YY_USER_ACTION to update location indicator
    void increaseLocation(unsigned int loc);

    // Used to get last Scanner location. Used in error messages.
    unsigned int location() const;
};

} // namespace ParserLayer

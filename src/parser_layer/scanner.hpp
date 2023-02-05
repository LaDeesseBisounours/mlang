#pragma once

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer                                                            \
    ParserLayer_FlexLexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL                                                                \
    ParserLayer::Parser::symbol_type ParserLayer::Scanner::get_next_token()

#include "parser.hpp" // this is needed for symbol_type

namespace ParserLayer {

// Forward declare interpreter to avoid include. Header is added
// inimplementation file.
class ParserHandler;

class Scanner : public yyFlexLexer {
  public:
    Scanner(ParserHandler &driver) : m_driver(driver) {}
    virtual ~Scanner() {}
    virtual ParserLayer::Parser::symbol_type get_next_token();

  private:
    ParserHandler &m_driver;
};

} // namespace ParserLayer

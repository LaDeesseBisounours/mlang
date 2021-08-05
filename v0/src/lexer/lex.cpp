#include "lex.hpp"

namespace lex {
    // tokens
    // name [A-Za-z_\-][A-Za-z_\-0-9]*

    /* bool is_white_char(char c) { */
    /*     return c == ' ' && c == '\r' && c == '\n' && c == '\t'; */
    /* } */
    /* unsigned skip_spaces(const std::string &str, unsigned index) { */
    /*     unsigned res = index; */
    /*     while (true) { */
    /*         if (res >= str.size()) */
    /*             return res; */
    /*         char c = str[res]; */

    /*         if (not is_white_char(c)) */
    /*             return res; */

    /*         res++; */
    /*     } */
    /*     return res; */
    /* } */

    /* bool is_valid_name(char c, bool first) { */
    /*     if (first) */
    /*         return ('a' <= c && c => 'z') || ('A' <= c && c => 'Z') ; */

    /*     else */
    /*         return ('a' <= c && c => 'z') || ('A' <= c && c => 'Z') || c ==
     * '_'; */
    /* } */
    /* unsigned property_lex(const std::string& str, unsigned index) { */
    /*     unsigned res = index + 1; */
    /*     if (res ) */

    /* } */
    struct token lexer::lex(const std::string &s, unsigned &index) {
        /* while ( index < s.size() ) { */
        /*     index = skip_spaces(s, index); */
        /*     if (index  < s.size()) */
        /*         return { "", token_type::EndOfFile }; */
        /*     char c = s[index]; */
        /*     if (c == '{') { */
        /*         return { "", token_type::start_scope }; */
        /*     } */
        /*     if (c == '}') { */
        /*         return { "", token_type::end_scope }; */
        /*     } */
        /*     if (c == '#') { */
        /*         return { "", token_type::property }; */
        /*     } */
        /* } */
        return {"", token_type::EndOfFile};
    }
} // namespace lex

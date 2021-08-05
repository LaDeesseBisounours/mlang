#ifndef LEX_HPP
#define LEX_HPP

#include <string>

namespace lex {

    enum token_type {
        word                  , // [a-zA-Z_][a-zA-Z_\-]*
        number                , // [[-]?[0-9]+|[0-9]+b[^ ]+]
        string                , // "anything"
        property_name         , // #![ \t\r\n[]{}<>()]+
        generator_call         , // @![ \t\r\n[]{}<>()]+

        EOF,    //unable to read more
        scope_square_brackets_begin , // [
        scope_square_brackets_end   , // ]
        scope_curly_brackets_begin  , // {
        scope_curly_brackets_end    , // }
        scope_angle_brackets_begin  , // <
        scope_angle_brackets_end    , // >
        scope_parentheses_begin     , // (
        scope_parentheses_end , // )
        reserved_if, // if
        reserved_elif, // elif
        reserved_else, // else
        reserved_dispatch, // switch
        reserved_for,  // for
        reserved_while, // while
        reserved_define, // define
        reserved_declare, // declare
        reserved_requires, // requires
        operator_binary,   // and, or, ||, user defined
        operator_unary   // !, not, user defined
        //========NO STRING ATTACHED============================================
        //-----scope tokens-----------------------------------------------------
        //----Reserved keyword--------------------------------------------------
        //----operators--------------------------------------------------------- 
    };


    


    

    struct token {
        std::string str;
        enum token_type type;
    };

    struct token lexer(const std::string& s, unsigned index);
}
#endif /* LEX_HPP */

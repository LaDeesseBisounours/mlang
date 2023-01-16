#pragma once

#include <iostream>
#include <stdint.h>
#include <string>
#include <variant>
#include <vector>

using namespace std;
namespace ParserLayer {

/**
 * AST node. If you can call it AST at all...
 * It keeps function name and a list of arguments.
 */
class AST_Node {
  public:
    enum class AST_Type {
        NUMBER,
        IDENTIFIER,
        POSTFIX_FUNCTION_CALL,
        POSTFIX_ARRAY_DEREF,
        POSTFIX_DOT,
        POSTFIX_ARROW,
        POSTFIX_INCREMENT,
        POSTFIX_DECREMENT,
        UNARY_BASE_PLUS,
        UNARY_BASE_MINUS,
        UNARY_BASE_ABS,
        UNARY_BASE_BOOL_NOT,
        UNARY_REF_ASSIGN_REF,
        UNARY_REF_ASSIGN_DEREF,
        UNARY_PTR_MOVE,
        UNARY_PTR_COPY,
        MULT,
        DIV,
        MOD,
        ADD,
        SUB,
        DOUBLEARROW_LEFT,
        DOUBLEARROW_RIGHT,
        BOOL_LT,
        BOOL_LTE,
        BOOL_GT,
        BOOL_GTE,
        BOOL_AND,
        BOOL_OR,
        BOOL_INEQ,
        BOOL_EQ,
        ASSIGNMENT,
        EXPR,
        LET_STATEMENT,
        PARAMETER_LIST,
        EXPR_LIST,
        FUNCTION_PROTOTYPE,
        FUNCTION_STATEMENT,
        SINGLE_RANGE,
        RANGE_INTERSECTION,
        RANGE_UNION,
        RANGE,
        TYPE,
        TYPE_POINTER,
        TYPE_REFERENCE,
        TYPE_ARRAY,
        TYPE_RANGE,
        STATEMENT,
        STATEMENT_LIST
    };

  public:
    AST_Node(AST_Type t);
    AST_Node(AST_Type t, vector<string> vec);
    AST_Node(AST_Type t, const vector<string> vec, AST_Node *left,
             AST_Node *right);
    AST_Node(AST_Node &other);
    AST_Node(AST_Node &&other);
    ~AST_Node();

    // accessors
    void setLeft(AST_Node *n) noexcept;
    void setRight(AST_Node *n) noexcept;
    AST_Node *getLeft() const noexcept;
    AST_Node *getRight() const noexcept;
    AST_Node::AST_Type getType() const noexcept;
    const vector<string> &getContent() const noexcept;
    void pushContent(std::string s);
    friend bool operator==(const AST_Node &lhs, const AST_Node &rhs);
    friend ostream &operator<<(ostream &os, const AST_Node &node);
    friend ostream &operator<<(ostream &os, const AST_Node::AST_Type &t);

  private:
    AST_Node::AST_Type _type;
    vector<string> _content;
    AST_Node *_left;
    AST_Node *_right;
};

} // namespace ParserLayer

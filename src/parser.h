#ifndef PARSER_H
#define PARSER_H 1
#include "arena.h"
#include "lexer.h"

#define INDENT_SIZE 7

int get_precision();
void set_precision(char *p);

#define BINOP_LIST                                                             \
  X(ADD)                                                                       \
  X(SUB)                                                                       \
  X(MUL)                                                                       \
  X(DIV)                                                                       \
  X(MOD)                                                                       \
  X(POW)

typedef enum {
#define X(name) OP_##name,
  BINOP_LIST
#undef X
} BinOp;

typedef struct {
  int lbp;
  int rbp;
  int op;
  int valid;
} BinOpInfo;

#define UNARYOP_LIST                                                           \
  X(POSITIVE)                                                                  \
  X(NEGATIVE)

typedef enum UnaryOp {
#define X(name) OP_##name,
  UNARYOP_LIST
#undef X
} UnaryOp;

typedef enum ExprType { EXPR_BINARY, EXPR_NUMERIC, EXPR_UNARY } ExprType;

typedef struct Parser {
  ExprArena *expr_arena;
  TokenArray *token_array;
  int idx;
} Parser;

typedef struct Expr Expr;
struct Expr {
  ExprType type;

  union {
    struct {
      int expr_lhs;
      int expr_rhs;
      BinOp op;
    } binary;

    struct {
      int expr;
      UnaryOp op;
    } unary;

    struct {
      double value;
    } numeric;
  };
};

void print_expr(ExprArena *, int);
void _print_expr_helper(ExprArena *, int, int);
int parse_binary(Parser *, int);
int parse_primary(Parser *);
int parse(Parser *);

#endif // !PARSER_H

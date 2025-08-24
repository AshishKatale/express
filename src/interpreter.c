#include <math.h>

#include "interpreter.h"
#include "parser.h"

double evaluate_numeric(ExprArena *expr_arena, int expr_idx) {
  Expr *e = expr_arena_get(expr_arena, expr_idx);
  return e->numeric.value;
}

double evaluate_unary(ExprArena *expr_arena, int expr_idx) {
  Expr *e = expr_arena_get(expr_arena, expr_idx);
  if (e->unary.op == OP_NEGATIVE) {
    return -evaluate_expr(expr_arena, e->unary.expr);
  }
  return evaluate_expr(expr_arena, e->unary.expr);
}

double evaluate_binary(ExprArena *expr_arena, int expr_idx) {
  double result = 0;

  Expr *e = expr_arena_get(expr_arena, expr_idx);
  double lhs_result = evaluate_expr(expr_arena, e->binary.expr_lhs);
  double rhs_result = evaluate_expr(expr_arena, e->binary.expr_rhs);

  switch (e->binary.op) {
  case OP_ADD:
    result = lhs_result + rhs_result;
    break;
  case OP_SUB:
    result = lhs_result - rhs_result;
    break;
  case OP_MUL:
    result = lhs_result * rhs_result;
    break;
  case OP_DIV:
    result = lhs_result / rhs_result;
    break;
  case OP_MOD:
    result = (int)lhs_result % (int)rhs_result;
    break;
  case OP_POW:
    result = pow(lhs_result, rhs_result);
    break;
  }

  return result;
}

double evaluate_expr(ExprArena *expr_arena, int expr_idx) {
  Expr *e = expr_arena_get(expr_arena, expr_idx);
  switch (e->type) {
  case EXPR_BINARY:
    return evaluate_binary(expr_arena, expr_idx);
  case EXPR_NUMERIC:
    return evaluate_numeric(expr_arena, expr_idx);
  case EXPR_UNARY:
    return evaluate_unary(expr_arena, expr_idx);
  }
  return 0;
}

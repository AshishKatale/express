#ifndef INTERPRETER_H
#define INTERPRETER_H 1
#include "parser.h"

double evaluate_numeric(ExprArena *, int);
double evaluate_unary(ExprArena *, int);
double evaluate_binary(ExprArena *, int);
double evaluate_expr(ExprArena *, int);

#endif // !INTERPRETER_H

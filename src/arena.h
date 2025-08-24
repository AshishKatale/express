#include "lexer.h"

#ifndef ARENA_H
#define ARENA_H 1

typedef struct TokenArray {
  Token *tokens;
  int count;
  int capacity;
} TokenArray;

typedef struct ExprArena {
  struct Expr *exprs;
  int count;
  int capacity;
} ExprArena;

TokenArray *token_array_init();
void token_array_append(TokenArray *, Token);
void token_array_free(TokenArray *);
void token_array_print(TokenArray *);

ExprArena *expr_arena_init();
int expr_arena_alloc(ExprArena *);
void expr_arena_free(ExprArena *);
struct Expr *expr_arena_get(ExprArena *, int);

#endif // !ARENA_H

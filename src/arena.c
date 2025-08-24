#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "lexer.h"
#include "parser.h"

TokenArray *token_array_init() {
  Token *data = calloc(16, sizeof(Token));
  TokenArray *arr = malloc(sizeof(TokenArray));

  arr->tokens = data;
  arr->capacity = 16;
  arr->count = 0;

  return arr;
}

void token_array_append(TokenArray *arr, Token t) {
  if (arr->count >= arr->capacity) {
    arr->capacity *= 2;
    arr->tokens = realloc(arr->tokens, sizeof(Token) * arr->capacity);
    if (!arr->tokens) {
      perror("realloc failed");
      exit(1);
    }
  }
  arr->tokens[arr->count++] = t;
}

void token_array_free(TokenArray *arr) {
  free(arr->tokens);
  free(arr);
}

void token_array_print(TokenArray *arr) {
  printf("Tokens [%d]: [\n", arr->count);
  for (int i = 0; i < arr->count; ++i) {
    print_token(&arr->tokens[i]);
  }
  printf("]\n");
}

ExprArena *expr_arena_init() {
  Expr *data = calloc(16, sizeof(Expr));
  ExprArena *expr_arena = malloc(sizeof(ExprArena));

  expr_arena->exprs = data;
  expr_arena->capacity = 16;
  expr_arena->count = 0;

  return expr_arena;
}

int expr_arena_alloc(ExprArena *expr_arena) {
  if (expr_arena->count >= expr_arena->capacity) {
    expr_arena->capacity *= 2;
    expr_arena->exprs =
        realloc(expr_arena->exprs, sizeof(Expr) * expr_arena->capacity);
    if (!expr_arena->exprs) {
      perror("ERROR: expr arena realloc failed");
      exit(1);
    }
  }

  return expr_arena->count++;
}

Expr *expr_arena_get(ExprArena *expr_arena, int idx) {
  return expr_arena->exprs + idx;
}

void expr_arena_free(ExprArena *expr_arena) {
  free(expr_arena->exprs);
  free(expr_arena);
}

#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "lexer.h"
#include "parser.h"

static int global_precision = 1;
int get_precision() { return global_precision; }
void set_precision(const char *prec) {
  int p = atoi(prec);
  global_precision = p < 1 ? 0 : p;
}

const char *binary_operator_name(BinOp o) {
  switch (o) {
#define X(name)                                                                \
  case OP_##name:                                                              \
    return #name;
    BINOP_LIST
#undef X
  default:
    return "UNKNOWN";
  }
}

const char *unary_operator_name(UnaryOp o) {
  switch (o) {
#define X(name)                                                                \
  case OP_##name:                                                              \
    return #name;
    UNARYOP_LIST
#undef X
  default:
    return "UNKNOWN";
  }
}

void print_error(Parser *parser, char *token_pos, char *msg) {
  char *first_tok_pos = parser->token_array->tokens[0].pos;
  int idx = token_pos - first_tok_pos;
  printf("\nERROR PARSING:\n");
  printf("%s\n", parser->token_array->tokens[0].pos);
  printf("%*s^ %s\n", idx, "", msg);
}

void print_expr(ExprArena *expr_arena, int expr_idx) {
  _print_expr_helper(expr_arena, expr_idx, 0);
  printf("\n");
}

void _print_expr_helper(ExprArena *expr_arena, int expr_idx, int i) {
  Expr *expr = expr_arena_get(expr_arena, expr_idx);
  if (expr == NULL)
    return;

  switch (expr->type) {
  case EXPR_NUMERIC: {
    printf("\n%*s", i * INDENT_SIZE, "");
    printf("Numeric(%.*lf)", global_precision, expr->numeric.value);
    break;
  }
  case EXPR_UNARY: {
    printf("\n%*s", i * INDENT_SIZE, "");
    printf("Unary(");
    printf("\n%*s", (i + 1) * INDENT_SIZE, "");
    printf("%s,", unary_operator_name(expr->unary.op));

    _print_expr_helper(expr_arena, expr->unary.expr, i + 1);

    printf("\n%*s", i * INDENT_SIZE, "");
    printf(")");
    break;
  }
  case EXPR_BINARY: {
    printf("\n%*s", i * INDENT_SIZE, "");
    printf("Binary(");

    _print_expr_helper(expr_arena, expr->binary.expr_lhs, i + 1);

    printf(",\n%*s", (i + 1) * INDENT_SIZE, "");
    printf("%s,", binary_operator_name(expr->binary.op));

    _print_expr_helper(expr_arena, expr->binary.expr_rhs, i + 1);

    printf("\n%*s", i * INDENT_SIZE, "");
    printf(")");
    break;
  }
  }
}

Token *expect_token(Parser *parser, TokenType type) {
  Token *tok = &parser->token_array->tokens[parser->idx];
  if (tok->type != type) {
    print_error(parser, tok->pos, "Unexpected token");
    return NULL;
  }
  return tok;
}

Token *peek_next_token(Parser *parser) {
  return &parser->token_array->tokens[parser->idx];
}

Token *pop_next_token(Parser *parser) {
  return &parser->token_array->tokens[parser->idx++];
}

int get_binding_priorities(TokenType type, int *op, int *lbp, int *rbp) {
  static const BinOpInfo op_table[] = {
      [TOKEN_PLUS] = {1, 2, OP_ADD, 1}, [TOKEN_MINUS] = {1, 2, OP_SUB, 1},
      [TOKEN_STAR] = {3, 4, OP_MUL, 1}, [TOKEN_SLASH] = {3, 4, OP_DIV, 1},
      [TOKEN_MOD] = {3, 4, OP_MOD, 1},  [TOKEN_CARET] = {5, 6, OP_POW, 1},
  };

  if (type < 0 || type >= (int)(sizeof(op_table) / sizeof(op_table[0])))
    return 1;

  const BinOpInfo info = op_table[type];
  if (!info.valid)
    return 1;

  *lbp = info.lbp;
  *rbp = info.rbp;
  *op = info.op;
  return 0;
}

void parse_number(Expr *expr, Token *tok) {
  char num[tok->len];
  sprintf(num, "%.*s", tok->len, tok->pos);
  expr->type = EXPR_NUMERIC;
  expr->numeric.value = atof(num);
}

int parse_binary(Parser *parser, int min_bp) {
  int lhs_expr_idx = parse_primary(parser);
  if (lhs_expr_idx < 0)
    return -1;

  while (1) {
    Token *tok = peek_next_token(parser);

    int lbp, rbp, op;
    int invalid_operator = get_binding_priorities(tok->type, &op, &lbp, &rbp);

    if (invalid_operator &&
        (tok->type != TOKEN_EOF && tok->type != TOKEN_RPAREN)) {
      print_error(parser, tok->pos, "Invalid operator");
      return -1;
      break;
    }

    if (invalid_operator || lbp < min_bp) {
      break;
    }

    pop_next_token(parser);

    int rhs_expr_idx = parse_binary(parser, rbp);
    if (rhs_expr_idx < 0)
      return -1;

    int expr_idx = expr_arena_alloc(parser->expr_arena);

    Expr *expr = expr_arena_get(parser->expr_arena, expr_idx);
    expr->type = EXPR_BINARY;
    expr->binary.op = op;
    expr->binary.expr_lhs = lhs_expr_idx;
    expr->binary.expr_rhs = rhs_expr_idx;
    lhs_expr_idx = expr_idx;
  }

  return lhs_expr_idx;
}

int parse_primary(Parser *parser) {
  int expr_idx = expr_arena_alloc(parser->expr_arena);
  Expr *expr = expr_arena_get(parser->expr_arena, expr_idx);

  Token *tok = pop_next_token(parser);
  switch (tok->type) {
  case TOKEN_NUM: {
    parse_number(expr, tok);
    break;
  };
  case TOKEN_PLUS: {
    int e = parse_primary(parser);
    if (e < 0)
      return -1;

    expr->type = EXPR_UNARY;
    expr->unary.op = OP_POSITIVE;
    expr->unary.expr = e;
    break;
  };
  case TOKEN_MINUS: {
    int e = parse_primary(parser);
    if (e < 0)
      return -1;

    expr->type = EXPR_UNARY;
    expr->unary.op = OP_NEGATIVE;
    expr->unary.expr = e;
    break;
  };
  case TOKEN_LPAREN: {
    expr_idx = parse_binary(parser, 0);
    if (expr_idx < 0)
      return -1;

    Token *t = expect_token(parser, TOKEN_RPAREN);
    if (!t)
      return -1;

    pop_next_token(parser); // pop rparen
    break;
  };
  default: {
    print_error(parser, tok->pos, "Unexpected token");
    return -1;
  }
  }

  return expr_idx;
}

int parse(Parser *parser) { return parse_binary(parser, 0); }

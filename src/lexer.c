#include <ctype.h>
#include <stdio.h>

#include "arena.h"
#include "lexer.h"

const char *token_type_name(TokenType t) {
  switch (t) {
#define X(name)                                                                \
  case TOKEN_##name:                                                           \
    return #name;
    TOKEN_LIST
#undef X
  default:
    return "UNKNOWN";
  }
}

void print_token(Token *t) {
  printf("    Token { type=%s, token='%.*s' }\n", token_type_name(t->type),
         t->len, t->pos);
}

void lex_number(Lexer *lex, Token *tok) {
  tok->len = 0;
  tok->pos = lex->cur_pos;
  tok->type = TOKEN_NUM;

  while (isdigit(*lex->cur_pos) || *lex->cur_pos == '.') {
    tok->len++;
    lex->cur_pos++;
  }
}

Token next_token(Lexer *lex) {
  char *ch = lex->cur_pos;
  Token tok = {};

  while (*ch == ' ' || *ch == '\t' || *ch == '\n' || *ch == '\r') {
    // skip whitespace
    ch = ++lex->cur_pos;
  }

  tok.pos = ch;
  tok.len = 1;
  if (*ch == '\0') {
    tok.type = TOKEN_EOF;
    ++lex->cur_pos;
    return tok;
  }

  if (*ch >= '0' && *ch <= '9') {
    lex_number(lex, &tok);
    return tok;
  }

  switch (*ch) {
  case '(': {
    tok.type = TOKEN_LPAREN;
    ++lex->cur_pos;
    break;
  };
  case ')': {
    tok.type = TOKEN_RPAREN;
    ++lex->cur_pos;
    break;
  };
  case '+': {
    tok.type = TOKEN_PLUS;
    ++lex->cur_pos;
    break;
  };
  case '-': {
    tok.type = TOKEN_MINUS;
    ++lex->cur_pos;
    break;
  };
  case '*': {
    tok.type = TOKEN_STAR;
    ++lex->cur_pos;
    break;
  };
  case '/': {
    tok.type = TOKEN_SLASH;
    ++lex->cur_pos;
    break;
  };
  case '%': {
    tok.type = TOKEN_MOD;
    ++lex->cur_pos;
    break;
  };
  case '^': {
    tok.type = TOKEN_CARET;
    ++lex->cur_pos;
    break;
  };
  default: {
    tok.type = TOKEN_ILLEGAL;
  }
  }
  return tok;
}

int tokenize(Lexer *lex, TokenArray *tokens) {
  while (1) {
    Token tok = next_token(lex);
    if (tok.type == TOKEN_ILLEGAL) {
      token_array_free(tokens);
      int idx = lex->cur_pos - lex->expr_str;
      printf("\nERROR TOKENIZING:\n");
      printf("%s\n", lex->expr_str);
      printf("%*s^ Unexpected token '%c'\n", idx, "", *lex->cur_pos);
      return 1;
    }

    token_array_append(tokens, tok);
    if (tok.type == TOKEN_EOF) {
      break;
    }
  }
  return 0;
}

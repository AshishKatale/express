#ifndef LEXER_H
#define LEXER_H 1

#define TOKEN_LIST                                                             \
  X(ILLEGAL)                                                                   \
  X(EOF)                                                                       \
  X(PLUS)                                                                      \
  X(MINUS)                                                                     \
  X(STAR)                                                                      \
  X(MOD)                                                                       \
  X(SLASH)                                                                     \
  X(CARET)                                                                     \
  X(LPAREN)                                                                    \
  X(RPAREN)                                                                    \
  X(NUM)

typedef enum {
#define X(name) TOKEN_##name,
  TOKEN_LIST
#undef X
} TokenType;

const char *token_type_name(TokenType);

typedef struct Token {
  TokenType type;
  int len;
  char *pos;
} Token;

typedef struct TokenArray TokenArray;

typedef struct Lexer {
  char *expr_str;
  char *cur_pos;
} Lexer;

int tokenize(Lexer *, TokenArray *);
Token next_token(Lexer *);
void print_token(Token *);
void lex_number(Lexer *, Token *);
void lex_indentifier_or_keyword(Lexer *, Token *);

#endif // !LEXER_H

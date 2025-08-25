#ifndef REPL_H
#define REPL_H
#include "parser.h"

#define FLAG_RESULT 0x1
#define FLAG_TOKENS 0x2
#define FLAG_AST 0x4

typedef enum {
  CMD_NONE,
  CMD_INVALID,
  CMD_QUIT,
  CMD_HELP,
  CMD_RESULT,
  CMD_TOKEN,
  CMD_AST,
} CMD;

typedef struct {
  CMD cmd;
  const char *cmd_str;
  const char *cmd_help;
} CmdInfo;

CMD check_slash_cmd(char *);
int parse_and_eval_expression(char *, int);
void print_repl_help();
void start_repl();
void repl_free();

#endif // !REPL_H

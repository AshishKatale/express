#ifndef REPL_H
#define REPL_H

#include "parser.h"
typedef enum {
  CMD_NONE,
  CMD_INVALID,
  CMD_QUIT,
  CMD_HELP,
} CMD;

typedef struct {
  CMD cmd;
  const char *cmd_str;
  const char *cmd_help;
} CmdInfo;

CMD check_slash_cmd(char *);
int parse_and_eval(char *expr_str, double *, ExprArena *, int *, TokenArray *);
int parse_and_eval_expression(char *);
int eval_expression(char *);
void print_help();
void start_repl();

#endif // !REPL_H

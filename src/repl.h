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
  CMD_PRECISION,
  CMD_RESULT,
  CMD_TOKEN,
  CMD_AST,
} CMD;

typedef struct {
  CMD cmd;
  const char *cmd_str;
  const char *cmd_help;
} CmdInfo;

typedef struct {
  CMD cmd;
  const char *cmd_args;
} Command;

int parse_and_eval_expression(char *, int);
Command repl_check_slash_cmd(char *);
void repl_print_help();
void repl_start();
void repl_free();

#endif // !REPL_H

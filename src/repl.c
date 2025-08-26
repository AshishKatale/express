#include <stdio.h>
#include <stdlib.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"

static ExprArena *expr_arena = NULL;
static TokenArray *token_array = NULL;

void repl_free() {
  if (token_array)
    token_array_free(token_array);
  if (expr_arena)
    expr_arena_free(expr_arena);
}

static const CmdInfo cmd_lookup[] = {
    {.cmd = CMD_HELP, .cmd_str = "?", .cmd_help = "show this help message"},
    {.cmd = CMD_HELP, .cmd_str = "h", .cmd_help = "alternative to \\?"},
    {.cmd = CMD_QUIT, .cmd_str = "q", .cmd_help = "quit this repl"},
    {.cmd = CMD_RESULT, .cmd_str = "r", .cmd_help = "toggle show result"},
    {.cmd = CMD_AST, .cmd_str = "a", .cmd_help = "toggle show AST"},
    {.cmd = CMD_TOKEN, .cmd_str = "l", .cmd_help = "toggle show lexer tokens"},
    {.cmd = CMD_PRECISION,
     .cmd_str = "p",
     .cmd_help = "set precision for numbers"},
};

void repl_print_help() {
  printf("Usage:\n");
  for (int i = 0; i < (int)(sizeof(cmd_lookup) / sizeof(cmd_lookup[0])); ++i) {
    printf("    \\%s     %s\n", cmd_lookup[i].cmd_str, cmd_lookup[i].cmd_help);
  }
}

Command repl_check_slash_cmd(char *cmd_str) {
  Command c;
  if (*cmd_str != '\\') {
    c.cmd = CMD_NONE;
    return c;
  }

  cmd_str++; // skip '\'
  for (int i = 0; i < (int)(sizeof(cmd_lookup) / sizeof(cmd_lookup[0])); ++i) {
    if (*cmd_str == *cmd_lookup[i].cmd_str) {
      cmd_str++; // skip cmd letter
      while (*cmd_str == ' ' || *cmd_str == '\t') {
        cmd_str++; // skip whitespace after cmd and before args
      }
      c.cmd = cmd_lookup[i].cmd;
      c.cmd_args = cmd_str;
      return c;
    }
  }

  c.cmd = CMD_INVALID;
  return c;
}

int parse_and_eval_expression(char *expr_str, int flags) {
  if (*expr_str == '\0') {
    printf("ERROR: empty expression\n");
    return 1;
  }

  Lexer lexer = {.expr_str = expr_str, .cur_pos = expr_str};
  if (!token_array) {
    token_array = token_array_init();
  }
  int failed = tokenize(&lexer, token_array);
  if (failed) {
    token_array_reset(token_array);
    return 1;
  }
  if ((flags & FLAG_TOKENS) == FLAG_TOKENS) {
    printf("\n");
    token_array_print(token_array);
  }

  if (!expr_arena) {
    expr_arena = expr_arena_init();
  }
  Parser parser = {
      .token_array = token_array, .expr_arena = expr_arena, .idx = 0};
  int expr_idx = parse(&parser);
  if (expr_idx < 0) {
    token_array_reset(token_array);
    expr_arena_reset(expr_arena);
    return 1;
  }
  if ((flags & FLAG_AST) == FLAG_AST) {
    printf("\nAST:");
    print_expr(expr_arena, expr_idx);
  }

  double result = evaluate_expr(expr_arena, expr_idx);
  if ((flags & FLAG_RESULT) == FLAG_RESULT) {
    printf("%s = %0.*lf\n", expr_str, get_precision(), result);
  }

  token_array_reset(token_array);
  expr_arena_reset(expr_arena);
  return 0;
}

void repl_start() {
  repl_print_help();

  char *line;
  char *prompt = "\nexpression >> ";
  int flags = FLAG_RESULT;
  while ((line = readline(prompt)) != NULL) {
    if (*line) {
      add_history(line);
    }

    char *line_ws_trimmed = line;
    while (*line_ws_trimmed == ' ' || *line_ws_trimmed == '\t') {
      line_ws_trimmed++;
    }

    Command command = repl_check_slash_cmd(line_ws_trimmed);
    if (command.cmd == CMD_INVALID) {
      printf("ERROR: invalid command '%s'\n", line_ws_trimmed);
      free(line);
      continue;
    } else if (command.cmd == CMD_HELP) {
      repl_print_help();
      free(line);
      continue;
    } else if (command.cmd == CMD_PRECISION) {
      set_precision(command.cmd_args);
      printf("Precision set: %d\n", get_precision());
      free(line);
      continue;
    } else if (command.cmd == CMD_QUIT) {
      free(line);
      break;
    } else if (command.cmd == CMD_RESULT) {
      flags ^= FLAG_RESULT;
      printf("Show result: %s\n", (flags & FLAG_RESULT) > 0 ? "on" : "off");
      continue;
    } else if (command.cmd == CMD_TOKEN) {
      flags ^= FLAG_TOKENS;
      printf("Show tokens: %s\n", (flags & FLAG_TOKENS) > 0 ? "on" : "off");
      continue;
    } else if (command.cmd == CMD_AST) {
      flags ^= FLAG_AST;
      printf("Show AST: %s\n", (flags & FLAG_AST) > 0 ? "on" : "off");
      continue;
    }

    parse_and_eval_expression(line_ws_trimmed, flags);

    free(line);
  }

  repl_free();
  clear_history();
}

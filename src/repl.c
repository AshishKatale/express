#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include "repl.h"

static const CmdInfo cmds[] = {
    {.cmd = CMD_QUIT, .cmd_str = "\\q", .cmd_help = "quit this repl"},
    {.cmd = CMD_HELP, .cmd_str = "\\h", .cmd_help = "show this help message"},
};

void print_help() {
  printf("\033[33;1;1mUsage:\033[0m\n");
  for (int i = 0; i < (int)(sizeof(cmds) / sizeof(cmds[0])); ++i) {
    printf("  \033[34;1;1m%s\033[0m     %s\n", cmds[i].cmd_str,
           cmds[i].cmd_help);
  }
}

CMD check_slash_cmd(char *cmd) {
  if (*cmd != '\\') {
    return CMD_NONE;
  }

  for (int i = 0; i < (int)(sizeof(cmds) / sizeof(cmds[0])); ++i) {
    if (strcmp(cmds[i].cmd_str, cmd) == 0) {
      return cmds[i].cmd;
    }
  }

  return CMD_INVALID;
}

int parse_and_eval_expression(char *expr_str) {
  if (*expr_str == '\0') {
    printf("ERROR: empty expression\n");
    return 1;
  }

  Lexer lexer = {.expr_str = expr_str, .cur_pos = expr_str};
  TokenArray *tokens = token_array_init();
  tokenize(&lexer, tokens);
  if (!tokens) {
    return 1;
  }
  printf("\n");
  token_array_print(tokens);

  ExprArena *expr_arena = expr_arena_init();
  Parser parser = {.token_array = tokens, .expr_arena = expr_arena, .index = 0};
  int expr_idx = parse(&parser);
  if (expr_idx < 0) {
    return 1;
  }
  printf("\nAST:");
  print_expr(expr_arena, expr_idx);

  double result = evaluate_expr(expr_arena, expr_idx);
  printf("\nResult:\n%s = %lf\n", expr_str, result);

  token_array_free(tokens);
  expr_arena_free(expr_arena);
  return 0;
}

void start_repl() {
  print_help();

  char *line;
  char *prompt = "\n\033[36;1;1mexpression\033[32;1;1m >>\033[0m ";
  while ((line = readline(prompt)) != NULL) {
    if (*line) {
      add_history(line);
    }

    char *line_ws_trimmed = line;
    while (*line_ws_trimmed == ' ' || *line_ws_trimmed == '\t') {
      line_ws_trimmed++;
    }

    CMD cmd = check_slash_cmd(line_ws_trimmed);
    if (cmd == CMD_INVALID) {
      printf("ERROR: invalid command '%s'\n", line_ws_trimmed);
      print_help();
      free(line);
      continue;
    } else if (cmd == CMD_HELP) {
      print_help();
      free(line);
      continue;
    } else if (cmd == CMD_QUIT) {
      free(line);
      break;
    }

    parse_and_eval_expression(line_ws_trimmed);

    free(line);
  }

  clear_history();
}

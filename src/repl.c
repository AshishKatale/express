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
    {.cmd = CMD_RESULT, .cmd_str = "\\r", .cmd_help = "toggle show result"},
    {.cmd = CMD_AST, .cmd_str = "\\a", .cmd_help = "toggle show AST"},
    {.cmd = CMD_TOKEN,
     .cmd_str = "\\l",
     .cmd_help = "toggle show lexer tokens"},
};

void print_repl_help() {
  printf("Usage:\n");
  for (int i = 0; i < (int)(sizeof(cmds) / sizeof(cmds[0])); ++i) {
    printf("    %s     %s\n", cmds[i].cmd_str, cmds[i].cmd_help);
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

int parse_and_eval_expression(char *expr_str, int flags) {
  if (*expr_str == '\0') {
    printf("ERROR: empty expression\n");
    return 1;
  }

  Lexer lexer = {.expr_str = expr_str, .cur_pos = expr_str};
  TokenArray *tokens = token_array_init();
  int failed = tokenize(&lexer, tokens);
  if (failed) {
    token_array_free(tokens);
    return 1;
  }
  if ((flags & FLAG_TOKENS) == FLAG_TOKENS) {
    printf("\n");
    token_array_print(tokens);
  }

  ExprArena *expr_arena = expr_arena_init();
  Parser parser = {.token_array = tokens, .expr_arena = expr_arena, .index = 0};
  int expr_idx = parse(&parser);
  if (expr_idx < 0) {
    expr_arena_free(expr_arena);
    return 1;
  }
  if ((flags & FLAG_AST) == FLAG_AST) {
    printf("\nAST:");
    print_expr(expr_arena, expr_idx);
  }

  double result = evaluate_expr(expr_arena, expr_idx);
  if ((flags & FLAG_RESULT) == FLAG_RESULT) {
    printf("%s = %lf\n", expr_str, result);
  }

  token_array_free(tokens);
  expr_arena_free(expr_arena);
  return 0;
}

void start_repl() {
  print_repl_help();

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

    CMD cmd = check_slash_cmd(line_ws_trimmed);
    if (cmd == CMD_INVALID) {
      printf("ERROR: invalid command '%s'\n", line_ws_trimmed);
      print_repl_help();
      free(line);
      continue;
    } else if (cmd == CMD_HELP) {
      print_repl_help();
      free(line);
      continue;
    } else if (cmd == CMD_QUIT) {
      free(line);
      break;
    } else if (cmd == CMD_RESULT) {
      flags ^= FLAG_RESULT;
      printf("Show result: %s\n", (flags & FLAG_RESULT) > 0 ? "on" : "off");
      continue;
    } else if (cmd == CMD_TOKEN) {
      flags ^= FLAG_TOKENS;
      printf("Show tokens: %s\n", (flags & FLAG_TOKENS) > 0 ? "on" : "off");
      continue;
    } else if (cmd == CMD_AST) {
      flags ^= FLAG_AST;
      printf("Show AST: %s\n", (flags & FLAG_AST) > 0 ? "on" : "off");
      continue;
    }

    parse_and_eval_expression(line_ws_trimmed, flags);

    free(line);
  }

  clear_history();
}

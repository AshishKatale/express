#include <stdio.h>
#include <unistd.h>

#include "arena.h"
#include "repl.h"

void print_cli_help(FILE *output_fd) {
  char *help_msg = "Usage:"
                   "\n    -h        show this help message"
                   "\n    -i        start interactive repl mode"
                   "\n    -t  -l    show lexer tokens"
                   "\n    -a        show ast for expression\n";
  fprintf(output_fd, "%s", help_msg);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    print_cli_help(stderr);
    return 1;
  }

  int flags = 1;
  int opt;
  while ((opt = getopt(argc, argv, "hialt")) != -1) {
    switch (opt) {
    case 'i':
      start_repl();
      return 0;
    case 'h':
      print_cli_help(stdout);
      return 0;
    case 'a':
      flags |= FLAG_AST;
      break;
    case 'l':
    case 't':
      flags |= FLAG_TOKENS;
      break;
    default: {
      print_cli_help(stderr);
      return 1;
    }
    }
  }

  if (argc <= optind) {
    fprintf(stderr,
            "ERROR: provide an expression to evaluate as an argument\n");
    fprintf(stderr, "or start interactive repl with '-i' flag\n\n");
    print_cli_help(stderr);
    return 1;
  }

  flags |= FLAG_RESULT;
  int exit = parse_and_eval_expression(argv[argc - 1], flags);
  repl_free();
  return exit;
}

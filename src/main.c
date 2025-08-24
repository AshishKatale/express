#include "arena.h"
#include "repl.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    return parse_and_eval_expression(argv[1]);
  }

  start_repl();
  return 0;
}

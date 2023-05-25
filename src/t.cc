#include <cmd_args/cmd_args.h>

#include "utility_cli/utility_cli.h"

using s21::UtilityCLI;

int main(int argc, const char* argv[]) {
  int exit_code = 1;
  UtilityCLI utility(argc, argv);
  utility.Exec();

  return exit_code;
}
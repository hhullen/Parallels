#ifndef SRC_UTILITY_CLI_UTILITY_CLI_H_
#define SRC_UTILITY_CLI_UTILITY_CLI_H_

#include <cmd_args/cmd_args.h>
#include <sle_algorithm/sle.h>

#include <iostream>
#include <list>
#include <map>
#include <string>

using hhullen::Argument;
using hhullen::CMDArgs;
using hhullen::Flag;
using std::atol;

namespace s21 {

using Str = std::string;

using std::atoi;
using std::cout;
using std::invalid_argument;
using std::isdigit;
using std::map;
using Str = std::string;
using FlagValues = std::list<Str>;

class UtilityCLI {
 public:
  UtilityCLI() {}
  UtilityCLI(int argc, const char* argv[]);
  void Exec();

 private:
  map<std::string, void (UtilityCLI::*)()> algorithms_runners_;
  CMDArgs command_line_;

  void InitializeAlgorithms();

  void RunACO();
  void RunSLE();
  void RunWinograd();

  void WriteOutFile();
};

}  // namespace s21

#endif  // SRC_UTILITY_CLI_UTILITY_CLI_H_

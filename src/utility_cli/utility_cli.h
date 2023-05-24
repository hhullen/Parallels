#ifndef SRC_UTILITY_CLI_UTILITY_CLI_H_
#define SRC_UTILITY_CLI_UTILITY_CLI_H_

#include <cmd_args/cmd_args.h>

#include <iostream>
#include <map>
#include <string>

using hhullen::Argument;
using hhullen::CMDArgs;
using hhullen::Flag;

namespace s21 {

using s21::Graph;
using s21::GraphAlgorithms;
using std::atoi;
using std::cout;
using std::invalid_argument;
using std::isdigit;
using std::map;
using std::string;

class UtilityCLI {
 public:
  UtilityCLI() = delete;
  UtilityCLI(int argc, char* argv[]);
  void Exec();

 private:
  map<string, void (UtilityCLI::*)()> algorithms_runners_;
  CMDArgs command_line_;

  void InitializeAlgorithms();
  void InitializeInput();

  void RunAlgorithm();
  void ACO();
  void SLE();
  void Winograd();

  void WriteOutFile();
};

}  // namespace s21

#endif  // SRC_UTILITY_CLI_UTILITY_CLI_H_

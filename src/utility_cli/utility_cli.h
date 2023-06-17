#ifndef SRC_UTILITY_CLI_UTILITY_CLI_H_
#define SRC_UTILITY_CLI_UTILITY_CLI_H_

#include <cmd_args/cmd_args.h>
#include <sle_algorithm/sle.h>
#include <timer/timer.h>

#include <iostream>
#include <list>
#include <map>
#include <string>

using hhullen::Argument;
using hhullen::CMDArgs;
using hhullen::DTime;
using hhullen::Flag;
using hhullen::Timer;
using std::atol;

namespace s21 {

using std::atoi;
using std::cout;
using std::invalid_argument;
using std::isdigit;
using std::map;
using Str = std::string;
using FlagValues = std::list<Str>;

class UtilityCLI {
 public:
  UtilityCLI() = delete;
  UtilityCLI(const UtilityCLI& other) = delete;
  UtilityCLI(const UtilityCLI&& other) = delete;
  UtilityCLI(int argc, const char* argv[]);

  void Exec();

 private:
  map<std::string, void (UtilityCLI::*)()> algorithms_runners_;
  CMDArgs command_line_;
  size_t repeats_;
  Str file_path_;
  Timer timer_;

  void InitializeAlgorithms();

  void RunACO();
  void RunSLE();
  void RunWinograd();

  void WriteOutFile();
  void StopAndReportTimer(const Str& message);
  void ReadCMDArguments();
};

}  // namespace s21

#endif  // SRC_UTILITY_CLI_UTILITY_CLI_H_

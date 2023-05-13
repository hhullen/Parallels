#ifndef SRC_UTILITY_CLI_UTILITY_CLI_H_
#define SRC_UTILITY_CLI_UTILITY_CLI_H_

#include <iostream>
#include <map>
#include <string>

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
  map<string, string> arguments_;

  void InitialiseAlgorithms();
  void ReadArguments(int argc, char* argv[]);
  bool IsOption(const string& arg);
  void CheckNextPresence(int i, int argc, const string& arg);

  void InitializeInput();
  string GetOptionParameterIfExists(string option,
                                    string exception_message = "");

  void RunAlgorithm();
  void ACO();
  void SLE();
  void Winograd();

  void WriteOutFile();
};

}  // namespace s21

#endif  // SRC_UTILITY_CLI_UTILITY_CLI_H_

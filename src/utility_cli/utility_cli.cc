#include "utility_cli.h"

namespace s21 {

UtilityCLI::UtilityCLI(int argc, char *argv[]) {
  ReadArguments(argc, argv);
  InitialiseAlgorithms();
}

void UtilityCLI::InitialiseAlgorithms() {
  algorithms_runners_["ACO"] = &UtilityCLI::ACO;
  algorithms_runners_["SLE"] = &UtilityCLI::SLE;
  algorithms_runners_["Winograd"] = &UtilityCLI::Winograd;
}

void UtilityCLI::ReadArguments(int argc, char *argv[]) {
  for (int i = 0; i < argc; ++i) {
    string arg(argv[i]);
    if (IsOption(arg)) {
      CheckNextPresence(i, argc, arg);
      arguments_[arg] = string(argv[i + 1]);
    }
  }
}

bool UtilityCLI::IsOption(const string &arg) {
  return arg.size() == 2 && arg[0] == '-' && !isdigit(arg[1]);
}

void UtilityCLI::CheckNextPresence(int i, int argc, const string &arg) {
  if (!(i + 1 < argc)) {
    throw invalid_argument("No parameter specified to " + arg + " option.");
  }
}

void UtilityCLI::Exec() {
  InitializeInput();
  RunAlgorithm();
  WriteOutFile();
}

void UtilityCLI::InitializeInput() {
  string file_path = GetOptionParameterIfExists(
      "-f",
      "No input graph file specified. For instance: ... -f path/to/file.");
  graph_.LoadGraphFromFile(file_path);
}

string UtilityCLI::GetOptionParameterIfExists(string option,
                                              string exception_message) {
  if (arguments_.find(option) == arguments_.end()) {
    throw invalid_argument(exception_message);
  }

  return arguments_.find(option)->second;
}

void UtilityCLI::RunAlgorithm() {
  string mode;
  try {
    mode = GetOptionParameterIfExists("-m");
  } catch (...) {
    return;
  }

  if (algorithms_runners_.find(mode) == algorithms_runners_.end()) {
    throw invalid_argument("No algorithm named " + mode + ".");
  }
  (this->*algorithms_runners_[mode])();
}

void UtilityCLI::ACO() {}

void UtilityCLI::SLE() {}

void UtilityCLI::Winograd() {}

void UtilityCLI::WriteOutFile() {
  string file_path;
  try {
    file_path = GetOptionParameterIfExists("-o");
  } catch (...) {
    return;
  }
  graph_.ExportGraphToDot(file_path);
}

}  // namespace s21

#include "utility_cli.h"

namespace s21 {

UtilityCLI::UtilityCLI(int argc, char *argv[]) {
  Argument algorithm = Argument("algorithm", Argument::Type::Str, "");
  command_line_.AddArguments({algorithm});
  command_line_.Read(argc, argv);
  InitializeAlgorithms();
}

void UtilityCLI::InitializeAlgorithms() {
  algorithms_runners_["ACO"] = &UtilityCLI::ACO;
  algorithms_runners_["SLE"] = &UtilityCLI::SLE;
  algorithms_runners_["Winograd"] = &UtilityCLI::Winograd;
}

void UtilityCLI::Exec() {
  InitializeInput();
  RunAlgorithm();
  WriteOutFile();
}

void UtilityCLI::InitializeInput() {}

void UtilityCLI::RunAlgorithm() {
  string mode;
  try {
    // mode = GetOptionParameterIfExists("-m");
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
    // file_path = GetOptionParameterIfExists("-o");
  } catch (...) {
    return;
  }
  graph_.ExportGraphToDot(file_path);
}

}  // namespace s21

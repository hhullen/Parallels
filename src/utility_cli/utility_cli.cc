#include "utility_cli.h"

namespace s21 {

UtilityCLI::UtilityCLI(int argc, const char *argv[]) {
  Argument algorithm = Argument("algorithm", Argument::Type::Str, "");

  Argument file_path = Argument("path", Argument::Type::Path, "");
  Flag input_file_flag = Flag("file", 'f', "", {file_path});

  Argument repeats = Argument("repeats", Argument::Type::Int, "");
  Flag repeats_flag = Flag("num-repeats", 'n', "", {repeats});

  command_line_.AddArguments({algorithm});
  command_line_.AddFlags({input_file_flag, repeats_flag});
  command_line_.Read(argc, argv);
  InitializeAlgorithms();
}

void UtilityCLI::InitializeAlgorithms() {
  algorithms_runners_["ACO"] = &UtilityCLI::ACO;
  algorithms_runners_["SLE"] = &UtilityCLI::SLE;
  algorithms_runners_["WNG"] = &UtilityCLI::Winograd;
}

void UtilityCLI::Exec() {
  Str algorithm = command_line_.GetArgument("algorithm");
  if (algorithms_runners_.find(algorithm) == algorithms_runners_.end()) {
    throw std::invalid_argument("Unknown algorithm specified: " + algorithm);
  }
  (this->*algorithms_runners_[algorithm])();
}

void UtilityCLI::ACO() {
  cout << "ACO RUN\n";
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  cout << file.front() << " input file\n";
  cout << repeats.front() << " repeats\n";
}

void UtilityCLI::SLE() {
  cout << "SLE RUN\n";
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  cout << file.front() << " input file\n";
  cout << repeats.front() << " repeats\n";
}

void UtilityCLI::Winograd() {
  cout << "Winograd RUN\n";
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  cout << file.front() << " input file\n";
  cout << repeats.front() << " repeats\n";
}

void UtilityCLI::WriteOutFile() {
  // Str file_path;
  // try {
  // file_path = GetOptionParameterIfExists("-o");
  // } catch (...) {
  //   return;
  // }
  // graph_.ExportGraphToDot(file_path);
}

}  // namespace s21

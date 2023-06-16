#include "utility_cli.h"

namespace s21 {

UtilityCLI::UtilityCLI(int argc, const char *argv[]) {
  Argument algorithm("algorithm", Argument::Type::Str, "");

  Argument file_path("path", Argument::Type::Path, "");
  Flag input_file_flag("file", 'f', "", {file_path});

  Argument repeats("repeats", Argument::Type::UInt, "");
  Flag repeats_flag("num-repeats", 'n', "", {repeats});

  command_line_.AddArguments({algorithm});
  command_line_.AddFlags({input_file_flag, repeats_flag});
  command_line_.Read(argc, argv);
  InitializeAlgorithms();
}

void UtilityCLI::InitializeAlgorithms() {
  algorithms_runners_["ACO"] = &UtilityCLI::RunACO;
  algorithms_runners_["SLE"] = &UtilityCLI::RunSLE;
  algorithms_runners_["WNG"] = &UtilityCLI::RunWinograd;
}

void UtilityCLI::Exec() {
  Str algorithm = command_line_.GetArgument("algorithm");
  if (algorithms_runners_.find(algorithm) == algorithms_runners_.end()) {
    throw std::invalid_argument("Unknown algorithm specified: " + algorithm);
  }
  (this->*algorithms_runners_[algorithm])();
}

void UtilityCLI::RunACO() {
  cout << "ACO RUN\n";
  timer_.Reset();
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  cout << file.front() << " input file\n";
  cout << repeats.front() << " repeats\n";
  StopAndReportTimer("ACO Finished");
}

void UtilityCLI::RunSLE() {
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  Str file_path = file.front();
  size_t repeat = atol(repeats.front().data());
  SLE runner;
  runner.Load(file_path);

  timer_.Reset();
  for (size_t i = 0; i < repeat; ++i) {
    runner.SolveUsual();
  }
  StopAndReportTimer("ACO usual Finished");

  timer_.Reset();
  for (size_t i = 0; i < repeat; ++i) {
    runner.SolveParallel();
  }
  StopAndReportTimer("ACO parallel Finished");
}

void UtilityCLI::RunWinograd() {
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

void UtilityCLI::StopAndReportTimer(const Str &message) {
  DTime dt = timer_.Elapsed();
  cout << (message + ": " + dt.SHours() + ":" + dt.SMin() + ":" + dt.SSec() +
           "." + dt.SMs(3) + "\n");
}

}  // namespace s21

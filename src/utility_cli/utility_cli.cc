#include "utility_cli.h"

namespace s21 {

UtilityCLI::UtilityCLI(int argc, const char *argv[])
    : file_path_(""), repeats_(0) {
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
  ReadCMDArguments();
  cout << file_path_ << " input file\n";
  cout << repeats_ << " repeats\n";
  StopAndReportTimer("ACO Finished");
}

void UtilityCLI::RunSLE() {
  ReadCMDArguments();
  SLE runner;
  runner.Load(file_path_);

  timer_.Reset();
  for (size_t i = 0; i < repeats_; ++i) {
    runner.Load(file_path_);
    runner.SolveUsual();
  }
  double usual_time = StopAndReportTimer("SLE usual Finished");

  timer_.Reset();
  for (size_t i = 0; i < repeats_; ++i) {
    runner.Load(file_path_);
    runner.SolveParallel();
  }
  double parallel_time = StopAndReportTimer("SLE parallel Finished");
  std::cout << "Usual in " << usual_time / parallel_time
            << " longer than Parallel\n";
}

void UtilityCLI::RunWinograd() {
  cout << "Winograd RUN\n";
  ReadCMDArguments();
  cout << file_path_ << " input file\n";
  cout << repeats_ << " repeats\n";
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

double UtilityCLI::StopAndReportTimer(const Str &message) {
  DTime dt = timer_.Elapsed();
  cout << (message + ": " + dt.SHours() + ":" + dt.SMin() + ":" + dt.SSec() +
           "." + dt.SMs(3) + "\n");
  return static_cast<double>(dt.InMs());
}

void UtilityCLI::ReadCMDArguments() {
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  file_path_ = file.front();
  repeats_ = atol(repeats.front().data());
}

}  // namespace s21

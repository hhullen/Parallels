#include "utility_cli.h"

namespace s21 {

UtilityCLI::UtilityCLI(int argc, const char *argv[])
    : repeats_(0), threads_(2) {
  Argument algorithm("algorithm", Argument::Type::String);

  Argument file_path("path", Argument::Type::Path);
  Flag input_file_flag("file", 'f', "path to file", {file_path});

  Argument repeats("repeats", Argument::Type::UInt);
  Flag repeats_flag("num-repeats", 'n', "repeats number", {repeats});

  Argument threads("threads", Argument::Type::UInt);
  Flag threads_flag("num-threads", 't', "threads number", {threads});

  command_line_.AddArguments({algorithm});
  command_line_.AddFlags({input_file_flag, repeats_flag, threads_flag});
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
  ReadCMDArguments();
  TSPAlgorithm runner;
  Graph graph;

  timer_.Reset();
  for (int i = 0; i < repeats_; ++i) {
    graph.LoadGraphFromFile(file_path_);
    runner.Solve(graph);
  }
  double usual_time = StopAndReportTimer("ACO usual Finished");

  timer_.Reset();
  for (int i = 0; i < repeats_; ++i) {
    graph.LoadGraphFromFile(file_path_);
    runner.Solve(graph);
  }
  double parallel_time = StopAndReportTimer("ACO parallel Finished");
  ReportRatio(usual_time, parallel_time);
}

void UtilityCLI::RunSLE() {
  ReadCMDArguments();
  SLE runner;
  runner.SetThreads(threads_);
  runner.Load(file_path_);

  timer_.Reset();
  for (int i = 0; i < repeats_; ++i) {
    runner.Load(file_path_);
    runner.SolveUsual();
  }
  double usual_time = StopAndReportTimer("SLE usual Finished");

  timer_.Reset();
  for (int i = 0; i < repeats_; ++i) {
    runner.Load(file_path_);
    runner.SolveParallel();
  }
  double parallel_time = StopAndReportTimer("SLE parallel Finished");
  ReportRatio(usual_time, parallel_time);
}

void UtilityCLI::RunWinograd() {
  cout << "Winograd RUN\n";
  ReadCMDArguments();
  cout << file_path_ << " input file\n";
  cout << repeats_ << " repeats\n";
}

double UtilityCLI::StopAndReportTimer(const Str &message) {
  DTime dt = timer_.Elapsed();
  cout << (message + ": " + dt.SHours() + ":" + dt.SMin() + ":" + dt.SSec() +
           "." + dt.SMs(3) + "\n");
  return static_cast<double>(dt.InMs());
}

void UtilityCLI::ReportRatio(const double usual_time,
                             const double parallel_time) {
  std::cout << "Parallel in " << usual_time / parallel_time
            << " times faster\n";
}

void UtilityCLI::ReadCMDArguments() {
  FlagValues file = command_line_.GetFlagValues("--file");
  FlagValues repeats = command_line_.GetFlagValues("--num-repeats");
  file_path_ = file.front();
  repeats_ = static_cast<int>(atol(repeats.front().data()));
  try {
    FlagValues threads = command_line_.GetFlagValues("--num-threads");
    threads_ = static_cast<int>(atol(threads.front().data()));
  } catch (...) {
    threads_ = 2;
  }
}

}  // namespace s21

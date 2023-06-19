#include "sle.cc"

int main() {
  s21::SLE runner;
  runner.Load(
      "/Users/hhullen/PROJECTS/github/Parallels/src/tests/dataset/"
      "matrix_correct_1.txt");
  // runner.SolveUsual();
  runner.SolveParallel();
  return 0;
}

#include "sle.cc"

int main() {
  s21::SLE runner;
  runner.Load("../../tests/dataset/marix_correct.txt");
  runner.SolveUsual();
  // runner.SolveParallel();
  return 0;
}

#include "sle.cc"

int main() {
  s21::SLE runner;
  runner.Load("../../tests/dataset/marix_correct_1.txt");
  runner.SolveUsual();
  return 0;
}

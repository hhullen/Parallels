#include "sle.h"

namespace s21 {

SLE::SLE() {}

SLE::~SLE() {}

void SLE::Load(Str &file_path) {
  extended_.Load(file_path);
  VerifyCorrectness();
}

void SLE::Save(Str &file_path) { extended_.Save(file_path); }

void SLE::SolveUsual() {}

void SLE::SolveParallel() {}

const Matrix SLE::GetVariables() {
  int rows = extended_.get_rows();
  int cols = extended_.get_cols() - 1;
  Matrix variables(rows, cols);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      variables(i, j) = extended_(i, j);
    }
  }
  return variables;
}

void SLE::VerifyCorrectness() {
  if (extended_.get_rows() < 1 || extended_.get_cols() < 2) {
    throw Str("Attempt to load invalid extendex matrix.");
  }
}

}  // namespace s21

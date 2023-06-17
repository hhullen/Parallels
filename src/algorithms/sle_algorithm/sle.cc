#include "sle.h"

#include "solve_parallel.cc"
#include "solve_usual.cc"

namespace s21 {

SLE::SLE() {}

SLE::~SLE() {}

void SLE::Load(const Str &file_path) {
  extended_.Load(file_path);
  VerifyCorrectness();
}

void SLE::VerifyCorrectness() {
  if (extended_.get_rows() < 1 || extended_.get_cols() < 2 ||
      extended_.get_cols() - 1 != extended_.get_rows()) {
    throw Str("Attempt to load invalid extended matrix.");
  }
}

void SLE::Save(const Str &file_path) { extended_.Save(file_path); }

const Matrix SLE::GetVariables() {
  int rows = extended_.get_rows();
  Matrix variables(1, rows);
  for (int row = 0; row < rows; ++row) {
    variables(0, row) = extended_(row, rows);
  }
  return variables;
}

}  // namespace s21

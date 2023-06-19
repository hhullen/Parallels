#include "sle.h"

#include "solve_parallel.cc"
#include "solve_usual.cc"

namespace s21 {

SLE::SLE() {
  if (threads_ < 1) {
    threads_ = 1;
  }
  workers_.resize(threads_);
}

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

void SLE::SetElementToZero(const int row, const int col) {
  if (extended_(row, col) != 0) {
    // int another_row = GetAnotherRowIdxWithNonZeroElement(row, col);
    int another_row = col;
    double factor = -(extended_(row, col) / extended_(another_row, col));
    for (int i = 0; i < extended_.get_cols(); ++i) {
      extended_(row, i) += extended_(another_row, i) * factor;
    }
  }
}

// int SLE::GetAnotherRowIdxWithNonZeroElement(const int row, const int col) {
//   int new_row = row - 1;
//   if (extended_(new_row, col) == 0) {
//     bool is_found = false;
//     int i = extended_.get_rows();
//     for (; !is_found && i > 0;) {
//       --i;
//       is_found = (i != row) && (extended_(i, col) != 0);
//     }
//     if (!is_found) {
//       throw Str("No solution for the matrix: one of columns is full zero.");
//     }
//     new_row = i;
//   }
//   return new_row;
// }

}  // namespace s21

#include "sle.h"

namespace s21 {

void SLE::SolveUsual() {
  GaussForward();
  MakeUnitsDiagonally();
  GaussBackward();
}

void SLE::GaussForward() {
  for (int col = 0; col < extended_.get_cols(); ++col) {
    for (int row = extended_.get_rows() - 1; row > col; --row) {
      SetElementToZero(row, col);
    }
  }
}

void SLE::SetElementToZero(const int row, const int col) {
  int another_row = GetAnotherRowIdxWithNonZeroElement(row, col);
  double factor = -(extended_(row, col) / extended_(another_row, col));
  for (int i = 0; i < extended_.get_cols(); ++i) {
    extended_(row, i) += extended_(another_row, i) * factor;
  }
}

int SLE::GetAnotherRowIdxWithNonZeroElement(const int row, const int col) {
  int new_row = row - 1;
  if (extended_(new_row, col) == 0) {
    bool is_found = false;
    int i = extended_.get_rows();
    for (; !is_found && i > 0;) {
      --i;
      is_found = (i != row) && (extended_(i, col) != 0);
    }
    if (!is_found) {
      throw Str("No solution for the matrix: one of columns is full zero.");
    }
    new_row = i;
  }
  return new_row;
}

void SLE::MakeUnitsDiagonally() {
  for (int i = 0; i < extended_.get_rows(); ++i) {
    double factor = extended_(i, i);
    for (int j = 0; j < extended_.get_cols(); ++j) {
      extended_(i, j) /= factor;
    }
  }
}

void SLE::GaussBackward() {
  for (int col = extended_.get_rows() - 1; col >= 0; --col) {
    for (int row = col - 1; row >= 0; --row) {
      double factor = extended_(row, col);
      extended_.AddRowMultiplyedByNumberToRow(col, -factor, row);
    }
  }
}

}  // namespace s21

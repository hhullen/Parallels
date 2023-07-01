#include "sle.h"

namespace s21 {

void SLE::SolveUsual() {
  GaussForward();
  MakeUnitsDiagonally();
  GaussBackward();
}

void SLE::GaussForward() {
  int rows = extended_.get_rows() - 1;
  int cols = extended_.get_cols() - 1;
  for (int col = 0; col < cols; ++col) {
    for (int row = rows; row > col; --row) {
      SetElementToZero(row, col);
    }
  }
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

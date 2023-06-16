#include "sle.h"

namespace s21 {

SLE::SLE() {}

SLE::~SLE() {}

void SLE::Load(const Str &file_path) {
  extended_.Load(file_path);
  VerifyCorrectness();
}

void SLE::Save(const Str &file_path) { extended_.Save(file_path); }

void SLE::SolveUsual() {
  GaussForward();
  MakeUnitsDiagonally();
  extended_.Save("triangle.txt");
}

void SLE::GaussForward() {
  for (int col = 0; col < extended_.get_cols(); ++col) {
    for (int row = extended_.get_rows() - 1; row > col; --row) {
      SetElementToZero(row, col);

      // for (int i = 0; i < extended_.get_rows(); ++i) {
      //   for (int j = 0; j < extended_.get_cols(); ++j) {
      //     std::cout << extended_(i, j) << " ";
      //   }
      //   std::cout << "\n";
      // }
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
    for (; !is_found && i >= 0;) {
      --i;
      is_found = (i != row) && (extended_(i, col) != 0);
    }
    if (i < 0) {
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

void SLE::SolveParallel() {}

const Matrix SLE::GetVariables() { return Matrix(); }

void SLE::VerifyCorrectness() {
  if (extended_.get_rows() < 1 || extended_.get_cols() < 2 ||
      extended_.get_cols() - 1 != extended_.get_rows()) {
    throw Str("Attempt to load invalid extended matrix.");
  }
}

}  // namespace s21

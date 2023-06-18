#include "sle.h"

namespace s21 {

void SLE::SolveParallel() {
  GaussForwardPrl();
  // extended_.Save("thread.txt");
  MakeUnitsDiagonallyPrl();
  // extended_.Save("thread_01.txt");
  GaussBackwardPrl();
  // extended_.Save("thread_02.txt");
}

void SLE::GaussForwardPrl() {
  int rows = extended_.get_rows() - 1;
  int cols = extended_.get_cols() - 1;
  for (int col = 0; col < cols; ++col) {
    Mutex local;
    for (int row = rows; row > col; --row) {
      local.lock();
      workers_[{row, col}] =
          Thread(&SLE::SetElementToZeroThread, this, row, col, &local);
      if (workers_.size() < threads_) {
        local.unlock();
      }
    }

    for (int row = rows; row > col; --row) {
      if (!workers_[{row, col}].joinable()) {
        workers_[{row, col}].~thread();
        workers_.erase({row, col});
      } else {
        workers_[{row, col}].join();
        workers_[{row, col}].~thread();
        workers_.erase({row, col});
      }
    }
  }
}

void SLE::SetElementToZeroThread(const int row, const int col, Mutex* local) {
  SetElementToZero(row, col);
  local->unlock();
}

void SLE::MakeUnitsDiagonallyPrl() {
  for (int i = 0; i < extended_.get_rows(); ++i) {
    double factor = extended_(i, i);
    for (int j = 0; j < extended_.get_cols(); ++j) {
      extended_(i, j) /= factor;
    }
  }
}

void SLE::GaussBackwardPrl() {
  for (int col = extended_.get_rows() - 1; col >= 0; --col) {
    for (int row = col - 1; row >= 0; --row) {
      double factor = extended_(row, col);
      extended_.AddRowMultiplyedByNumberToRow(col, -factor, row);
    }
  }
}

}  // namespace s21

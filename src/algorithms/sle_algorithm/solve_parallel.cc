#include "../../include/timer/timer.h"
#include "sle.h"

namespace s21 {

void SLE::SolveParallel() {
  GaussForwardPrl();
  MakeUnitsDiagonallyPrl();
  GaussBackwardPrl();
}

void SLE::GaussForwardPrl() {
  int cols = extended_.get_cols() - 1;
  for (int col = 0; col < cols; ++col) {
    int rows = extended_.get_rows() - 1;
    if (threads_ < (rows - col)) {
      RunForwardMultithreadPerSet(rows, col);
    } else {
      RunForwardMultithreadPerLine(rows, col);
    }
    CatchWorkers();
  }
}

void SLE::RunForwardMultithreadPerSet(const int rows, const int col) {
  int shift = (rows - col) / threads_;
  for (int i = rows, trd = 0; i > col; i -= shift, ++trd) {
    if (trd == workers_.size() - 1) {
      shift = i - col;
    }
    workers_[trd] = Thread(&SLE::ForwardRunner, this, i, i - shift, col);
  }
}

void SLE::RunForwardMultithreadPerLine(const int rows, const int col) {
  for (int i = rows, trd = 0; i > col; --i, ++trd) {
    workers_[trd] = Thread(&SLE::ForwardRunner, this, i, i - 1, col);
  }
}

void SLE::ForwardRunner(const int from, const int to, const int col) {
  for (int row = from; row > to; --row) {
    SetElementToZero(row, col);
  }
}

void SLE::MakeUnitsDiagonallyPrl() {
  int rows = extended_.get_rows();
  if (threads_ < rows) {
    RunDiagonallyMultithreadPerSet(rows);
  } else {
    RunDiagonallyMultithreadPerLine(rows);
  }
  CatchWorkers();
}

void SLE::RunDiagonallyMultithreadPerSet(const int rows) {
  int shift = rows / threads_;
  for (int i = 0, trd = 0; i < rows - 1; i += shift, ++trd) {
    if (trd == workers_.size() - 1) {
      shift = rows - i - 1;
    }
    workers_[trd] = Thread(&SLE::DiagonallyRunner, this, i, i + shift + 1);
  }
}

void SLE::RunDiagonallyMultithreadPerLine(const int rows) {
  for (int i = 0; i < rows; ++i) {
    workers_[i] = Thread(&SLE::DiagonallyRunner, this, i, i + 1);
  }
}

void SLE::DiagonallyRunner(const int from, const int to) {
  int cols = extended_.get_cols();
  for (int i = from; i < to; ++i) {
    double factor = extended_(i, i);
    for (int j = 0; j < cols; ++j) {
      extended_(i, j) /= factor;
    }
  }
}

void SLE::GaussBackwardPrl() {
  for (int col = extended_.get_rows() - 1; col >= 0; --col) {
    if (threads_ < col) {
      RunBackwardMultithreadPerSet(col);
    } else {
      RunBackwardMultithreadPerLine(col);
    }
    CatchWorkers();
  }
}

void SLE::RunBackwardMultithreadPerSet(const int col) {
  int rows = col - 1;
  int shift = rows / threads_;
  for (int i = rows, trd = 0; i > 0; i -= shift, ++trd) {
    if (trd == workers_.size() - 1) {
      shift = i;
    }
    workers_[trd] = Thread(&SLE::BackwardRunner, this, i, i - shift, col);
  }
}

void SLE::RunBackwardMultithreadPerLine(const int col) {
  for (int row = col - 1; row >= 0; --row) {
    double factor = extended_(row, col);
    workers_[row] = Thread(&Matrix::AddRowMultiplyedByNumberToRow, &extended_,
                           col, -factor, row);
  }
}

void SLE::BackwardRunner(const int from, const int to, const int col) {
  for (int row = from, trd = 0; row >= to; --row, ++trd) {
    double factor = extended_(row, col);
    extended_.AddRowMultiplyedByNumberToRow(col, -factor, row);
  }
}

void SLE::CatchWorkers() {
  for (int i = 0; i < workers_.size(); ++i) {
    if (workers_[i].joinable()) {
      workers_[i].join();
    }
  }
}

}  // namespace s21

#include "../../include/timer/timer.h"
#include "sle.h"

namespace s21 {

void SLE::SolveParallel() {
  GaussForwardPrl();
  extended_.Save("tester00.txt");
  MakeUnitsDiagonallyPrl();
  extended_.Save("tester11.txt");
  GaussBackwardPrl();
  extended_.Save("tester.txt");
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
    task_manager_.AwaitAll();
  }
}

void SLE::RunForwardMultithreadPerSet(const int rows, const int col) {
  int shift = (rows - col) / threads_;
  int trd = 0;
  for (int i = rows; i > col; i -= shift, ++trd) {
    if (trd == threads_ - 1) {
      shift = i - col;
    }
    task_manager_.AddTask(&SLE::ForwardRunner, this, i, i - shift, col);
  }
}

void SLE::RunForwardMultithreadPerLine(const int rows, const int col) {
  int trd = 0;
  for (int i = rows; i > col; --i, ++trd) {
    task_manager_.AddTask(&SLE::ForwardRunner, this, i, i - 1, col);
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
  task_manager_.AwaitAll();
}

void SLE::RunDiagonallyMultithreadPerSet(const int rows) {
  int shift = rows / threads_;
  int trd = 0;
  for (int i = 0; i < rows - 1; i += shift, ++trd) {
    if (trd == threads_ - 1) {
      shift = rows - i;
    }
    task_manager_.AddTask(&SLE::DiagonallyRunner, this, i, i + shift);
  }
}

void SLE::RunDiagonallyMultithreadPerLine(const int rows) {
  for (int i = 0; i < rows; ++i) {
    task_manager_.AddTask(&SLE::DiagonallyRunner, this, i, i + 1);
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
  for (int col = extended_.get_rows() - 1; col > 0; --col) {
    if (threads_ < col) {
      RunBackwardMultithreadPerSet(col);
    } else {
      RunBackwardMultithreadPerLine(col);
    }
    task_manager_.AwaitAll();
  }
}

void SLE::RunBackwardMultithreadPerSet(const int col) {
  int rows = col - 1;
  int shift = rows / threads_;
  int trd = 0;
  for (int i = rows; i > 0; i -= shift, ++trd) {
    if (trd == threads_ - 1) {
      shift = i;
    }
    task_manager_.AddTask(&SLE::BackwardRunner, this, i, i - shift, col);
  }
}

void SLE::RunBackwardMultithreadPerLine(const int col) {
  for (int row = col - 1; row >= 0; --row) {
    double factor = extended_(row, col);
    task_manager_.AddTask(&Matrix::AddRowMultiplyedByNumberToRow, &extended_,
                          col, -factor, row);
  }
}

void SLE::BackwardRunner(const int from, const int to, const int col) {
  for (int row = from, trd = 0; row >= to; --row, ++trd) {
    double factor = extended_(row, col);
    extended_.AddRowMultiplyedByNumberToRow(col, -factor, row);
  }
}

}  // namespace s21

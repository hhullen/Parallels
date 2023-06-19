#ifndef SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_
#define SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

#include <matrix/matrix.h>

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

// #include "worker.h"

using hhullen::Matrix;

namespace s21 {
using Str = std::string;
using Thread = std::thread;
using ThreadPtr = std::shared_ptr<Thread>;
using Mutex = std::mutex;

class SLE {
 public:
  SLE();
  ~SLE();
  void Load(const Str &file_path);
  void Save(const Str &file_path);
  void SolveUsual();
  void SolveParallel();
  const Matrix GetVariables();

 private:
  Matrix extended_;
  // int threads_ = Thread::hardware_concurrency();
  int threads_ = 10;
  Mutex this_thread_;

  std::vector<Thread> workers_;

  void VerifyCorrectness();
  void SetElementToZero(const int row, const int col);
  // int GetAnotherRowIdxWithNonZeroElement(const int row, const int col);

  void GaussForward();
  void MakeUnitsDiagonally();
  void GaussBackward();

  void GaussForwardPrl();
  void MakeUnitsDiagonallyPrl();
  void GaussBackwardPrl();

  void RunForwardMultithreadPerSet(const int rows, const int col);
  void RunForwardMultithreadPerLine(const int rows, const int col);
  void ForwardRunner(const int from, const int to, const int col);

  void RunDiagonallyMultithreadPerSet(const int rows);
  void RunDiagonallyMultithreadPerLine(const int rows);
  void DiagonallyRunner(const int from, const int to);

  void CatchWorkers();
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

#ifndef SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_
#define SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

#include <matrix/matrix.h>

#include <string>

using hhullen::Matrix;

namespace s21 {
using Str = std::string;

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

  void VerifyCorrectness();

  void GaussForward();
  void SetElementToZero(const int row, const int col);
  int GetAnotherRowIdxWithNonZeroElement(const int row, const int col);
  void MakeUnitsDiagonally();
  void GaussBackward();
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

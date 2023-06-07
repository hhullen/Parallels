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
  void Load(Str &file_path);
  void Save(Str &file_path);
  void SolveUsual();
  void SolveParallel();
  const Matrix GetVariables();

 private:
  Matrix extended_;

  void VerifyCorrectness();
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

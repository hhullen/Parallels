#ifndef SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_
#define SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

#include <matrix/matrix.h>

#include <string>

using hhullen::Matrix;
using std::string;

namespace s21 {

class SLE {
 public:
  SLE();
  ~SLE();
  void Load(string &file_path);
  void Save(string &file_path);
  void SolveUsual();
  void SolveParallel();
  const Matrix GetVariables();

 private:
  Matrix extended_;
  Matrix variables_;

  void VerifyCorrectness();
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_SLE_ALGORITHM_SLE_H_

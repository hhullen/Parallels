#include "sle.h"

namespace s21 {

SLE::SLE() {}

SLE::~SLE() {}

void SLE::Load(string &file_path) {
  extended_.Load(file_path);
  VerifyCorrectness();
}

void SLE::Save(string &file_path) { extended_.Save(file_path); }

void SLE::SolveUsual() {}

void SLE::SolveParallel() {}

const Matrix SLE::GetVariables() {}

void SLE::VerifyCorrectness() {}

}  // namespace s21

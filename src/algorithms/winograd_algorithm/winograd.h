#include "../../include/matrix/matrix.h"
#include <vector>
namespace s21 {
class winograd {
  //   using matrix = std::vector<std::vector<double>>;

private:
  hhullen::Matrix multiplicand, factor;

public:
  std::vector<std::vector<double>>
  multiplyMatrices(const std::vector<std::vector<double>> &multiplicand,
                   const std::vector<std::vector<double>> &factor);
  winograd(string pathMultiplicand, string pathFactor);
  ~winograd();
};
} // namespace s21

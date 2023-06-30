#include <vector>

class winograd {
  using matrix = std::vector<std::vector<double>>;

private:
  matrix multiplicand, factor;

public:
  winograd(matrix multiplicand, matrix factor);
  ~winograd();
};

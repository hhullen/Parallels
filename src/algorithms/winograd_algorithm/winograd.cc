#include "winograd.h"
#include <iostream>

winograd::winograd(matrix multiplicand, matrix factor) {}

winograd::~winograd() {}

std::vector<std::vector<double>>
multiplyMatrices(const std::vector<std::vector<double>> &multiplicand,
                 const std::vector<std::vector<double>> &factor) {
  int rows1 = multiplicand.size();
  int cols1 = multiplicand[0].size();
  int rows2 = factor.size();
  int cols2 = factor[0].size();

  if (cols1 != rows2) {
    std::cout << "Error: Invalid matrix dimensions for multiplication"
              << std::endl;
    return {};
  }

  std::vector<std::vector<double>> result(rows1, std::vector<double>(cols2, 0));
  std::vector<double> rowFactor(rows1, 0);
  std::vector<double> colFactor(cols2, 0);

  for (int i = 0; i < rows1; i++) {
    for (int j = 0; j < cols1 / 2; j++) {
      rowFactor[i] += multiplicand[i][2 * j] * multiplicand[i][2 * j + 1];
    }
  }

  for (int i = 0; i < rows1; ++i) {
    std::cout << rowFactor[i];
  }

  for (int i = 0; i < cols2; i++) {
    for (int j = 0; j < rows2 / 2; j++) {
      colFactor[i] += factor[2 * j][i] * factor[2 * j + 1][i];
    }
  }

  for (int i = 0; i < rows1; i++) {
    for (int j = 0; j < cols2; j++) {
      int sum = -rowFactor[i] - colFactor[j];
      for (int k = 0; k < cols1 / 2; k++) {
        sum += (multiplicand[i][2 * k] + factor[2 * k + 1][j]) *
               (multiplicand[i][2 * k + 1] + factor[2 * k][j]);
      }
      result[i][j] = sum;
    }
  }

  if (cols1 % 2 == 1) {
    for (int i = 0; i < rows1; i++) {
      for (int j = 0; j < cols2; j++) {
        result[i][j] += multiplicand[i][cols1 - 1] * factor[cols1 - 1][j];
      }
    }
  }

  return result;
}

int main() {
  std::vector<std::vector<double>> multiplicand = {{1, 2, 3}, {4, 5, 6}};
  std::vector<std::vector<double>> factor = {{7, 8}, {9, 10}, {11, 12}};
  std::vector<std::vector<double>> result =
      multiplyMatrices(multiplicand, factor);
  for (const auto &row : result) {
    for (int element : row) {
      std::cout << element << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
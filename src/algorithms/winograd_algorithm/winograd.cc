#include <iostream>
#include <vector>

// using namespace std;

std::vector<std::vector<int>>
multiplyMatrices(const std::vector<std::vector<int>> &matrix1,
                 const std::vector<std::vector<int>> &matrix2) {
  int rows1 = matrix1.size();
  int cols1 = matrix1[0].size();
  int rows2 = matrix2.size();
  int cols2 = matrix2[0].size();

  if (cols1 != rows2) {
    std::cout << "Error: Invalid matrix dimensions for multiplication"
              << std::endl;
    return {};
  }

  std::vector<std::vector<int>> result(rows1, std::vector<int>(cols2, 0));
  std::vector<int> rowFactor(rows1, 0);
  std::vector<int> colFactor(cols2, 0);

  for (int i = 0; i < rows1; i++) {
    for (int j = 0; j < cols1 / 2; j++) {
      rowFactor[i] += matrix1[i][2 * j] * matrix1[i][2 * j + 1];
    }
  }

  for (int i = 0; i < rows1; ++i) {
    std::cout << rowFactor[i];
  }

  for (int i = 0; i < cols2; i++) {
    for (int j = 0; j < rows2 / 2; j++) {
      colFactor[i] += matrix2[2 * j][i] * matrix2[2 * j + 1][i];
    }
  }

  for (int i = 0; i < rows1; i++) {
    for (int j = 0; j < cols2; j++) {
      int sum = -rowFactor[i] - colFactor[j];
      for (int k = 0; k < cols1 / 2; k++) {
        sum += (matrix1[i][2 * k] + matrix2[2 * k + 1][j]) *
               (matrix1[i][2 * k + 1] + matrix2[2 * k][j]);
      }
      result[i][j] = sum;
    }
  }

  if (cols1 % 2 == 1) {
    for (int i = 0; i < rows1; i++) {
      for (int j = 0; j < cols2; j++) {
        result[i][j] += matrix1[i][cols1 - 1] * matrix2[cols1 - 1][j];
      }
    }
  }

  return result;
}

int main() {
  std::vector<std::vector<int>> matrix1 = {{1, 2, 3}, {4, 5, 6}};
  std::vector<std::vector<int>> matrix2 = {{7, 8}, {9, 10}, {11, 12}};
  std::vector<std::vector<int>> result = multiplyMatrices(matrix1, matrix2);
  for (const auto &row : result) {
    for (int element : row) {
      std::cout << element << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
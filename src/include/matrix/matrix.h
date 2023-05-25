#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
using std::atof;
using std::fstream;
using std::getline;
using std::ifstream;
using std::invalid_argument;
using std::isdigit;
using std::ofstream;
using std::out_of_range;
using std::sscanf;
using std::string;

const double kAccuracy = 0.0000001;
const bool kFillWithZero = true;
const bool kNoFill = false;

namespace hhullen {

class Matrix {
 public:
  Matrix();
  Matrix(int rows, int cols);
  Matrix(const Matrix& other);
  Matrix(Matrix&& other);
  ~Matrix();

  void Load(const string& file_path);
  void Save(const string& file_path);
  void ExchangeRows(const int row_1, const int row_2);
  bool IsEqual(const Matrix& other) const;
  void Summarize(const Matrix& other);
  void SummarizeRows(const int base_row, const int summ_row);
  void Substract(const Matrix& other);
  void MultiplyNumber(const double num);
  void Multiply(const Matrix& other);
  void MuliplyRowNumber(const int row, const double num);
  void HadamardProduct(const Matrix& other);
  Matrix Transpose() const;
  Matrix CelculateComplements();
  double Determinant();
  Matrix Inverse();
  double get_element(int i, int j) const;
  void set_element(int i, int j, double value);
  int get_rows() const;
  int get_cols() const;
  void set_rows(int new_val);
  void set_cols(int new_val);

  bool operator==(const Matrix& other) const;
  Matrix& operator=(const Matrix& other);
  Matrix operator+(const Matrix& other) const;
  Matrix operator-(const Matrix& other) const;
  Matrix operator*(const Matrix& other) const;
  Matrix operator+=(const Matrix& other);
  Matrix operator-=(const Matrix& other);
  Matrix operator*=(const Matrix& other);
  double& operator()(int i, int j);
  double operator()(int i, int j) const;

 private:
  int rows_, cols_;
  double** matrix_;
  ifstream* input_file_;
  ofstream* output_file_;

  void init_matrix(bool fill);
  void copy_data_other_to_this_matrix(double** other_matrix);
  void copy_data_this_to_other_matrix(double** other_matrix);
  void calculate_multiplied_matrix_element(const Matrix& other, int i, int j,
                                           double* res);
  void fill_with_zeros(int current_row);
  double calculate_2d_Determinant();
  double calculate_3d_Determinant();
  double calculate_Gauss_Determinant();
  double multiply_diagonal(Matrix* buffer);
  void process_the_row(Matrix* buffer, int row, int col, bool* is_det_zero);
  void scan_column_to_find_nonzero_num(Matrix* buffer, const int row,
                                       const int col, bool* is_det_zero);
  void summ_rows(Matrix* buffer, const int row_num, const int row_zero);
  double algebraic_addition(Matrix* initial_matrix, int row, int col);
  void make_matrix_minor(Matrix* initial_matrix, int row, int col,
                         Matrix* minor);
  void IsInputFileOpened(const ifstream& file);
  void IsOutputFileOpened(const ofstream& file);
  void ReadMatrixSize(ifstream& file);
  void ReadMatrix(ifstream& file);
  void ReadLineToMatrixRow(const string& line, int row);
  void ShiftToNextNumber(const string& line, size_t* i);
  void WriteMatrixSize(ofstream& file);
  void WriteMatrix(ofstream& file);
};

}  // namespace hhullen

#endif  // SRC_MATRIX_H_

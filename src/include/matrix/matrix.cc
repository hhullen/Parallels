#include "matrix.h"

namespace hhullen {

/*
  Public functions
*/
/**
 * @brief Construct a new Matrix::Matrix object
 *
 */
Matrix::Matrix() : rows_(4), cols_(4) { init_matrix(kFILL_WITH_ZERO); }

/**
 * @brief Construct a new Matrix::Matrix object
 *
 * @param rows int type
 * @param cols int type
 */
Matrix::Matrix(int rows, int cols) {
  if (rows <= 0 && cols <= 0) {
    throw invalid_argument("Creation matrix with 0x0 size or less");
  }

  rows_ = rows;
  cols_ = cols;
  init_matrix(kFILL_WITH_ZERO);
}

/**
 * @brief Construct a new Matrix::Matrix object
 *
 * @param other const Matrix& type
 */
Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_) {
  init_matrix(kNO_FILL);
  copy_data_other_to_this_matrix(other.matrix_);
}

/**
 * @brief Construct a new Matrix::Matrix object
 *
 * @param other Matrix&& type
 */
Matrix::Matrix(Matrix&& other) {
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = other.matrix_;

  other.matrix_ = nullptr;
  other.cols_ = 0;
  other.rows_ = 0;
}

/**
 * @brief Destroy the Matrix::Matrix object
 *
 */
Matrix::~Matrix() {
  if (matrix_) {
    for (rows_ -= 1; rows_ >= 0; rows_ -= 1) {
      if (matrix_[rows_]) {
        delete[] matrix_[rows_];
        matrix_[rows_] = nullptr;
      }
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
  cols_ = 0;
  rows_ = 0;
}

/**
 * @brief Compare matrix of this object with other
 *
 * @param other const Matrix& type
 * @return true
 * @return false
 */
bool Matrix::IsEqual(const Matrix& other) const {
  bool is_equal = true;

  if (rows_ == other.rows_ && cols_ == other.cols_) {
    for (int i = 0; is_equal && i < rows_; ++i) {
      for (int j = 0; is_equal && j < cols_; ++j) {
        is_equal = fabs(matrix_[i][j] - other.matrix_[i][j]) < kACCURACY;
      }
    }
  } else {
    is_equal = false;
  }

  return is_equal;
}

/**
 * @brief Summs this matrix with other
 *
 * @param other const Matrix& type
 */
void Matrix::Summarize(const Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw invalid_argument("Summation the matrix that is not square");
  }

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

/**
 * @brief Substracts this matrix with other
 *
 * @param other const Matrix& type
 */
void Matrix::Substract(const Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw invalid_argument("Substraction the matrix that is not square");
  }

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

/**
 * @brief Multiplies every matrix element by "num" value
 *
 * @param num const double type
 */
void Matrix::MultiplyNumber(const double num) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= num;
    }
  }
}

/**
 * @brief Multiplies matrix of this object with other matrix with sizes m*k and
 * k*m
 *
 * @param other const Matrix& type
 */
void Matrix::Multiply(const Matrix& other) {
  if (cols_ != other.rows_) {
    throw invalid_argument(
        "Multiplication matrix with different cols and rows");
  }
  double* buffer_row = nullptr;

  for (int i = 0; i < rows_; ++i) {
    buffer_row = new double[other.cols_];
    for (int j = 0; j < other.cols_; ++j) {
      buffer_row[j] = 0;
      calculate_multiplied_matrix_element(other, i, j, &buffer_row[j]);
    }
    delete[] matrix_[i];
    matrix_[i] = buffer_row;
    buffer_row = nullptr;
  }
  cols_ = other.cols_;
}

/**
 * @brief Calculates elementwise product
 *
 * @param other const Matrix& type
 */
void Matrix::HadamardProduct(const Matrix& other) {
  if (cols_ != other.cols_ || rows_ != other.rows_) {
    throw invalid_argument("Hadamatd product with different cols or rows");
  }

  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] *= other.matrix_[i][j];
    }
  }
}

/**
 * @brief Transposes matrix
 *
 * @return Matrix
 */
Matrix Matrix::Transpose() const {
  Matrix returnable(cols_, rows_);

  for (int i = 0; i < returnable.rows_; ++i) {
    for (int j = 0; j < returnable.cols_; ++j) {
      returnable.matrix_[i][j] = matrix_[j][i];
    }
  }

  return returnable;
}

/**
 * @brief Calculates algebraic additions matrix of this object
 *
 * @return Matrix
 */
Matrix Matrix::CelculateComplements() {
  if (cols_ != rows_) {
    throw invalid_argument(
        "Calculation complements of matrices with different cols and rows");
  }
  Matrix returnable(cols_, rows_);

  if (cols_ != 1 && rows_ != 1) {
    for (int i = 0; i < returnable.rows_; ++i) {
      for (int j = 0; j < returnable.cols_; ++j) {
        returnable.matrix_[i][j] =
            algebraic_addition(this, i, j) * (-1 + !((i + j) % 2) * 2);
      }
    }
  } else {
    returnable.matrix_[0][0] = 1;
  }

  return returnable;
}

/**
 * @brief Calculates Determinant
 *
 * @return double
 */
double Matrix::Determinant() {
  double returnable = NAN;

  if (cols_ != rows_) {
    throw invalid_argument(
        "Calculation Determinant of matrix with different cols and rows");
  }

  if (rows_ == 1) {
    returnable = matrix_[0][0];
  } else if (rows_ == 2) {
    returnable = calculate_2d_Determinant();
  } else if (rows_ == 3) {
    returnable = calculate_3d_Determinant();
  } else {
    returnable = calculate_Gauss_Determinant();
  }

  return returnable;
}

/**
 * @brief Calculates inverse matrix
 *
 * @return Matrix
 */
Matrix Matrix::Inverse() {
  double det = this->Determinant();

  if (fabs(det) < kACCURACY) {
    throw invalid_argument("Inversion of matrix with Determinant equals 0");
  }
  Matrix returnable(*this);

  returnable = returnable.CelculateComplements();
  returnable = returnable.Transpose();
  returnable.MultiplyNumber(1 / det);

  return returnable;
}

/**
 * @brief Returns matrix element from i*j position
 *
 * @param i int type
 * @param j int type
 * @return double
 */
double Matrix::get_element(int i, int j) const {
  if ((i < 0 || i >= rows_) || (j < 0 || j >= cols_)) {
    throw out_of_range("Getting element that is out of matrix range");
  }

  return matrix_[i][j];
}

/**
 * @brief Sets "value" to matrix element in i*j position
 *
 * @param i int type
 * @param j int type
 * @param value double type
 */
void Matrix::set_element(int i, int j, double value) {
  if ((i < 0 || i >= rows_) || (j < 0 || j >= cols_)) {
    throw out_of_range("Setting element that is out of matrix range");
  }

  matrix_[i][j] = value;
}

/**
 * @brief Returns amount of matrix rows
 *
 * @return int
 */
int Matrix::get_rows() const { return rows_; }

/**
 * @brief Returns amount of matrix columns
 *
 * @return int
 */
int Matrix::get_cols() const { return cols_; }

/**
 * @brief Sets new amount of matrix rows
 *
 * @param new_val int type
 */
void Matrix::set_rows(int new_val) {
  if (new_val <= 0) {
    throw invalid_argument("Setting rows amount that is equal or less than 0");
  }
  double** buffer = new double*[new_val];

  if (new_val < rows_) {
    rows_ = new_val;
  }
  for (int i = 0; i < rows_; ++i) {
    buffer[i] = matrix_[i];
  }
  delete[] this->matrix_;
  this->matrix_ = buffer;
  buffer = nullptr;

  for (int i = rows_; i < new_val; ++i) {
    this->matrix_[i] = new double[cols_];
    fill_with_zeros(i);
  }
  rows_ = new_val;
}

/**
 * @brief Sets new amount of matrix cols
 *
 * @param new_val int type
 */
void Matrix::set_cols(int new_val) {
  if (new_val <= 0) {
    throw invalid_argument("Setting cols amount that is equal or less than 0");
  }
  double* buffer = nullptr;

  if (new_val < cols_) {
    cols_ = new_val;
  }
  for (int i = 0; i < rows_; ++i) {
    buffer = new double[new_val];
    for (int j = 0; j < cols_; ++j) {
      buffer[j] = matrix_[i][j];
    }
    for (int j = cols_; j < new_val; ++j) {
      buffer[j] = 0;
    }
    delete[] matrix_[i];
    matrix_[i] = buffer;
    buffer = nullptr;
  }
  cols_ = new_val;
}

/*
  Operators
*/
bool Matrix::operator==(const Matrix& other) const {
  return this->IsEqual(other);
}

Matrix& Matrix::operator=(const Matrix& other) {
  this->~Matrix();
  rows_ = other.rows_;
  cols_ = other.cols_;
  init_matrix(kNO_FILL);
  copy_data_other_to_this_matrix(other.matrix_);

  return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
  Matrix returnable(*this);
  returnable.Summarize(other);

  return returnable;
}

Matrix Matrix::operator-(const Matrix& other) const {
  Matrix returnable(*this);
  returnable.Substract(other);

  return returnable;
}

Matrix Matrix::operator*(const Matrix& other) const {
  Matrix returnable(*this);
  returnable.Multiply(other);

  return returnable;
}

Matrix Matrix::operator+=(const Matrix& other) {
  this->Summarize(other);
  return *this;
}

Matrix Matrix::operator-=(const Matrix& other) {
  this->Substract(other);
  return *this;
}

Matrix Matrix::operator*=(const Matrix& other) {
  this->Multiply(other);
  return *this;
}

double& Matrix::operator()(int i, int j) {
  if ((i < 0 || i > rows_) || j < 0 || j > cols_) {
    throw out_of_range("Setting element that is out of matrix range");
  }

  return matrix_[i][j];
}

double Matrix::operator()(int i, int j) const {
  if ((i < 0 || i > rows_) || j < 0 || j > cols_) {
    throw out_of_range("Setting element that is out of matrix range");
  }

  return matrix_[i][j];
}

/*
  Private functions
*/
void Matrix::init_matrix(bool fill) {
  matrix_ = new double*[rows_];
  if (matrix_) {
    for (int i = 0; i < rows_; ++i) {
      matrix_[i] = new double[cols_];
      if (fill) fill_with_zeros(i);
    }
  }
}

void Matrix::copy_data_other_to_this_matrix(double** other_matrix) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = other_matrix[i][j];
    }
  }
}

void Matrix::copy_data_this_to_other_matrix(double** other_matrix) {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      other_matrix[i][j] = matrix_[i][j];
    }
  }
}

void Matrix::calculate_multiplied_matrix_element(const Matrix& other, int i,
                                                 int j, double* res) {
  for (int k = 0; k < cols_; k += 1) {
    *res += matrix_[i][k] * other.matrix_[k][j];
  }
}

void Matrix::fill_with_zeros(int current_row) {
  for (int i = 0; i < cols_; ++i) {
    matrix_[current_row][i] = 0;
  }
}

double Matrix::calculate_2d_Determinant() {
  return matrix_[0][0] * matrix_[1][1] - matrix_[1][0] * matrix_[0][1];
}

double Matrix::calculate_3d_Determinant() {
  double returnable = 0.0;

  returnable += matrix_[0][0] *
                (matrix_[1][1] * matrix_[2][2] - matrix_[2][1] * matrix_[1][2]);
  returnable -= matrix_[0][1] *
                (matrix_[1][0] * matrix_[2][2] - matrix_[2][0] * matrix_[1][2]);
  returnable += matrix_[0][2] *
                (matrix_[1][0] * matrix_[2][1] - matrix_[2][0] * matrix_[1][1]);

  return returnable;
}

double Matrix::calculate_Gauss_Determinant() {
  Matrix buffer(cols_, rows_);
  double returnable = 0.0;
  int row_constrain = 0;
  bool is_det_zero = false;

  copy_data_this_to_other_matrix(buffer.matrix_);
  for (int i = 0; !is_det_zero && i < buffer.cols_; ++i) {
    for (int j = buffer.rows_ - 1; !is_det_zero && j > row_constrain; j -= 1) {
      process_the_row(&buffer, j, i, &is_det_zero);
    }
    row_constrain += 1;
  }
  if (!is_det_zero) {
    returnable = multiply_diagonal(&buffer);
  } else {
    returnable = 0.0;
  }

  return returnable;
}

double Matrix::multiply_diagonal(Matrix* buffer) {
  double returnable = 1.0;

  for (int i = 0; i < buffer->cols_; ++i) {
    returnable *= buffer->matrix_[i][i];
  }

  return returnable;
}

void Matrix::process_the_row(Matrix* buffer, int row, int col,
                             bool* is_det_zero) {
  double prew_row_n = buffer->matrix_[row - 1][col];
  double row_n = buffer->matrix_[row][col] * -1;

  for (int k = col; k < buffer->cols_ && !*is_det_zero; k += 1) {
    if (prew_row_n != 0) {
      buffer->matrix_[row][k] =
          (row_n / prew_row_n) * buffer->matrix_[row - 1][k] +
          buffer->matrix_[row][k];
    } else {
      scan_column_to_find_nonzero_num(buffer, row - 1, k, is_det_zero);
      prew_row_n = buffer->matrix_[row - 1][col];
      buffer->matrix_[row][k] =
          (row_n / prew_row_n) * buffer->matrix_[row - 1][k] +
          buffer->matrix_[row][k];
    }
  }
  buffer->matrix_[row][col] = 0;
}

void Matrix::scan_column_to_find_nonzero_num(Matrix* buffer, const int row,
                                             const int col, bool* is_det_zero) {
  bool found = false;

  for (int i = buffer->rows_ - 1; !found && i >= 0; i--) {
    if (fabs(buffer->matrix_[i][col]) >= kACCURACY) {
      summ_rows(buffer, i, row);
      found = true;
    } else if (i == 0) {
      *is_det_zero = true;
    }
  }
}

void Matrix::summ_rows(Matrix* buffer, const int row_num, const int row_zero) {
  for (int i = 0; i < buffer->cols_; ++i) {
    buffer->matrix_[row_zero][i] += buffer->matrix_[row_num][i];
  }
}

double Matrix::algebraic_addition(Matrix* initial_matrix, int row, int col) {
  double returnable = 0.0;
  Matrix minor(initial_matrix->rows_ - 1, initial_matrix->cols_ - 1);

  make_matrix_minor(initial_matrix, row, col, &minor);
  returnable = minor.Determinant();

  return returnable;
}

void Matrix::make_matrix_minor(Matrix* initial_matrix, int row, int col,
                               Matrix* minor) {
  int i = 0, j = 0;

  for (int im = 0; im < minor->rows_; ++im) {
    for (int jm = 0; jm < minor->rows_; ++jm) {
      if (i == row) ++i;
      if (j == col) ++j;
      minor->matrix_[im][jm] = initial_matrix->matrix_[i][j];
      ++i;
    }
    i = 0;
    ++j;
  }
}

}  // namespace hhullen

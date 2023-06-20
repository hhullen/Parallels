#include <gtest/gtest.h>
#include <sle_algorithm/sle.h>

TEST(SLE_tests, constructor) { EXPECT_NO_THROW(s21::SLE object); }

TEST(SLE_tests, loading) {
  s21::SLE object;
  EXPECT_NO_THROW(object.Load("tests/dataset/matrix_random_1000.txt"));
  EXPECT_THROW(object.Load("ERROR_FILE.NO"), invalid_argument);
}

TEST(SLE_tests, saving) {
  s21::SLE object;
  EXPECT_NO_THROW(object.Load("tests/dataset/matrix_random_1000.txt"));
  EXPECT_NO_THROW(object.Save("matrix_random_1000_test.out"));
}

TEST(SLE_tests, solve_usual) {
  s21::SLE object;
  object.Load("tests/dataset/matrix_correct_4.txt");
  EXPECT_NO_THROW(object.SolveUsual());
  object.Load("tests/dataset/matrix_correct_8.txt");
  EXPECT_NO_THROW(object.SolveUsual());
  object.Load("tests/dataset/matrix_random_200.txt");
  EXPECT_NO_THROW(object.SolveUsual());
}

TEST(SLE_tests, solve_parallel) {
  s21::SLE object;
  object.Load("tests/dataset/matrix_correct_4.txt");
  EXPECT_NO_THROW(object.SolveParallel());
  object.Load("tests/dataset/matrix_correct_8.txt");
  EXPECT_NO_THROW(object.SolveParallel());
  object.Load("tests/dataset/matrix_random_200.txt");
  EXPECT_NO_THROW(object.SolveParallel());
}

TEST(SLE_tests, get_variables) {
  s21::SLE object;

  object.Load("tests/dataset/matrix_correct_8.txt");
  object.SolveParallel();

  EXPECT_NO_THROW(Matrix vars = object.GetVariables());
}

TEST(SLE_tests, set_threads) {
  s21::SLE object;

  object.Load("tests/dataset/matrix_correct_8.txt");
  EXPECT_NO_THROW(object.SetThreads(5));
  object.SolveParallel();
  object.Load("tests/dataset/matrix_random_200.txt");
  EXPECT_NO_THROW(object.SetThreads(7));
  object.SolveParallel();
}

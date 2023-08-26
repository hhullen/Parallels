#include <gtest/gtest.h>

#include <vector>

#include "../utility_cli/utility_cli.cc"

using std::vector;

TEST(CLI_tests, constructor) {
  vector<const char *> argv({
      "prl",
      "-f",
      "tests/dataset/matrix_correct_4.txt",
      "-n",
      "5",
      "SLE",
  });
  EXPECT_NO_THROW(
      s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data()));
}

TEST(CLI_tests, no_algorithm) {
  vector<const char *> argv({
      "prl",
      "-f",
      "tests/dataset/matrix_correct_4.txt",
      "-n",
      "5",
  });
  EXPECT_THROW(s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data()),
               invalid_argument);
}

TEST(CLI_tests, wrong_algorithm) {
  vector<const char *> argv({
      "prl",
      "WRONG_NAME",
      "-f",
      "tests/dataset/matrix_correct_4.txt",
      "-n",
      "5",
  });
  s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data());
  EXPECT_THROW(cmd.Exec(), invalid_argument);
}

TEST(CLI_tests, no_flags) {
  vector<const char *> argv({
      "prl",
      "SLE",
  });
  EXPECT_NO_THROW(
      s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data()));
}

TEST(CLI_tests, run_SLE_mc4_n5_t4) {
  vector<const char *> argv({
      "prl",
      "SLE",
      "-f",
      "tests/dataset/matrix_correct_4.txt",
      "-n",
      "5",
      "-t",
      "4",
  });
  s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data());
  EXPECT_NO_THROW(cmd.Exec());
}

TEST(CLI_tests, run_SLE_mc4_n500_t3) {
  vector<const char *> argv({
      "prl",
      "SLE",
      "-f",
      "tests/dataset/matrix_correct_4.txt",
      "-n",
      "500",
      "-t",
      "3",
  });
  s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data());
  EXPECT_NO_THROW(cmd.Exec());
}

TEST(CLI_tests, run_SLE_mr500_n2_t5) {
  vector<const char *> argv({
      "prl",
      "SLE",
      "-f",
      "tests/dataset/matrix_random_500.txt",
      "-n",
      "2",
      "-t",
      "5",
  });
  s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data());
  EXPECT_NO_THROW(cmd.Exec());
}

TEST(CLI_tests, run_SLE_mr500_n0_t5) {
  vector<const char *> argv({
      "prl",
      "SLE",
      "-f",
      "tests/dataset/matrix_random_500.txt",
      "-n",
      "0",
      "-t",
      "5",
  });
  s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data());
  EXPECT_NO_THROW(cmd.Exec());
}

TEST(CLI_tests, run_SLE_mr500_n2neg_t5) {
  vector<const char *> argv({
      "prl",
      "SLE",
      "-f",
      "tests/dataset/matrix_random_500.txt",
      "-n",
      "-2",
      "-t",
      "5",
  });
  EXPECT_THROW(s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data()),
               invalid_argument);
}

TEST(CLI_tests, run_SLE_mr500_n2_t5neg) {
  vector<const char *> argv({
      "prl",
      "SLE",
      "-f",
      "tests/dataset/matrix_random_500.txt",
      "-n",
      "2",
      "-t",
      "-5",
  });
  EXPECT_THROW(s21::UtilityCLI cmd(static_cast<int>(argv.size()), argv.data()),
               invalid_argument);
}
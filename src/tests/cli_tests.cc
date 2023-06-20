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
  EXPECT_NO_THROW(s21::UtilityCLI cmd(argv.size(), argv.data()));
}

TEST(CLI_tests, no_algorithm) {
  vector<const char *> argv({
      "prl",
      "-f",
      "tests/dataset/matrix_correct_4.txt",
      "-n",
      "5",
  });
  EXPECT_THROW(s21::UtilityCLI cmd(argv.size(), argv.data()), invalid_argument);
}

TEST(CLI_tests, no_flags) {
  vector<const char *> argv({
      "prl",
      "SLE",
  });
  EXPECT_NO_THROW(s21::UtilityCLI cmd(argv.size(), argv.data()));
}
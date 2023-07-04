#include <aco_algorithm/aco.h>
#include <gtest/gtest.h>

using std::invalid_argument;

TEST(ACO_tests, constructor) { EXPECT_NO_THROW(s21::TSPAlgorithm object); }

TEST(ACO_tests, loading) {
  s21::TSPAlgorithm object;
  s21::Graph grpaph;
  EXPECT_NO_THROW(
      grpaph.LoadGraphFromFile("tests/dataset/graph_example_11.txt"));
  EXPECT_THROW(grpaph.LoadGraphFromFile("ERROR_FILE.NO"), invalid_argument);
}

TEST(ACO_tests, solve_usual) {
  s21::TSPAlgorithm object;
  s21::Graph grpaph;

  grpaph.LoadGraphFromFile("tests/dataset/graph_example_100.txt");
  EXPECT_NO_THROW(object.Solve(grpaph));
}

TEST(ACO_tests, solve_parallel) {
  s21::TSPAlgorithm object;
  s21::Graph grpaph;

  grpaph.LoadGraphFromFile("tests/dataset/graph_example_100.txt");
  EXPECT_NO_THROW(object.SolveParallel(grpaph));
}

TEST(ACO_tests, set_threads) {
  s21::TSPAlgorithm object(2);
  s21::Graph grpaph;

  grpaph.LoadGraphFromFile("tests/dataset/graph_example_100.txt");
  EXPECT_NO_THROW(object.SolveParallel(grpaph));

  s21::TSPAlgorithm object_new(11);
  grpaph.LoadGraphFromFile("tests/dataset/graph_example_100.txt");
  EXPECT_NO_THROW(object_new.SolveParallel(grpaph));
}

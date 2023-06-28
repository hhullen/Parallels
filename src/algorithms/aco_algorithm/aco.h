#ifndef SRC_ALGORITHMS_SLE_ALGORITHM_ACO_H_
#define SRC_ALGORITHMS_SLE_ALGORITHM_ACO_H_

#include <graph/graph.h>

#include <algorithm>
#include <random>
#include <vector>

namespace s21 {

using std::find;
using std::pair;
using std::pow;
using std::rand;
using std::vector;

struct TsmResult {
  TsmResult() : distance(0) {}
  vector<int> vertices;
  double distance;
};

class TSPAlgorithm {
 public:
  TsmResult Solve(Graph &graph);

 private:
  vector<pair<int, double>> probabilities_;
  const int kPheromone_track_ = 5, kPheromon_evaporation_rate_ = 1;
  const double kAlpha_ = 1, kBeta_ = 1;
  vector<int> attended_;
  Graph pheromones_;
  TsmResult result_;

  void CloseRoute(Graph &graph);
  void RunThroughGraphFromVertex(Graph &graph, int i);
  double GetProbabilitiesDenominator(Graph &graph, int start);
  bool IsNotAllAttended(Graph &graph);
  bool IsAttended(const int j);
  double CalculateNumerator(const int pheromone, const int edge_length);
  void DivideProbabilitiesByDenominator(const double denominator);
  double GetRandomPercentValue();
  int GetNextDestination(const double random_percent);
  void RunPheromonesEvaporation();
  void AddPheromoneTrack(int start, int dest);
  void UpdateResult(TsmResult &result, int distance, int dest);
  void SetNewResult(const TsmResult &result);
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_SLE_ALGORITHM_ACO_H_
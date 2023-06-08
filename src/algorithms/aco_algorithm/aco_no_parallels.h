#ifndef SRC_ALGORITHMS_ACO_ALGORITHM_ACO_NO_PARALLELS_H_
#define SRC_ALGORITHMS_ACO_ALGORITHM_ACO_NO_PARALLELS_H_

#include <algorithm>
#include <random>
#include <vector>

#include "../graph/graph.h"

using std::find;
using std::pair;
using std::pow;
using std::rand;
using std::vector;

namespace s21 {

struct TsmResult {
  vector<int> vertices;
  double distance = 0;
};

class TSPAlgorithm {
public:
  TsmResult Solve(Graph &graph);

private:
  vector<pair<size_t, float>> probabilities_;
  const int kPheromone_track_ = 5, kPheromon_evaporation_rate_ = 1;
  const float kAlpha_ = 1, kBeta_ = 1;
  vector<int> attended_;
  Graph pheromones_;
  TsmResult result_;

  void CloseRoute(Graph &graph);
  void RunThroughGraphFromVertex(Graph &graph, size_t i);
  float GetProbabilitiesDenominator(Graph &graph, size_t start);
  bool IsNotAllAttended(Graph &graph);
  bool IsAttended(const int j);
  float CalculateNumerator(const int pheromone, const int edge_length);
  void DivideProbabilitiesByDenominator(const float denominator);
  float GetRandomPercentValue();
  size_t GetNextDestination(const float random_percent);
  void RunPheromonesEvaporation();
  void AddPheromoneTrack(size_t start, size_t dest);
  void UpdateResult(TsmResult &result, int distance, size_t dest);
  void SetNewResult(TsmResult &result);
};

} // namespace s21

#endif // SRC_ALGORITHMS_ACO_ALGORITHM_ACO_NO_PARALLELS_H_
#ifndef SRC_ALGORITHMS_SLE_ALGORITHM_ACO_H_
#define SRC_ALGORITHMS_SLE_ALGORITHM_ACO_H_

#include <graph/graph.h>

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <random>
#include <vector>

#include "../task_manager/task_manager.h"

using std::atomic;
using std::find;
using std::mutex;
using std::pair;
using std::pow;
using std::rand;
using std::unique_lock;
using std::vector;

namespace s21 {

struct TsmResult {
  TsmResult() : distance(0) {}
  vector<int> vertices;
  double distance;
};

class TSPAlgorithm {
 public:
  TSPAlgorithm(size_t threads = 2);
  TsmResult Solve(Graph &graph);
  TsmResult SolveParallel(Graph &graph);

 private:
  TaskManager task_namager_{2};
  vector<pair<int, double>> probabilities_;
  const int kPheromone_track_ = 5, kPheromon_evaporation_rate_ = 1;
  const double kAlpha_ = 1, kBeta_ = 1;
  vector<int> attended_;
  Graph pheromones_;
  TsmResult result_;
  mutex mtx_;

  void CloseRoute(const Graph &graph);
  void RunThroughGraphFromVertex(Graph &graph, int i);
  void RunThroughGraphFromVertexTrd(Graph graph, int i);
  double GetProbabilitiesDenominator(Graph &graph, int start);
  double GetProbabilitiesDenominatorTrd(
      Graph &graph, int start, vector<pair<int, double>> &probabilities,
      vector<int> &attended);
  bool IsNotAllAttended(Graph &graph);
  bool IsAttended(const int j);
  bool IsAttendedTrd(const int j, vector<int> &attended);
  double CalculateNumerator(const int pheromone, const int edge_length);
  void DivideProbabilitiesByDenominator(const double denominator);
  void DivideProbabilitiesByDenominatorTrd(
      const double denominator, vector<pair<int, double>> &probabilities);
  double GetRandomPercentValue();
  int GetNextDestination(const double random_percent);
  int GetNextDestinationTrd(const double random_percent,
                            const vector<pair<int, double>> &probabilities);
  void RunPheromonesEvaporation();
  void AddPheromoneTrack(int start, int dest);
  void UpdateResult(TsmResult &result, int distance, int dest);
  void SetNewResult(const TsmResult &result);
};

}  // namespace s21

#endif  // SRC_ALGORITHMS_SLE_ALGORITHM_ACO_H_

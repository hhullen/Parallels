#include "aco.h"

namespace s21 {

TsmResult TSPAlgorithm::SolveParallel(Graph &graph) {
  int size = static_cast<int>(graph.get_size());
  pheromones_.Resize(size);
  for (int i = 0; i < size; ++i) {
    task_namager_.AddTask(&TSPAlgorithm::RunThroughGraphFromVertexTrd, this,
                          graph, i);
  }
  task_namager_.AwaitAll();
  CloseRoute(graph);
  return result_;
}

void TSPAlgorithm::RunThroughGraphFromVertexTrd(Graph graph, int start) {
  TsmResult result;
  result.vertices.emplace_back(start + 1);
  thread_local vector<int> attended;
  while (attended.size() < graph.get_size()) {
    thread_local vector<pair<int, double>> probabilities;
    double denominator =
        GetProbabilitiesDenominatorTrd(graph, start, probabilities, attended);
    DivideProbabilitiesByDenominatorTrd(denominator, probabilities);

    double random_percent = GetRandomPercentValue();
    int next_dest = GetNextDestinationTrd(random_percent, probabilities);

    RunPheromonesEvaporation();
    AddPheromoneTrack(start, next_dest);

    int distance = graph(start, next_dest);
    UpdateResult(result, distance, next_dest);
    start = next_dest;
    attended.emplace_back(start);
  }
  SetNewResult(result);
}

double TSPAlgorithm::GetProbabilitiesDenominatorTrd(
    Graph &graph, int start, vector<pair<int, double>> &probabilities,
    vector<int> &attended) {
  double denominator = 0;
  int size = static_cast<int>(graph.get_size());
  for (int dest = 0; dest < size; ++dest) {
    if (!IsAttendedTrd(dest, attended)) {
      int pheromone = pheromones_(start, dest);
      int edge_length = graph(start, dest);
      double numerator = CalculateNumerator(pheromone, edge_length);
      probabilities.emplace_back(pair<int, double>{dest, numerator});
      denominator += numerator;
    }
  }
  return denominator;
}

bool TSPAlgorithm::IsAttendedTrd(const int j, vector<int> &attended) {
  return find(attended.begin(), attended.end(), j) != attended.end();
}

void TSPAlgorithm::DivideProbabilitiesByDenominatorTrd(
    const double denominator, vector<pair<int, double>> &probabilities) {
  for (size_t i = 0; i < probabilities.size(); ++i) {
    probabilities[i].second = probabilities[i].second / denominator * 100;
  }
}

int TSPAlgorithm::GetNextDestinationTrd(
    const double random_percent,
    const vector<pair<int, double>> &probabilities) {
  size_t i = 0;
  size_t size = probabilities.size() - 1;
  double summ = 0;
  for (; summ < random_percent && i < size; ++i) {
    summ += probabilities[i].second;
  }
  return probabilities[i].first;
}

}  // namespace s21

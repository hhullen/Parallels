#include "aco.h"

namespace s21 {

TsmResult TSPAlgorithm::SolveParallel(Graph &graph) {
  int size = static_cast<int>(graph.get_size());
  pheromones_.Resize(size);
  attended_.clear();
  for (int i = 0; i < size; ++i) {
    attended_.push_back(i);
    RunThroughGraphFromVertex(graph, i);
    attended_.clear();
  }
  CloseRoute(graph);
  return result_;
}

void TSPAlgorithm::RunThroughGraphFromVertex(Graph &graph, int start) {
  TsmResult result;
  result.vertices.push_back(start + 1);
  while (IsNotAllAttended(graph)) {
    probabilities_.clear();
    double denominator = GetProbabilitiesDenominator(graph, start);
    DivideProbabilitiesByDenominator(denominator);

    double random_percent = GetRandomPercentValue();
    int next_dest = GetNextDestination(random_percent);

    RunPheromonesEvaporation();
    AddPheromoneTrack(start, next_dest);

    int distance = graph(start, next_dest);
    UpdateResult(result, distance, next_dest);
    start = next_dest;
    attended_.push_back(start);
  }
  SetNewResult(result);
}

double TSPAlgorithm::GetProbabilitiesDenominator(Graph &graph, int start) {
  double denominator = 0;
  int size = static_cast<int>(graph.get_size());
  for (int dest = 0; dest < size; ++dest) {
    if (!IsAttended(dest)) {
      int pheromone = pheromones_(start, dest);
      int edge_length = graph(start, dest);
      double numerator = CalculateNumerator(pheromone, edge_length);
      probabilities_.push_back(pair(dest, numerator));
      denominator += numerator;
    }
  }
  return denominator;
}

bool TSPAlgorithm::IsNotAllAttended(Graph &graph) {
  return attended_.size() < graph.get_size();
}

bool TSPAlgorithm::IsAttended(const int j) {
  return find(attended_.begin(), attended_.end(), j) != attended_.end();
}

double TSPAlgorithm::CalculateNumerator(const int pheromone,
                                        const int edge_length) {
  return pow(pheromone, kAlpha_) + 1 / pow(edge_length, kBeta_);
}

void TSPAlgorithm::DivideProbabilitiesByDenominator(const double denominator) {
  for (size_t i = 0; i < probabilities_.size(); ++i) {
    probabilities_[i].second = probabilities_[i].second / denominator * 100;
  }
}

int TSPAlgorithm::GetNextDestination(const double random_percent) {
  size_t i = 0, size = probabilities_.size() - 1;
  double summ = 0;
  for (; summ < random_percent && i < size; ++i) {
    summ += probabilities_[i].second;
  }
  return probabilities_[i].first;
}

void TSPAlgorithm::RunPheromonesEvaporation() {
  int size = static_cast<int>(pheromones_.get_size());
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (pheromones_(i, j) - kPheromon_evaporation_rate_ < 0) {
        pheromones_(i, j);
      } else {
        pheromones_(i, j) -= kPheromon_evaporation_rate_;
      }
    }
  }
}

void TSPAlgorithm::AddPheromoneTrack(int start, int dest) {
  pheromones_(start, dest) += kPheromone_track_;
}

void TSPAlgorithm::UpdateResult(TsmResult &result, int distance, int dest) {
  result.distance += distance;
  result.vertices.push_back(static_cast<int>(dest) + 1);
}

void TSPAlgorithm::SetNewResult(const TsmResult &result) {
  if (result_.distance == 0 || result.distance < result_.distance) {
    result_ = result;
  }
}

}  // namespace s21

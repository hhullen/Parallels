#include "aco_no_parallels.h"

namespace s21 {

TsmResult TSPAlgorithm::Solve(Graph &graph) {
  size_t size = graph.get_size();
  pheromones_.Resize(size);
  attended_.clear();
  for (size_t i = 0; i < size; ++i) {
    attended_.push_back(i);
    RunThroughGraphFromVertex(graph, i);
    attended_.clear();
  }
  CloseRoute(graph);
  return result_;
}

void TSPAlgorithm::CloseRoute(Graph &graph) {
  int first = result_.vertices.front();
  int last = result_.vertices.back();
  result_.distance += graph(first, last);
  result_.vertices.push_back(first);
}

void TSPAlgorithm::RunThroughGraphFromVertex(Graph &graph, size_t start) {
  TsmResult result;
  result.vertices.push_back(static_cast<int>(start) + 1);
  while (IsNotAllAttended(graph)) {
    probabilities_.clear();
    float denominator = GetProbabilitiesDenominator(graph, start);
    DivideProbabilitiesByDenominator(denominator);

    float random_percent = GetRandomPercentValue();
    size_t next_dest = GetNextDestination(random_percent);

    RunPheromonesEvaporation();
    AddPheromoneTrack(start, next_dest);

    int distance = graph(start, next_dest);
    UpdateResult(result, distance, next_dest);
    start = next_dest;
    attended_.push_back(start);
  }
  SetNewResult(result);
}

float TSPAlgorithm::GetProbabilitiesDenominator(Graph &graph, size_t start) {
  float denominator = 0;
  for (size_t dest = 0; dest < graph.get_size(); ++dest) {
    if (!IsAttended(dest)) {
      int pheromone = pheromones_(start, dest);
      int edge_length = graph(start, dest);
      float numerator = CalculateNumerator(pheromone, edge_length);
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

float TSPAlgorithm::CalculateNumerator(const int pheromone,
                                       const int edge_length) {
  return pow(pheromone, kAlpha_) + 1 / pow(edge_length, kBeta_);
}

void TSPAlgorithm::DivideProbabilitiesByDenominator(const float denominator) {
  for (size_t i = 0; i < probabilities_.size(); ++i) {
    probabilities_[i].second = probabilities_[i].second / denominator * 100;
  }
}

float TSPAlgorithm::GetRandomPercentValue() { return rand() % 101; }

size_t TSPAlgorithm::GetNextDestination(const float random_percent) {
  size_t i = 0, size = probabilities_.size() - 1;
  float summ = 0;
  for (; summ < random_percent && i < size; ++i) {
    summ += probabilities_[i].second;
  }
  return probabilities_[i].first;
}

void TSPAlgorithm::RunPheromonesEvaporation() {
  size_t size = pheromones_.get_size();
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; ++j) {
      if (pheromones_(i, j) - kPheromon_evaporation_rate_ < 0) {
        pheromones_(i, j);
      } else {
        pheromones_(i, j) -= kPheromon_evaporation_rate_;
      }
    }
  }
}

void TSPAlgorithm::AddPheromoneTrack(size_t start, size_t dest) {
  pheromones_(start, dest) += kPheromone_track_;
}

void TSPAlgorithm::UpdateResult(TsmResult &result, int distance, size_t dest) {
  result.distance += distance;
  result.vertices.push_back(static_cast<int>(dest) + 1);
}

void TSPAlgorithm::SetNewResult(TsmResult &result) {
  if (result_.distance == 0 || result.distance < result_.distance) {
    result_ = result;
  }
}

} // namespace s21
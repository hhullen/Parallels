#include "graph.h"

namespace s21 {

Graph::Graph() {}

Graph::Graph(int size) { Resize(size); }

Graph::Graph(const vector<vector<int>> &matrix) {
  matrix_ = AdjacencyMatrix(matrix);
  Resize(static_cast<int>(matrix.size()));
}

Graph::~Graph() { matrix_.Clear(); }

int &Graph::operator()(int i, int j) { return matrix_(i, j); }

int Graph::operator()(int i, int j) const { return matrix_(i, j); }

void Graph::Resize(int size) {
  if (size < 1) {
    throw invalid_argument("Incorrect adjacency matrix size.");
  }

  matrix_.Resize(size);
}

size_t Graph::get_size() { return matrix_.Size(); }

void Graph::LoadGraphFromFile(const string &path) {
  ifstream file(path);

  IsInputFileOpened(file);
  ReadAdjacencyMatrixSize(file);
  ReadAdjacencyMatrix(file);

  file.close();
}

void Graph::IsInputFileOpened(ifstream &input_file) {
  if (!input_file.is_open()) {
    throw invalid_argument("File cuold not be opened.");
  }
}

void Graph::ReadAdjacencyMatrixSize(ifstream &input_file) {
  string line;
  getline(input_file, line, '\n');
  int size = atoi(line.data());
  if (size < 1) {
    input_file.close();
    throw invalid_argument("Incorrect matrix size");
  }
  Resize(size);
}

void Graph::ReadAdjacencyMatrix(ifstream &input_file) {
  string line;
  int row = 0;
  int size = static_cast<int>(get_size());
  while (getline(input_file, line, '\n') && row < size) {
    ReadLineToMatrixRow(line, row);
    ++row;
  }
}

void Graph::ReadLineToMatrixRow(string &line, int row) {
  int col = 0;
  for (size_t i = 0; i < line.size(); ++i) {
    char *number = &(line.data())[i];
    matrix_(row, col) = atoi(number);
    ++col;
    ShiftToNextNumber(line, &i);
  }
}

void Graph::ShiftToNextNumber(string &line, size_t *i) {
  while (isdigit(line.data()[*i]) && *i < line.size()) {
    ++(*i);
  }
  while (!isdigit(line.data()[*i]) && *i < line.size()) {
    ++(*i);
  }
  --(*i);
}

}  // namespace s21

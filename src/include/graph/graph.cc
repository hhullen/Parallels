#include "graph.h"

namespace s21 {

Graph::Graph() : input_file_(nullptr), output_file_(nullptr) {}

Graph::Graph(int size) : input_file_(nullptr), output_file_(nullptr) {
  Resize(size);
}

Graph::Graph(const vector<vector<int>> &matrix)
    : input_file_(nullptr), output_file_(nullptr) {
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
  input_file_ = &file;

  IsInputFileOpened();
  ReadAdjacencyMatrixSize();
  ReadAdjacencyMatrix();

  input_file_->close();
  input_file_ = nullptr;
}

void Graph::IsInputFileOpened() {
  if (!input_file_->is_open()) {
    throw invalid_argument("File cuold not be opened.");
  }
}

void Graph::ReadAdjacencyMatrixSize() {
  string line;
  getline(*input_file_, line, '\n');
  int size = atoi(line.data());
  if (size < 1) {
    input_file_->close();
    throw invalid_argument("Incorrect matrix size");
  }
  Resize(size);
}

void Graph::ReadAdjacencyMatrix() {
  string line;
  int row = 0;
  int size = static_cast<int>(get_size());
  while (getline(*input_file_, line, '\n') && row < size) {
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

void Graph::ExportGraphToDot(const string &path, const string &graph_name) {
  ofstream file(path);
  output_file_ = &file;

  IsOutputFileOpened();
  WriteDotWriterObject(graph_name);

  output_file_->close();
  output_file_ = nullptr;
}

void Graph::IsOutputFileOpened() {
  if (!output_file_->is_open()) {
    throw invalid_argument("File could not be opened.");
  }
}

void Graph::WriteDotWriterObject(const string &graph_name) {
  RootGraph dot_object(false, "", graph_name);

  AddNodesToDotWriterObject(dot_object);
  AddEdgesToDotWriterObject(dot_object);

  dot_object.Print(*output_file_, 2);
  nodes_.clear();
  nodes_.shrink_to_fit();
}

void Graph::AddNodesToDotWriterObject(RootGraph &dot_object) {
  size_t size = get_size();
  for (size_t i = 1; i <= size; ++i) {
    string name = to_string(i);
    Node *node = dot_object.AddNode("", name);
    nodes_.push_back(node);
  }
}

void Graph::AddEdgesToDotWriterObject(RootGraph &dot_object) {
  int size = static_cast<int>(get_size());
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      AddEdgeBetwenNodesToDotWriterObject(i, j, dot_object);
    }
  }
}

void Graph::AddEdgeBetwenNodesToDotWriterObject(int i, int j,
                                                RootGraph &dot_object) {
  if (i != j) {
    Node *node_i = nodes_[static_cast<size_t>(i)];
    Node *node_j = nodes_[static_cast<size_t>(j)];
    Edge *edge = dot_object.AddEdge(node_i, node_j);
    EdgeAttributeSet &attribute = edge->GetAttributes();
    string weight = to_string((*this)(i, j));
    attribute.AddCustomAttribute("weight", weight);
  }
}

}  // namespace s21

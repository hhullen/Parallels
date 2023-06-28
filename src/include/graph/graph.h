#ifndef SRC_GRAPH_GRAPH_H_
#define SRC_GRAPH_GRAPH_H_

#include <dot_writer/DotWriter.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace s21 {

using DotWriter::Edge;
using DotWriter::EdgeAttributeSet;
using DotWriter::Node;
using DotWriter::RootGraph;
using std::atoi;
using std::cout;
using std::getline;
using std::ifstream;
using std::invalid_argument;
using std::isdigit;
using std::ofstream;
using std::ostream;
using std::string;
using std::to_string;
using std::vector;

class AdjacencyMatrix {
 public:
  AdjacencyMatrix() {}
  explicit AdjacencyMatrix(const vector<vector<int>> &matrix)
      : matrix_(matrix) {}
  void Clear() {
    matrix_.clear();
    matrix_.shrink_to_fit();
  }

  void Resize(int size) {
    matrix_.resize(static_cast<size_t>(size));
    matrix_.shrink_to_fit();
    for (vector<int> &row : matrix_) {
      row.resize(static_cast<size_t>(size));
      row.shrink_to_fit();
    }
  }

  size_t Size() { return matrix_.size(); }
  int &operator()(int i, int j) {
    return matrix_[static_cast<size_t>(i)][static_cast<size_t>(j)];
  }

 private:
  vector<vector<int>> matrix_;
};

class Graph {
 public:
  Graph();
  explicit Graph(int size);
  explicit Graph(const vector<vector<int>> &matrix);
  ~Graph();

  int &operator()(int i, int j);

  void Resize(int size);
  size_t get_size();
  void LoadGraphFromFile(const string &path);
  void ExportGraphToDot(const string &path,
                        const string &graph_name = "somegraph");

  static void Print(Graph &graph) {
    size_t size = graph.get_size();
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        cout << graph(static_cast<int>(i), static_cast<int>(j)) << "\t";
      }
      cout << "\n";
    }
  }

 private:
  AdjacencyMatrix matrix_;
  ifstream *input_file_;
  ofstream *output_file_;
  vector<Node *> nodes_;

  void IsInputFileOpened();
  void IsOutputFileOpened();
  void ReadAdjacencyMatrixSize();
  void ReadAdjacencyMatrix();
  void ReadLineToMatrixRow(string &line, int row);
  void ShiftToNextNumber(string &line, size_t *i);
  void WriteDotWriterObject(const string &graph_name);
  void AddNodesToDotWriterObject(RootGraph &dot_object);
  void AddEdgesToDotWriterObject(RootGraph &dot_object);
  void AddEdgeBetwenNodesToDotWriterObject(int i, int j, RootGraph &dot_object);
};

}  // namespace s21

#endif  // SRC_GRAPH_GRAPH_H_

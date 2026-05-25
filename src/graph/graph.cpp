#include "graph.h"

Graph::Graph(vector<vector<i32>> new_matrix, bool is_directed)
  : matrix(new_matrix), directed(is_directed) {}

i32 Graph::size() const
{
  return matrix.size();
}

bool Graph::isDirected() const
{
  return directed;
}

bool Graph::isWeighted() const
{
  for (const auto& row : matrix) {
    for (i32 value : row) {
      if (value > 1)
        return true;
    }
  }

  return false;
}

bool Graph::hasLoops() const
{
  for (i32 i = 0; i < size(); i++) {
    if (matrix[i][i] != 0)
      return true;
  }

  return false;
}

i32 Graph::edgeCount() const
{
  i32 count = 0;

  for (i32 i = 0; i < size(); i++) {
    for (i32 j = directed ? 0 : i; j < size(); j++) {
      if (matrix[i][j] != 0)
        count++;
    }
  }

  return count;
}

vector<Edge> Graph::edges() const
{
  vector<Edge> result;

  for (i32 i = 0; i < size(); i++) {
    for (i32 j = directed ? 0 : i + 1; j < size(); j++) {
      if (matrix[i][j] != 0)
        result.push_back({i, j, matrix[i][j]});
    }
  }

  return result;
}

vector<i32> Graph::neighbors(i32 vertex) const
{
  vector<i32> result;

  for (i32 i = 0; i < size(); i++) {
    if (matrix[vertex][i] != 0)
      result.push_back(i);
  }

  return result;  
}

const vector<vector<i32>>& Graph::getMatrix() const
{
  return matrix;
}

void Graph::printMatrix() const
{
  for (const auto& row : matrix) {
    for (i32 value : row) {
      cout << value << " ";
    }
    
    cout << endl;
  }
}

void Graph::printInfo() const
{
  cout << "Vertices: " << size() << endl;
  cout << "Edges: " << edgeCount() << endl;
  cout << "Directed: " << (directed ? "yes" : "no") << endl;
  cout << "Weighted: " << (isWeighted() ? "yes" : "no") << endl;
  cout << "Loops: " << (hasLoops() ? "yes" : "no") << endl;

  for (i32 i = 0; i < size(); i++) {
    cout << "deg(" << i + 1 << ") = " << neighbors(i).size() << endl;
  }
}

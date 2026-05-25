#pragma once
#include "base.h"

struct Edge
{
  i32 from;
  i32 to;
  i32 weight;
};

class Graph
{
  private:
    vector<vector<i32>> matrix;
    bool directed;

  public:
    Graph(vector<vector<i32>> new_matrix, bool is_directed = false);

    i32 size() const;
    bool isDirected() const;
    bool isWeighted() const;
    bool hasLoops() const;
    i32 edgeCount() const;

    vector<Edge> edges() const;
    vector<i32> neighbors(i32 vector) const;
    const vector<vector<i32>>& getMatrix() const;

    void printMatrix() const;
    void printInfo() const;
};

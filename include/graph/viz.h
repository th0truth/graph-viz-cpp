#pragma once
#include "graph.h"

class GraphViz {
  public:
    static void writeDot(const Graph& graph, const string& filename);
    static void writeDot(const Graph& graph, const string& filename, const vector<Edge>& highlighted);
};

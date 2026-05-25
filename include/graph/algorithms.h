#pragma once
#include "graph.h"

namespace graph {
  vector<i32> DFS(const Graph& graph, i32 start, bool visualize = false);
  vector<i32> BFS(const Graph& graph, i32 start, bool visualize = false);
  vector<i32> TopologicalSort(const Graph& graph, bool& ok);
  vector<Edge> Kruskal(const Graph& graph, i32& total_weight);
  vector<Edge> Prim(const Graph& graph, i32& total_weight);
  vector<i32> Dijkstra(const Graph& graph, i32 start, vector<i32>& parent);
}

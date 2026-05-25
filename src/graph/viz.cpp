#include "viz.h"

void GraphViz::writeDot(const Graph& graph, const string& filename)
{
  writeDot(graph, filename, {});
}

void GraphViz::writeDot(const Graph& graph, const string& filename, const vector<Edge>& highlighted)
{
  ofstream out(filename);
  set<pair<i32, i32>> marked;

  for (const Edge& edge : highlighted) {
    marked.insert({edge.from, edge.to});
    marked.insert({edge.to, edge.from});
  }

  out << (graph.isDirected() ? "digraph" : "graph") << " G {\n";
  out << "  node [shape=circle];\n";

  string link = graph.isDirected() ? " -> " : " -- ";

  for (const Edge& edge : graph.edges()) {
    bool is_marked = marked.count({edge.from, edge.to});

    out << "  " << edge.from + 1 << link << edge.to + 1;
    out << " [label=\"" << edge.weight << "\"";

    if (is_marked) {
      out << ", color=red, penwidth=3";
    }

    out << "];\n";
  }

  out << "}\n";
}

#include "viz.h"
#include <cmath>

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

void GraphViz::writeSvg(const Graph& graph, const string& filename)
{
  writeSvg(graph, filename, {});
}

void GraphViz::writeSvg(const Graph& graph, const string& filename, const vector<Edge>& highlighted)
{
  ofstream out(filename);
  set<pair<i32, i32>> marked;
  const double pi = acos(-1.0);
  const i32 width = 1280;
  const i32 height = 720;
  const i32 graph_width = 640;
  const i32 center_x = 320;
  const i32 center_y = 360;
  const i32 radius = 230;
  const i32 node_radius = 34;
  i32 n = graph.size();

  for (const Edge& edge : highlighted) {
    marked.insert({edge.from, edge.to});
    marked.insert({edge.to, edge.from});
  }

  vector<pair<double, double>> pos(n);

  for (i32 i = 0; i < n; i++) {
    double angle = -pi / 2.0 + (2.0 * pi * i) / max(1, n);
    pos[i] = {
      center_x + radius * cos(angle),
      center_y + radius * sin(angle)
    };
  }

  out << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width
      << "\" height=\"" << height << "\" viewBox=\"0 0 " << width << " " << height << "\">\n";
  out << "  <rect width=\"100%\" height=\"100%\" fill=\"#0f0f0f\"/>\n";
  out << "  <line x1=\"" << graph_width << "\" y1=\"0\" x2=\"" << graph_width
      << "\" y2=\"" << height << "\" stroke=\"#777\" stroke-width=\"2\"/>\n";

  if (graph.isDirected()) {
    out << "  <defs><marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"8\" refY=\"3\" orient=\"auto\" markerUnits=\"strokeWidth\">"
        << "<path d=\"M0,0 L0,6 L9,3 z\" fill=\"#a0a0a0\"/></marker></defs>\n";
  }

  for (const Edge& edge : graph.edges()) {
    bool is_marked = marked.count({edge.from, edge.to}) > 0;
    double x1 = pos[edge.from].first;
    double y1 = pos[edge.from].second;
    double x2 = pos[edge.to].first;
    double y2 = pos[edge.to].second;
    string color = is_marked ? "#f59f6b" : "#8b8b8b";
    i32 stroke = is_marked ? 7 : 5;

    out << "  <line x1=\"" << x1 << "\" y1=\"" << y1
        << "\" x2=\"" << x2 << "\" y2=\"" << y2
        << "\" stroke=\"" << color << "\" stroke-width=\"" << stroke
        << "\" stroke-linecap=\"round\"";

    if (graph.isDirected()) {
      out << " marker-end=\"url(#arrow)\"";
    }

    out << "/>\n";

    if (graph.isWeighted()) {
      double label_x = (x1 + x2) / 2.0;
      double label_y = (y1 + y2) / 2.0;

      out << "  <rect x=\"" << label_x - 18 << "\" y=\"" << label_y - 15
          << "\" width=\"36\" height=\"24\" rx=\"5\" fill=\"#111\" stroke=\"#555\"/>\n";
      out << "  <text x=\"" << label_x << "\" y=\"" << label_y + 6
          << "\" text-anchor=\"middle\" fill=\"#fff\" font-size=\"16\" font-family=\"monospace\">"
          << edge.weight << "</text>\n";
    }
  }

  for (i32 i = 0; i < n; i++) {
    out << "  <circle cx=\"" << pos[i].first << "\" cy=\"" << pos[i].second
        << "\" r=\"" << node_radius << "\" fill=\"#0f0f0f\" stroke=\"#fff\" stroke-width=\"7\"/>\n";
    out << "  <text x=\"" << pos[i].first << "\" y=\"" << pos[i].second + 13
        << "\" text-anchor=\"middle\" fill=\"#fff\" font-size=\"42\" font-family=\"serif\">"
        << i + 1 << "</text>\n";
  }

  out << "  <text x=\"690\" y=\"70\" fill=\"#fff\" font-size=\"50\" font-family=\"serif\">"
      << "Edge list</text>\n";
  out << "  <text x=\"690\" y=\"115\" fill=\"#f59f6b\" font-size=\"24\" font-family=\"monospace\">"
      << (graph.isDirected() ? "directed graph" : "undirected graph") << "</text>\n";

  i32 y = 170;
  out << "  <text x=\"740\" y=\"" << y << "\" fill=\"#fff\" font-size=\"28\" font-family=\"monospace\">{</text>\n";
  y += 42;

  for (const Edge& edge : graph.edges()) {
    out << "  <text x=\"795\" y=\"" << y << "\" fill=\"#fff\" font-size=\"28\" font-family=\"monospace\">("
        << edge.from + 1 << ", " << edge.to + 1 << ")";

    if (!graph.isDirected()) {
      out << ", (" << edge.to + 1 << ", " << edge.from + 1 << ")";
    }

    if (graph.isWeighted()) {
      out << "  w=" << edge.weight;
    }

    out << "</text>\n";
    y += 38;

    if (y > 660) {
      out << "  <text x=\"795\" y=\"" << y << "\" fill=\"#aaa\" font-size=\"24\" font-family=\"monospace\">...</text>\n";
      break;
    }
  }

  out << "  <text x=\"740\" y=\"" << min(y + 20, height - 35)
      << "\" fill=\"#fff\" font-size=\"28\" font-family=\"monospace\">}</text>\n";
  out << "</svg>\n";
}

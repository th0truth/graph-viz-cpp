#include "algorithms.h"
#include "graph.h"
#include "viz.h"

const string RESET = "\033[0m";
const string DIM = "\033[2m";
const string BOLD = "\033[1m";
const string ACCENT = "\033[38;5;208m";
const string CYAN = "\033[38;5;81m";
const string MUTED = "\033[38;5;245m";
const string OUTPUT_DIR = "output";

void ensureOutputDir()
{
  mkdir(OUTPUT_DIR.c_str(), 0755);
}

string outputPath(const string& filename)
{
  return OUTPUT_DIR + "/" + filename;
}

void clearScreen()
{
  cout << "\033[2J\033[H";
}

string visiblePad(const string& text, i32 width)
{
  i32 length = static_cast<i32>(text.size());

  if (length >= width) {
    return text.substr(0, width);
  }

  return text + string(width - length, ' ');
}

void printBox(const string& title, const vector<string>& lines, i32 width = 72)
{
  cout << ACCENT << "+-" << title << string(max(0, width - static_cast<i32>(title.size()) - 3), '-') << "+" << RESET << endl;

  for (const string& line : lines) {
    cout << ACCENT << "| " << RESET << visiblePad(line, width - 3) << ACCENT << "|" << RESET << endl;
  }

  cout << ACCENT << "+" << string(width - 2, '-') << "+" << RESET << endl;
}

i32 readInt(const string& prompt, i32 min_value, i32 max_value)
{
  i32 value;

  while (true) {
    cout << prompt;

    if (cin >> value && value >= min_value && value <= max_value) {
      return value;
    }

    cout << "Invalid value. Try again." << endl;
    cin.clear();
    cin.ignore(10000, '\n');
  }
}

bool readYesNo(const string& prompt)
{
  char answer;

  while (true) {
    cout << prompt << " (y/n): ";

    if (cin >> answer) {
      if (answer == 'y' || answer == 'Y') return true;
      if (answer == 'n' || answer == 'N') return false;
    }

    cout << "Enter y or n." << endl;
  }
}

vector<vector<i32>> sampleMatrix()
{
  return {
    {0, 9, 75, 0, 0},
    {9, 0, 95, 19, 42},
    {75, 95, 0, 51, 66},
    {0, 19, 51, 0, 31},
    {0, 42, 66, 31, 0}
  };
}

vector<vector<i32>> sampleDAGMatrix()
{
  return {
    {0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0}
  };
}

vector<vector<i32>> readMatrix(bool directed)
{
  i32 n = readInt("Enter vertex count: ", 1, 100);
  vector<vector<i32>> matrix(n, vector<i32>(n, 0));

  cout << "Enter adjacency matrix. Use 0 if there is no edge." << endl;

  for (i32 i = 0; i < n; i++) {
    for (i32 j = 0; j < n; j++) {
      matrix[i][j] = readInt("matrix[" + to_string(i + 1) + "][" + to_string(j + 1) + "]: ", 0, 1000000);
    }
  }

  if (!directed) {
    for (i32 i = 0; i < n; i++) {
      for (i32 j = i + 1; j < n; j++) {
        if (matrix[i][j] != matrix[j][i]) {
          cout << "Undirected graph requires symmetry. Using upper triangle value for edge "
               << i + 1 << " - " << j + 1 << "." << endl;

          matrix[j][i] = matrix[i][j];
        }
      }
    }
  }

  return matrix;
}

vector<vector<i32>> generateMatrix(bool directed)
{
  i32 n = readInt("Enter vertex count: ", 1, 100);
  i32 max_weight = readInt("Enter max edge weight: ", 1, 1000000);
  i32 density = readInt("Enter edge density percent: ", 0, 100);

  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<i32> chance(1, 100);
  uniform_int_distribution<i32> weight(1, max_weight);
  vector<vector<i32>> matrix(n, vector<i32>(n, 0));

  for (i32 i = 0; i < n; i++) {
    for (i32 j = directed ? 0 : i + 1; j < n; j++) {
      if (i == j) continue;

      if (chance(generator) <= density) {
        matrix[i][j] = weight(generator);

        if (!directed) {
          matrix[j][i] = matrix[i][j];
        }
      }
    }
  }

  return matrix;
}

string graphKind(const Graph& graph)
{
  return string(graph.isDirected() ? "directed" : "undirected") + ", "
       + string(graph.isWeighted() ? "weighted" : "unweighted");
}

vector<string> graphSummaryLines(const Graph& graph)
{
  vector<string> lines;

  lines.push_back("Type: " + graphKind(graph));
  lines.push_back("Vertices: " + to_string(graph.size()));
  lines.push_back("Edges: " + to_string(graph.edgeCount()));
  lines.push_back("Loops: " + string(graph.hasLoops() ? "yes" : "no"));
  lines.push_back("Output folder: output/");
  lines.push_back("Exports: graph.svg, graph.dot, graph.png when Graphviz is installed");

  return lines;
}

vector<string> matrixLines(const Graph& graph)
{
  vector<string> lines;
  const vector<vector<i32>>& matrix = graph.getMatrix();

  for (const auto& row : matrix) {
    ostringstream out;

    for (i32 value : row) {
      out << setw(4) << value;
    }

    lines.push_back(out.str());
  }

  return lines;
}

vector<string> edgeListLines(const Graph& graph, i32 max_lines = 14)
{
  vector<string> lines;
  vector<Edge> edges = graph.edges();

  if (edges.empty()) {
    lines.push_back("No edges.");
    return lines;
  }

  for (i32 i = 0; i < static_cast<i32>(edges.size()) && i < max_lines; i++) {
    const Edge& edge = edges[i];
    ostringstream out;

    out << "(" << edge.from + 1 << ", " << edge.to + 1 << ")";

    if (!graph.isDirected()) {
      out << ", (" << edge.to + 1 << ", " << edge.from + 1 << ")";
    }

    if (graph.isWeighted()) {
      out << "  w=" << edge.weight;
    }

    lines.push_back(out.str());
  }

  if (static_cast<i32>(edges.size()) > max_lines) {
    lines.push_back("...");
  }

  return lines;
}

string normalizeCommand(string command)
{
  for (char& ch : command) {
    ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
  }

  return command;
}

i32 readStartVertex(const Graph& graph, const string& algorithm_name)
{
  return readInt("Start vertex for " + algorithm_name + ": ", 1, graph.size()) - 1;
}

void printOrder(const string& title, const vector<i32>& order)
{
  cout << title << ": ";

  for (i32 vertex : order) {
    cout << vertex + 1 << " ";
  }

  cout << endl;
}

void printEdges(const vector<Edge>& edges)
{
  for (const Edge& edge : edges) {
    cout << edge.from + 1 << " - " << edge.to + 1 << " = " << edge.weight << endl;
  }
}

void showGraphInfo(const Graph& graph)
{
  clearScreen();
  printBox("Graph Algorithm Visualizer", graphSummaryLines(graph));
  cout << endl;
  printBox("Adjacency Matrix", matrixLines(graph));
  cout << endl;
  printBox("Edge List", edgeListLines(graph));

  ensureOutputDir();
  GraphViz::writeDot(graph, outputPath("graph.dot"));
  GraphViz::writeSvg(graph, outputPath("graph.svg"));
}

void runDFS(const Graph& graph)
{
  i32 start = readStartVertex(graph, "DFS");
  cout << endl << "Depth-first search" << endl;
  printOrder("DFS", graph::DFS(graph, start, true));
}

void runBFS(const Graph& graph)
{
  i32 start = readStartVertex(graph, "BFS");
  cout << endl << "Breadth-first search" << endl;
  printOrder("BFS", graph::BFS(graph, start, true));
}

void runTopologicalSort(const Graph& graph)
{
  cout << endl << "Topological sort" << endl;
  
  if (graph.isDirected()) {
    bool topo_ok = false;
    vector<i32> topo = graph::TopologicalSort(graph, topo_ok);

    if (topo_ok) {
      printOrder("Topological order", topo);
    } else {
      cout << "Graph has a cycle. Topological sorting is impossible." << endl;
    }
  } else {
    Graph dag(sampleDAGMatrix(), true);
    bool topo_ok = false;
    vector<i32> topo = graph::TopologicalSort(dag, topo_ok);

    cout << "Current graph is undirected, so sample DAG is used." << endl;
    printOrder("Topological order", topo);
  }
}

void runKruskal(const Graph& graph)
{
  if (graph.isDirected()) {
    cout << "Kruskal requires an undirected graph." << endl;
    return;
  }

  cout << endl << "Kruskal minimum spanning tree" << endl;

  i32 total_weight = 0;
  vector<Edge> mst = graph::Kruskal(graph, total_weight);
  printEdges(mst);
  cout << "Total: " << total_weight << endl;
  ensureOutputDir();
  GraphViz::writeDot(graph, outputPath("kruskal.dot"), mst);
  GraphViz::writeSvg(graph, outputPath("kruskal.svg"), mst);
}

void runPrim(const Graph& graph)
{
  if (graph.isDirected()) {
    cout << "Prim requires an undirected graph." << endl;
    return;
  }

  cout << endl << "Prim minimum spanning tree" << endl;

  i32 total_weight = 0;
  vector<Edge> mst = graph::Prim(graph, total_weight);
  printEdges(mst);
  cout << "Total: " << total_weight << endl;
  ensureOutputDir();
  GraphViz::writeDot(graph, outputPath("prim.dot"), mst);
  GraphViz::writeSvg(graph, outputPath("prim.svg"), mst);
}

void runDijkstra(const Graph& graph)
{
  i32 start = readStartVertex(graph, "Dijkstra");

  cout << endl << "Dijkstra shortest paths" << endl;

  vector<i32> parent;
  vector<i32> dist = graph::Dijkstra(graph, start, parent);

  for (i32 i = 0; i < static_cast<i32>(dist.size()); i++) {
    cout << start + 1 << " -> " << i + 1 << " = ";

    if (dist[i] == INT_MAX) {
      cout << "INF" << endl;
    } else {
      cout << dist[i] << endl;
    }
  }
}

void exportVisualization(const Graph& graph)
{
  ensureOutputDir();
  GraphViz::writeDot(graph, outputPath("graph.dot"));
  GraphViz::writeSvg(graph, outputPath("graph.svg"));
  i32 png_status = system("dot -Tpng output/graph.dot -o output/graph.png >/dev/null 2>&1");

  cout << endl;
  vector<string> lines = {
    "Wrote output/graph.svg",
    "Wrote output/graph.dot",
    "Open output/graph.svg in a browser for the cleanest built-in visualization."
  };

  if (png_status == 0) {
    lines.push_back("Wrote output/graph.png using Graphviz.");
  } else {
    lines.push_back("PNG was skipped because Graphviz dot is not available.");
    lines.push_back("Install Graphviz and run: dot -Tpng output/graph.dot -o output/graph.png");
  }

  printBox("Visualization Export", lines);
}

void runAll(const Graph& graph)
{
  showGraphInfo(graph);
  runDFS(graph);
  runBFS(graph);
  runTopologicalSort(graph);
  runKruskal(graph);
  runPrim(graph);
  runDijkstra(graph);
}

void printOperationMenu()
{
  cout << endl;
  printBox("Commands", {
    "info      print graph properties, matrix, and edge list",
    "viz       export output/graph.svg and output/graph.dot",
    "dfs       depth-first search",
    "bfs       breadth-first search",
    "topo      topological sort",
    "kruskal   minimum spanning tree by Kruskal",
    "prim      minimum spanning tree by Prim",
    "dijkstra  shortest paths by Dijkstra",
    "all       run every operation",
    "exit      quit"
  });
}

void runInteractiveShell(const Graph& graph)
{
  string command;

  clearScreen();
  printBox("Graph Algorithm Visualizer", graphSummaryLines(graph));

  while (true) {
    printOperationMenu();
    cout << ACCENT << "Operation" << RESET << " > ";
    cin >> command;

    command = normalizeCommand(command);

    if (command == "info") {
      showGraphInfo(graph);
    } else if (command == "viz" || command == "visualize" || command == "export") {
      exportVisualization(graph);
    } else if (command == "dfs") {
      runDFS(graph);
    } else if (command == "bfs") {
      runBFS(graph);
    } else if (command == "topo" || command == "topological") {
      runTopologicalSort(graph);
    } else if (command == "kruskal") {
      runKruskal(graph);
    } else if (command == "prim") {
      runPrim(graph);
    } else if (command == "dijkstra") {
      runDijkstra(graph);
    } else if (command == "all") {
      runAll(graph);
    } else if (command == "exit" || command == "quit") {
      break;
    } else {
      cout << "Unknown operation. Try: info, viz, dfs, bfs, topo, kruskal, prim, dijkstra, all, exit." << endl;
    }
  }
}

int main()
{
  clearScreen();
  printBox("Graph Algorithm Visualizer", {
    "1  Use sample matrix",
    "2  Enter matrix manually",
    "3  Generate random matrix"
  });

  i32 choice = readInt("Choose input mode: ", 1, 3);
  bool directed = false;
  vector<vector<i32>> matrix;

  if (choice == 1) {
    matrix = sampleMatrix();
  } else {
    directed = readYesNo("Is graph directed?");

    if (choice == 2) {
      matrix = readMatrix(directed);
    } else {
      matrix = generateMatrix(directed);
    }
  }

  Graph graph(matrix, directed);
  runInteractiveShell(graph);

  cout << endl << "Visualization files were written to output/ when requested." << endl;

  return 0;
}

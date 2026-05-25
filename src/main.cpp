#include "algorithms.h"
#include "graph.h"
#include "viz.h"

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
  cout << endl << "Graph representation" << endl;
  graph.printMatrix();
  graph.printInfo();
  GraphViz::writeDot(graph, "graph.dot");
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
  GraphViz::writeDot(graph, "kruskal.dot", mst);
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
  GraphViz::writeDot(graph, "prim.dot", mst);
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
  cout << "Available operations:" << endl;
  cout << "  info      - print matrix and graph properties" << endl;
  cout << "  dfs       - depth-first search" << endl;
  cout << "  bfs       - breadth-first search" << endl;
  cout << "  topo      - topological sort" << endl;
  cout << "  kruskal   - minimum spanning tree by Kruskal" << endl;
  cout << "  prim      - minimum spanning tree by Prim" << endl;
  cout << "  dijkstra  - shortest paths by Dijkstra" << endl;
  cout << "  all       - run every operation" << endl;
  cout << "  exit      - quit" << endl;
}

void runInteractiveShell(const Graph& graph)
{
  string command;

  while (true) {
    printOperationMenu();
    cout << "Operation: ";
    cin >> command;

    command = normalizeCommand(command);

    if (command == "info") {
      showGraphInfo(graph);
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
      cout << "Unknown operation. Try: info, dfs, bfs, topo, kruskal, prim, dijkstra, all, exit." << endl;
    }
  }
}

int main()
{
  cout << "Graph Algorithm Visualizer" << endl;
  cout << "1 - Use sample matrix" << endl;
  cout << "2 - Enter matrix manually" << endl;
  cout << "3 - Generate random matrix" << endl;

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

  cout << endl << "DOT files were written to the current directory." << endl;

  return 0;
}

# Graph Algorithm Visualizer

A compact C++17 project for working with graph representations and classic graph algorithms from the command line. The codebase is organized with separate headers and source files, following a small modular layout similar to data-structure and algorithm study projects.

## Features

- Adjacency matrix representation for directed and undirected graphs
- Manual matrix entry from the terminal
- Random weighted matrix generation with configurable density
- Built-in sample graph for quick testing
- Graph property report:
  - vertex count
  - edge count
  - directed or undirected type
  - weighted or unweighted type
  - loop detection
  - vertex degrees
- Depth-first search
- Breadth-first search
- Topological sort with cycle detection
- Kruskal minimum spanning tree
- Prim minimum spanning tree
- Dijkstra shortest paths
- Graphviz `.dot` export for graph and tree visualization

## Project Structure

```text
.
├── include/
│   ├── base.h
│   └── graph/
│       ├── algorithms.h
│       ├── graph.h
│       └── viz.h
├── src/
│   ├── main.cpp
│   └── graph/
│       ├── algorithms.cpp
│       ├── graph.cpp
│       └── viz.cpp
├── Makefile
└── README.md
```

## Build

```bash
make
```

The executable is created at:

```bash
./build/main
```

Clean build artifacts:

```bash
make clean
```

## Run

```bash
./build/main
```

Input modes:

```text
1 - Use sample matrix
2 - Enter matrix manually
3 - Generate random matrix
```

After creating a graph, choose an operation:

```text
info
dfs
bfs
topo
kruskal
prim
dijkstra
all
exit
```

## Matrix Input

For manual input, enter the number of vertices and then each matrix cell. Use `0` when no edge exists.

Example for an undirected weighted graph with three vertices:

```text
0 1 4
1 0 2
4 2 0
```

For undirected graphs, the program normalizes asymmetric input by using the upper-triangle value for each pair.

## Visualization

The application writes Graphviz `.dot` files for supported operations:

```text
graph.dot
kruskal.dot
prim.dot
```

Render a `.dot` file to PNG with Graphviz:

```bash
dot -Tpng graph.dot -o graph.png
```

## License

This project is available under the [MIT License](LICENSE).

#include "algorithms.h"

namespace {
  class DSU {
    vector <i32> parent;

    public:
      DSU(i32 n)
      {
        parent.resize(n);

        for (i32 i = 0; i < n; i++) {
          parent[i] = i;
        }
      }

      i32 find(i32 v)
      {
        if (parent[v] == v)
          return v;
        return parent[v] = find(parent[v]);
      };

      bool unite(i32 a, i32 b)
      {
        a = find(a);
        b = find(b);

        if (a == b)
          return false;
      
        parent[b] = a;
        return true;  
      }
  };
};

namespace graph {
  vector<i32> DFS(const Graph& graph, i32 start, bool visualize)
  {
    vector<i32> order;

    if (!graph.isValidVertex(start)) {
      return order;
    }

    vector<bool> used(graph.size(), false);
    function<void(i32)> dfs = [&](i32 v) {
      used[v] = true;
      order.push_back(v);
      TRACE(visualize, "DFS visit: " << v + 1 << endl);
      for (i32 to : graph.neighbors(v)) {
        if (!used[to]) dfs(to);
      }
    };
    dfs(start);
    return order;
  }
  vector<i32> BFS(const Graph& graph, i32 start, bool visualize)
  {
    vector<i32> order;

    if (!graph.isValidVertex(start)) {
      return order;
    }

    vector<bool> used(graph.size(), false);
    queue<i32> q;
    used[start] = true;
    q.push(start);
    while (!q.empty()) {
      i32 v = q.front();
      q.pop();
      order.push_back(v);
      TRACE(visualize, "BFS visit: " << v + 1 << endl);
      for (i32 to : graph.neighbors(v)) {
        if (!used[to]) {
          used[to] = true;
          q.push(to);
        }
      }
    }
    return order;
  }
  vector<i32> TopologicalSort(const Graph& graph, bool& ok)
  {
    vector<i32> color(graph.size(), 0);
    vector<i32> order;
    ok = true;
    function<void(i32)> dfs = [&](i32 v) {
      color[v] = 1;
      for (i32 to : graph.neighbors(v)) {
        if (color[to] == 1) {
          ok = false;
          return;
        }
        if (color[to] == 0) dfs(to);
      }
      color[v] = 2;
      order.push_back(v);
    };
    for (i32 i = 0; i < graph.size(); i++) {
      if (color[i] == 0) dfs(i);
      if (!ok) break;
    }
    reverse(order.begin(), order.end());
    return order;
  }

  vector<Edge> Kruskal(const Graph& graph, i32& total_weight)
  {
    vector<Edge> edges = graph.edges();
    vector<Edge> mst;
    DSU dsu(graph.size());
    total_weight = 0;
  
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
      return a.weight < b.weight;
    });
  
    for (const Edge& edge : edges) {
      if (dsu.unite(edge.from, edge.to)) {
        mst.push_back(edge);
        total_weight += edge.weight;
      }
    }
    return mst;
  }

  vector<Edge> Prim(const Graph& graph, i32& total_weight)
  {
    i32 n = graph.size();
    vector<i32> key(n, INT_MAX);
    vector<i32> parent(n, -1);
    vector<bool> used(n, false);
    vector<Edge> mst;

    if (n == 0) {
      total_weight = 0;
      return mst;
    }

    key[0] = 0;
    total_weight = 0;
  
    for (i32 step = 0; step < n; step++) {
      i32 v = -1;
      for (i32 i = 0; i < n; i++) {
        if (!used[i] && (v == -1 || key[i] < key[v])) {
          v = i;
        }
      }
 
      if (v == -1) break;
      used[v] = true;
      if (parent[v] != -1) {
        mst.push_back({parent[v], v, key[v]});
        total_weight += key[v];
      }
 
      for (i32 to : graph.neighbors(v)) {
        i32 weight = graph.getMatrix()[v][to];
        if (!used[to] && weight < key[to]) {
          key[to] = weight;
          parent[to] = v;
        }
      }
    }
    return mst;
  }
  
  vector<i32> Dijkstra(const Graph& graph, i32 start, vector<i32>& parent)
  {
    i32 n = graph.size();
    vector<i32> dist(n, INT_MAX);
    vector<bool> used(n, false);
    parent.assign(n, -1);

    if (!graph.isValidVertex(start)) {
      return dist;
    }

    dist[start] = 0;
 
    for (i32 step = 0; step < n; step++) {
      i32 v = -1;
      for (i32 i = 0; i < n; i++) {
        if (!used[i] && (v == -1 || dist[i] < dist[v])) {
          v = i;
        }
      }
      if (v == -1 || dist[v] == INT_MAX) break;
      used[v] = true;
 
      for (i32 to : graph.neighbors(v)) {
        i32 weight = graph.getMatrix()[v][to];
        if (dist[v] + weight < dist[to]) {
          dist[to] = dist[v] + weight;
          parent[to] = v;
        }
      }
    }
 
    return dist;
  }

  vector<i32> BellmanFord(const Graph& graph, i32 start, vector<i32>& parent, bool& has_negative_cycle)
  {
    i32 n = graph.size();
    vector<i32> dist(n, INT_MAX);
    parent.assign(n, -1);
    has_negative_cycle = false;

    if (!graph.isValidVertex(start)) {
      return dist;
    }

    dist[start] = 0;

    for (i32 step = 0; step < n - 1; step++) {
      bool changed = false;

      for (i32 from = 0; from < n; from++) {
        if (dist[from] == INT_MAX) {
          continue;
        }

        for (i32 to : graph.neighbors(from)) {
          i32 weight = graph.getMatrix()[from][to];

          if (dist[from] + weight < dist[to]) {
            dist[to] = dist[from] + weight;
            parent[to] = from;
            changed = true;
          }
        }
      }

      if (!changed) {
        break;
      }
    }

    for (i32 from = 0; from < n; from++) {
      if (dist[from] == INT_MAX) {
        continue;
      }

      for (i32 to : graph.neighbors(from)) {
        i32 weight = graph.getMatrix()[from][to];

        if (dist[from] + weight < dist[to]) {
          has_negative_cycle = true;
          return dist;
        }
      }
    }

    return dist;
  }

  vector<vector<i32>> FloydWarshall(const Graph& graph, bool& has_negative_cycle)
  {
    const i32 inf = INT_MAX / 4;
    i32 n = graph.size();
    vector<vector<i32>> dist(n, vector<i32>(n, inf));
    const vector<vector<i32>>& matrix = graph.getMatrix();
    has_negative_cycle = false;

    for (i32 i = 0; i < n; i++) {
      dist[i][i] = 0;

      for (i32 j = 0; j < n; j++) {
        if (matrix[i][j] != 0) {
          dist[i][j] = matrix[i][j];
        }
      }
    }

    for (i32 through = 0; through < n; through++) {
      for (i32 from = 0; from < n; from++) {
        if (dist[from][through] == inf) {
          continue;
        }

        for (i32 to = 0; to < n; to++) {
          if (dist[through][to] == inf) {
            continue;
          }

          i32 candidate = dist[from][through] + dist[through][to];

          if (candidate < dist[from][to]) {
            dist[from][to] = candidate;
          }
        }
      }
    }

    for (i32 i = 0; i < n; i++) {
      if (dist[i][i] < 0) {
        has_negative_cycle = true;
        break;
      }
    }

    for (i32 i = 0; i < n; i++) {
      for (i32 j = 0; j < n; j++) {
        if (dist[i][j] == inf) {
          dist[i][j] = INT_MAX;
        }
      }
    }

    return dist;
  }
}

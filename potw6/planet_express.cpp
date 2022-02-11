// Idea: SCC + Dijkstra. Find teleportation networks as SCCs. Connect every
//       nodes in an SCC --- 0 ---> a new virtual node A --- t(scc)-1 ---> 
//       a new virtual node B --- 0 --- > nodes in the SCC. Construct the graph
//       reversely and run Dijkstra from the delivery destination.
#include <iostream>
#include <vector>
#include <unordered_set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
using namespace std;
using namespace boost;

struct teleportable_t { typedef vertex_property_tag kind; };

using Graph = adjacency_list<vecS, vecS, directedS, 
  property<teleportable_t, bool>, property<edge_weight_t, long long>>;
using Vertex = graph_traits<Graph>::vertex_descriptor;
using Edge = graph_traits<Graph>::edge_descriptor;
using Vertex_It = graph_traits<Graph>::vertex_iterator;
using Edge_It = graph_traits<Graph>::edge_iterator;

void work_on_test_case() {
  
  int n, m, k, T;
  cin >> n >> m >> k >> T;
  
  Graph G(n);  // construct the graph in reverse
  
  auto teleportable = boost::get(teleportable_t(), G);
  
  for (int u = 0; u < n; u++) teleportable[u] = false;
  
  for (int i = 0; i < T; i++) {
    int t;  
    cin >> t;
    teleportable[t] = true;
  }
  
  for (int i = 0; i < m; i++) {
    int u, v;
    long long c;
    cin >> u >> v >> c;
    add_edge(v, u, {c}, G);  // reversed
  }
  
  assert(num_vertices(G) == n);
  
  // Get SCCs
  std::vector<Vertex> SCC(n);
  int nSCC = strong_components(G, 
    make_iterator_property_map(SCC.begin(), get(vertex_index, G)));
    
  // Connect T vertices to/from virtual nodes
  // Additional vertices are created automatically
  std::vector<long long> t(nSCC, 0);
  for (int u = 0; u < n /* use original count*/; u++) {
    if (teleportable[u]) {
      Vertex scc = SCC[u];
      add_edge(n + scc * 2, u, {0}, G);  // reversed
      add_edge(u, n + scc * 2 + 1, {0}, G);  // reversed
      t[scc]++;
    }
  }
  
  // Connect virtual node pairs
  for (int scc = 0; scc < nSCC; scc++) {
    add_edge(n + scc * 2 + 1, n + scc * 2, {t[scc] - 1}, G);
  }
  
  // Find the shortest path
  std::vector<long long> dist(n + nSCC * 2);
  dijkstra_shortest_paths(G, vertex(n - 1, G),
    distance_map(boost::make_iterator_property_map(
      dist.begin(), get(boost::vertex_index, G))));
  
  long long min_dist = 1000000;
  for (int u = 0; u < k; u++) {
    if (dist[u] < min_dist) min_dist = dist[u];
  }
  
  if (min_dist >= 1000000) {
    cout << "no" << endl;
  } else {
    cout << min_dist << endl;
  }
  
}

int main() {
  
  ios::sync_with_stdio(false);
  
  int t; cin >> t;
  for (int g = 0; g < t; g++) {
    work_on_test_case();
  }
  
  return 0;
}
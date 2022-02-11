// Idea: simple iteration (someone says it's DP, but I think it does not have typical DP characteristic).
// Complexity: O(k(n+m)). It turns out to work with proper implementation.

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::unordered_map;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, long> >     weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator          out_edge_it;

static void testcase() {
  int n, m, k;
  long x;
  cin >> n >> m >> x >> k;
  
  vector<bool> is_sink(n, true);
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  // Store edges to ws first and preserve only one edge for any u -> v with
  // the highest score
  map<std::pair<int, int>, long> ws;
  for (int i = 0; i < m; i++) {
    int u, v;
    long p;
    cin >> u >> v >> p;
    
    is_sink[u] = false;
    
    auto it = ws.find({u, v});
    if (it == ws.end()) {
      ws[{u, v}] = p;
    } else {
      it->second = std::max(it->second, p);
    }
  }
  
  // Construct the graph
  for (const auto &it : ws) {
    edge_desc e = boost::add_edge(it.first.first, it.first.second, G).first; 
    weights[e] = it.second;
  }
  
  // NOTE: unordered_map leads to timeout
  vector<long> score(n, -1);
  score[0] = 0;
  
  for (int step = 1; step <= k; step++) {
    vector<long> new_score(n, -1);
    
    for (int u = 0; u < n; u++) {
      const long s = score[u];
      if (s == -1) continue;
      
      
      out_edge_it oe_beg, oe_end;
      for (boost::tie(oe_beg, oe_end) = boost::out_edges(u, G); oe_beg != oe_end; ++oe_beg) {
        int v = boost::target(*oe_beg, G);
        
        if (is_sink[v]) {
          v = 0;
        }
        
        const long new_s = s + weights[*oe_beg];
        
        if (new_s >= x) {
          cout << step << endl;
          return;
        }
        
        new_score[v] = std::max(new_score[v], new_s);
      }
    }
    
    score = std::move(new_score);
  }
  
  cout << "Impossible" << endl;
  
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
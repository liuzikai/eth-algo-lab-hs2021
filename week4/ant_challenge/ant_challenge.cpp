// Idea: BFS + Dijkstra. Construct a graph of the forest. BFS from each hive 
//       and update the weight of each edge to be the smallest one among all 
//       species. Then run Dijkstra from s to t.
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#define MAX_DIST 1000000
using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator          out_edge_it;


static int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

static void explore(weighted_graph &G, std::map<edge_desc, int> &w, int hive) {
  int n = boost::num_vertices(G);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  vector<bool> visited(n, false);
  
  typedef std::pair<int, edge_desc> entry;  // edge length, target node
  std::priority_queue<entry, std::vector<entry>, std::greater<entry>> q;
  
  out_edge_it oe_beg, oe_end;
  for (boost::tie(oe_beg, oe_end) = boost::out_edges(hive, G); oe_beg != oe_end; ++oe_beg) { 
    q.push(entry{w[*oe_beg], *oe_beg});
  }
  visited[hive] = true;
    
  while (!q.empty()) {
    edge_desc e = q.top().second;
    q.pop();
    int u = boost::target(e, G);
    if (visited[u]) continue;
    
    if (w[e] < weights[e]) {
      weights[e] = w[e];
    }
    
    for (boost::tie(oe_beg, oe_end) = boost::out_edges(u, G); oe_beg != oe_end; ++oe_beg) { 
      int v = boost::target(*oe_beg, G);
      if (!visited[v]) {
        q.push(entry{w[*oe_beg], *oe_beg});
      }
    }
    
    visited[u] = true;
  }
}

static void testcase() {
  int n, e, s, a, b;
  cin >> n >> e >> s >> a >> b;
  
  weighted_graph g(n);
  weight_map weights = boost::get(boost::edge_weight, g);
  
  std::vector<std::map<edge_desc, int>> w(s);
  for (int i = 0; i < e; i++) {
    int t1, t2;
    cin >> t1 >> t2;
    edge_desc e = boost::add_edge(t1, t2, g).first;
    weights[e] = MAX_DIST;
    for (int j = 0; j < s; j++) {
      int x;
      cin >> x;
      w[j][e] = x;
    }
  }
  
  for (int i = 0; i < s; i++) {
    int h;
    cin >> h;
    explore(g, w[i], h);
  }
  
  cout << dijkstra_dist(g, a, b) << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
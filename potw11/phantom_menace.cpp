// Idea: split each astronomical object into two nodes: in-edges => i * 2 -> i * 2 + 1 => out-edges.
//       every edge has the capacity of 1. Find the minimal cut (maximal flow) of the graph.
// Complexity: O(k(n+m)). It turns out to work with proper implementation.
// Note: push_relabel_max_flow O(n^3) turns out to be fast enough...
//       edmonds_karp_max_flow O(nmU) where U is 1 seems more reasonable, but turns out to be slower...

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

static void testcase() {
  int n, m, s, d;
  cin >> n >> m >> s >> d;
  
  graph G(n * 2);
  edge_adder adder(G);
  for (int i = 0; i < n; i++) {
    adder.add_edge(i * 2, i * 2 + 1, 1);
  }
  
  for (int t = 0; t < m; t++) {
    int i, j;
    cin >> i >> j;
    adder.add_edge(i * 2 + 1, j * 2, 1);
  }
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  for (int i = 0; i < s; i++) {
    int v;
    cin >> v;
    adder.add_edge(v_source, v * 2, 1);
  }
  for (int i = 0; i < d; i++) {
    int v;
    cin >> v;
    adder.add_edge(v * 2 + 1, v_sink, 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
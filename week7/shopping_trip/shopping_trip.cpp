// Idea: max flow on undirected graph (reverse edge has the same capacity).
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


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
    c_map[rev_e] = capacity; // reverse edge has capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n, m, s;
  cin >> n >> m >> s;
  
  graph G(n);
  edge_adder adder(G);
  
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for (int i = 0; i < s; i++) {
    int p;
    cin >> p;
    adder.add_edge(p, v_sink, 1);
  }
  
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    adder.add_edge(u, v, 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, 0, v_sink);
  
  cout << (flow == s ? "yes" : "no") << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
// Idea: min cut max flow.
// Graph: source === positive balance ===> cities & debts === negative balance ===> sink
// If there is a positive balance edge to city c that has residual capacity, 
// it means that there is a cut containing c, but there is exceeded balance for 
// it. It doesn't matter how c fulfills its debts, either from its own supply or 
// from the other cities in the union. If another city supplies, it will be
// included in the union during the BFS on the residual graph.
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

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

#define MAX_N 1000
int b[MAX_N];

void testcase() {
  int n, m;
  cin >> n >> m;
  
  graph G(n + 2);
  edge_adder adder(G);
  const vertex_desc v_source = n;
  const vertex_desc v_sink = n + 1;
  
  int positive_sum = 0;
  for (int i = 0; i < n; i++) {
    cin >> b[i];
    if (b[i] > 0) {
      adder.add_edge(v_source, i, b[i]);
      positive_sum += b[i];
    } else {
      adder.add_edge(i, v_sink, -b[i]);
    }
  }
  
  for (int k = 0; k < m; k++) {
    int i, j, d;
    cin >> i >> j >> d;
    adder.add_edge(i, j, d);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

  // positive_sum is the sum of capacity from v_source
  cout << (flow < positive_sum ? "yes" : "no") << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
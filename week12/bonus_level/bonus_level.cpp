// Idea: min cost max flow. Two flow from upper left to lower right to simulate
//       going down and back. Split every node into IN & OUT. Two edges from IN
//       to OUT, one with cost -a, another with cost 0. To eliminate negative
//       weight, offset IN-OUT edges by MAX_COIN. Every flow go through (2n - 1)
//       nodes, so the result is MAX_COIN * (2n - 1) - cost.
// Note: it's not correct to find a optimal single path, and then find another
//       on the remaining coins. Consider the following graph:
//           1 2
//           2 3
//           2 2
//       Two paths can collect all the coins, while single optimial path may
//       take 1 down 2 right 3 down 2, and the remaining 2's cannot be collected
//       by another path.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

#define IN(i, j) (((i) * n + (j)) * 2)
#define OUT(i, j) (((i) * n + (j)) * 2 + 1)
#define MAX_COIN 100

void testcase() {
  int n;
  cin >> n;
  
  graph G(n * n * 2);
  edge_adder adder(G);  
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int a;
      cin >> a;
      
      adder.add_edge(IN(i, j), OUT(i, j), 1, MAX_COIN - a);
      adder.add_edge(IN(i, j), OUT(i, j), 1, MAX_COIN);
      if (i < n - 1) adder.add_edge(OUT(i, j), IN(i + 1, j), 2, 0);
      if (j < n - 1) adder.add_edge(OUT(i, j), IN(i, j + 1), 2, 0);
    }
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, IN(0, 0), OUT(n - 1, n - 1));
  int cost = boost::find_flow_cost(G);
  
  cout << (MAX_COIN * (2 * n - 1)) * 2 - cost << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
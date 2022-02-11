// Idea: directed minimum cut (max flow). It's better to think in the reversed
//       way. Assume the solution is a parition of S and T, where S + T = V.
//       The s-t max flow gives us a min cut where S contains s and T contains 
//       t. The problem can be solved if we accidentially find a s in the 
//       solution S and a t in the solution T. Every node is either in S or T.
//       If we put all nodes in order in a ring (0, 1, 2, ..., n-1, back to 0),
//       since at least one node is in T, there must be a point where 
//       i in S and (i + 1) % n in T (the same reasoning applies if S and T
//       are swapped).
// Note: initially I was trying to prove: given a s-t min cut, using any other 
//       vertex s' in S as s (or any other vertex in T as t) gives the SAME 
//       solution. It is not the case! Consider the following graph:
//           s'
//           ^
//           |
//          0/1
//           |
//           s --- 1/1 ---> t
//       BFS on the residual graph from s gives S = {s, s'}, but s'-t min cut
//       is 0 rather than 1! This cut actually doesn't contains s, so it needs
//       not to be the same as the s-t min cut. But it does not disturb the 
//       reasoning above. We are only interested in the solution (S, T), and
//       the solution can always be derived as long as we find a s in S and a t
//       in T.

#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor   edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

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

void testcase() {
  int n, m;
  cin >> n >> m;
  
  graph G(n);
  edge_adder adder(G);
  
  for (int i = 0; i < m; i++) {
    int a, b, c;
    cin >> a >> b >> c;
    adder.add_edge(a, b, c);
  }
  
  int result = 5000000;
  for (int s = 0; s < n; s++) {
    long flow = boost::push_relabel_max_flow(G, s, (s + 1) % n);
    if (flow < result) result = flow;
  }
  
  cout << result << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
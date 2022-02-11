// Idea: DFS + greedy. The number of gold at each chamber actually doesn't 
//       matter, as the problem says the gold never runs out before collection
//       even in the worse case. I formulate the result as: sum of gold - lost
//       of gold, and the solution is to minimize lost.
//
//       The graph is a tree rooted at 0. Every chamber (node) must be visited 
//       while every tunnel (edge) is visited twice. Therefore every subtree
//       must be visited at once. For a tree rooted at u, the lost in the
//       tree when visiting the subtree rooted at v is:
//
//           # of remaining nodes * T + lost(v) + # of nodes in v * l(u, v),
//
//       where T is the time visiting the subtree v. T = 2 * (edge length sum 
//       + l(u, v)). l(u, v) is the length of edge u-v. The total lost is the
//       sum above for every subtree of u.
//
//       The later two components depends on only the graph and are fixed. The 
//       problem is how to order subtrees of u to minimize the sum of:
//
//           V * T
//         = # of remaining nodes * [2 * (edge length sum in v + l(u, v))]
//
//       When a tree is balanced, every subtree have the same number of nodes.
//       # of remaining nodes decrements by a fixed amount every time. Therefore
//       We just needs to visit smaller T first.
//
//       When the tree is not balanced, we prioritize smaller T / V. The hint
//       comes from the case when there is only two subtrees. T1V2 < T2V1 iff
//       T1 / V1 < T2 / V2. A strict proof has not been derived yet, but it 
//       turns out to work.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,                         // no vertex property
  boost::property<boost::edge_weight_t, int>  // interior edge weight property
> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

weighted_graph G;
weight_map weights;

// # of chambers, sum of edge length, lost of subtree
std::tuple<long, long, long> solve(vertex_desc u, vertex_desc parent) {
  long total_nodes = 0, edge_sum = 0;
  long lost = 0;
  
  vector<std::tuple<double, long, long>> order;  // time, node count
  order.reserve(boost::out_degree(u, G));
  
  out_edge_it oe, oe_end;
  for (boost::tie(oe, oe_end) = boost::out_edges(u, G); oe != oe_end; ++oe) {
    vertex_desc v = boost::target(*oe, G);
    if (v == parent) continue;
    
    auto res = solve(v, u);
    total_nodes += std::get<0>(res);
    edge_sum += std::get<1>(res) + weights[*oe];
    lost += std::get<2>(res) + std::get<0>(res) * weights[*oe];
    
    long T = (std::get<1>(res) + weights[*oe]) * 2;
    long V = std::get<0>(res);
    order.emplace_back((double) T / V, T, V);
  }
  
  std::sort(order.begin(), order.end(),
    [](const auto& e1, const auto& e2) -> bool {
      return std::get<0>(e1) < std::get<0>(e2);
    });  // increasing time
  
  long remaining_nodes = total_nodes;
  for (const auto &p : order) {
    remaining_nodes -= std::get<2>(p);
    lost += remaining_nodes * std::get<1>(p);
  }
  
  return {total_nodes + 1, edge_sum, lost};
}

void testcase() {
  int n;
  cin >> n;
  
  long sum = 0;
  for (int i = 0; i < n; i++) {
    long g;
    cin >> g;
    sum += g;
  }
  
  G = weighted_graph(n + 1);
  weights = boost::get(boost::edge_weight, G);
  
  for (int i = 0; i < n; i++) {
    int u, v, l;
    cin >> u >> v >> l;
    edge_desc e = boost::add_edge(u, v, G).first; 
    weights[e] = l;
  }
  
  cout << sum - std::get<2>(solve(0, 0)) << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
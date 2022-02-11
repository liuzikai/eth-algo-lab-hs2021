// Idea: min cost max flow.
// Graph: -- capacity(cost, 0 if unspecified) -->
//
//                                           v_target      v_source       v_target      v_source
//                                             ^             |.             ^             |
//                                             |             l*             |             l*
//                                             l*            |              l*            |
//                                             |             V              |             V
//     v_source --p*--> v_threshold --inf--> E_IN --inf--> E_OUT --1(r)#--> W_IN --inf--> W_OUT --inf(MAX_COST)$--> v_source
//                            |                                                                                         ^
//                            |                                (difficult)                                              |
//                            --------inf------------------> E_D --1(r)#--> W_D ---------inf(MAX_COST)$------------------
//
// * These edges should be asserted for full flow for feability
// $ These edges has MAX_COST to encourage flows go through l edges
// # Sum up cost on these edges for the result

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

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
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
    return e;
  }
};

#define E_IN(i)  ((i) * 3)
#define E_OUT(i) ((i) * 3 + 1)
#define E_D(i)   ((i) * 3 + 2)
#define W_IN(i)  (e * 3 + (i) * 3)
#define W_OUT(i) (e * 3 + (i) * 3 + 1)
#define W_D(i)   (e * 3 + (i) * 3 + 2)
#define MAX_FLOW 10000
#define MAX_COST (1 << 15)

void testcase() {
  int e, w, m, d, p, l;
  cin >> e >> w >> m >> d >> p >> l;
  
  graph G((e + w) * 3 + 3);
  const vertex_desc v_source = (e + w) * 3;
  const vertex_desc v_threshold = (e + w) * 3 + 1;
  const vertex_desc v_target = (e + w) * 3 + 2;
  edge_adder adder(G);  
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto w_map = boost::get(boost::edge_weight, G);
  
  vector<edge_desc> e_constraint;
  e_constraint.reserve(1 + e + w);
  
  e_constraint.emplace_back(adder.add_edge(v_source, v_threshold, p, 0));
  
  for (int i = 0; i < e; i++) {
    adder.add_edge(v_threshold, E_IN(i), MAX_FLOW, 0);
    adder.add_edge(v_threshold, E_D(i), MAX_FLOW, 0);
    e_constraint.emplace_back(adder.add_edge(E_IN(i), v_target, l, 0));
    adder.add_edge(E_IN(i), E_OUT(i), MAX_FLOW, 0);
    e_constraint.emplace_back(adder.add_edge(v_source, E_OUT(i), l, 0));
  }
  
  for (int i = 0; i < w; i++) {
    e_constraint.emplace_back(adder.add_edge(W_IN(i), v_target, l, 0));
    adder.add_edge(W_IN(i), W_OUT(i), MAX_FLOW, 0);
    e_constraint.emplace_back(adder.add_edge(v_source, W_OUT(i), l, 0));
    adder.add_edge(W_OUT(i), v_target, MAX_FLOW, MAX_COST);
    adder.add_edge(W_D(i), v_target, MAX_FLOW, MAX_COST);
  }
  
  vector<edge_desc> e_match;
  e_match.reserve(m);
  for (int i = 0; i < m; i++) {
    int u, v, r;
    cin >> u >> v >> r;
    e_match.emplace_back(adder.add_edge(E_OUT(u), W_IN(v), 1, r));
  }
  
  for (int i = 0; i < d; i++) {
    int u, v, r;
    cin >> u >> v >> r;
    e_match.emplace_back(adder.add_edge(E_D(u), W_D(v), 1, r));
  }
  
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  
  for(const auto &e : e_constraint) {
    if (rc_map[e] != 0) {
      cout << "No schedule!" << endl;
      return;
    }
  }

  int cost = 0;
  for(const auto &e : e_match) {
    if (rc_map[e] == 0) {
      cost += w_map[e];
    }
  }
  cout << cost << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
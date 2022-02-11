// Idea: min cost max flow + binary search for luggage count

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

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

int get_flow(const graph &G, vertex_desc v_source) {
  const auto c_map = boost::get(boost::edge_capacity, G);
  const auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source, G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  return s_flow;
}

void testcase() {
  int c, g, b, k, a;
  cin >> c >> g >> b >> k >> a;
  
  graph G(c);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  // auto r_map = boost::get(boost::edge_reverse, G);
  // auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  for (int i = 0; i < g; i++) {
    int x, y, d, e;
    cin >> x >> y >> d >> e;
    adder.add_edge(x, y, e, d);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, k, a);
  
  if (boost::find_flow_cost(G) <= b) {
    
    cout << get_flow(G, k) << endl;
    
  } else {
    
    const vertex_desc v_source = boost::add_vertex(G);
    const edge_desc e_source = adder.add_edge(v_source, k, 0, 0);
    
    int feasible_flow = 0;
    
    int l = 0, u = get_flow(G, k);
    while (l < u) {
      int m = (l + u + 1) / 2;
      
      c_map[e_source] = m;
      boost::successive_shortest_path_nonnegative_weights(G, v_source, a);
      
      if (boost::find_flow_cost(G) <= b) {
        l = m;
      } else {
        u = m - 1;
      }
    }
    
    c_map[e_source] = l;
    boost::successive_shortest_path_nonnegative_weights(G, v_source, a);
    feasible_flow = get_flow(G, v_source);
    
    // for (int i = 0; i < e_sum; i++) {
    //   adder.add_edge(v_source, k, 1, 0);
    //   boost::successive_shortest_path_nonnegative_weights(G, v_source, a);
    //   int cost = boost::find_flow_cost(G);
    //   if (cost <= b) {
    //     feasible_flow = get_flow(G, v_source);
    //   } else {
    //     break;
    //   }
    // }
    
    cout << feasible_flow << endl;
    
  }
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
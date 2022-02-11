// Idea: min cost max flow. Nodes are (station, time) pairs where there is
//       an order departs or arrives at (station, time) (discretization of
//       time). The key is to offset the negative -profit to a non-negative
//       value while ensuring every source-sink path is offset by the same 
//       amount. Since every path spans the same time (assuming the source is
//       at time 0 and the sink is at the possibly maximal time MAX_TIME), 
//       offseting each path by MAX_COST * the time it takes 
//       results in a total offset of MAX_COST * MAX_TIME for every s-t path.
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;

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

const int MAX_COST = 100;
const int MAX_FLOW = 1000;
const int MAX_TIME = 100000;

void testcase() {
  int N, S;
  cin >> N >> S;
  
  typedef std::pair<int, int> TimePoint;  // station, time
  map<TimePoint, vertex_desc> tpi;        // time point indices, ordered
  
  vector<int> initial;
  initial.reserve(S + 1);
  initial.emplace_back(0);
  for (int i = 1; i <= S; i++) {
    int l;
    cin >> l;
    initial.emplace_back(l);
    
    tpi[{i, 0}] = 0;  // assign index later
  }
  
  typedef std::tuple<TimePoint, TimePoint, int> Order; // from, to, price
  vector<Order> orders;
  orders.reserve(N);
  
  for (int i = 0; i < N; i++) {
    int s, t, d, a, p;
    cin >> s >> t >> d >> a >> p;
    TimePoint from{s, d}, to{t, a};
    orders.emplace_back(from, to, p);
    tpi[from] = tpi[to] = 0;  // assign index later
  }
  
  // Add nodes and edges for the timeline of each station
  graph G(2 + tpi.size());
  edge_adder adder(G);
  const vertex_desc v_source = 0;
  const vertex_desc v_target = 1;
  int idx = 2;
  for (auto it = tpi.begin(); it != tpi.end(); ++it) {
    const auto &tp = it->first;
    if (tp.second == 0) {  // time 0 for the station
      adder.add_edge(v_source, idx, initial[tp.first], 0);
    }
    
    auto next = it;
    ++next;
    if (next == tpi.end() || next->first.first != tp.first) {
      // Last time of the station to the virtual endpoint
      adder.add_edge(idx, v_target, MAX_FLOW, MAX_COST * (MAX_TIME - tp.second));  
    } else {
      // To the next time
      adder.add_edge(idx, idx + 1, MAX_FLOW, MAX_COST * (next->first.second - tp.second));   
    }
    
    // cout << "(" << tp.first << ", " << tp.second << ") = " << idx << endl;
    it->second = idx++;
  }
  
  // Add edges of orders
  for (const auto &order : orders) {
    const auto &from = std::get<0>(order);
    const auto &to = std::get<1>(order);
    adder.add_edge(tpi[from], tpi[to], 1, MAX_COST * (to.second - from.second) - std::get<2>(order));
  }

  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  
  int cost = boost::find_flow_cost(G);
  
  cout << s_flow * MAX_COST * MAX_TIME - cost << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
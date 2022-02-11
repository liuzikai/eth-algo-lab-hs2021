// Idea: maximum flow. Handle minimal flow by flowing to sink and from source.
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
using std::cin;
using std::cout;
using std::endl;

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
    c_map[rev_e] = 0;  // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  } 
};

void testcase() {
  
  int L, P;  cin >> L >> P;
  
  graph G(L);
  const vertex_desc source = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  
  edge_adder adder(G);
  
  std::vector<long> in, out;  
  in.reserve(L);  
  out.reserve(L);
  
  for (int i = 0; i < L; i++) {
    long g, d; 
    cin >> g >> d;
    in.emplace_back(g);  
    out.emplace_back(d);
  }
  
  for (int i = 0; i < P; i++) {
    long f, t, c, C; 
    cin >> f >> t >> c >> C;
    out[f] += c;  
    in[t] += c;
    adder.add_edge(f, t, C - c);
  }
  
  long sum = 0;
  
  for (int i = 0; i < L; i++) {
    adder.add_edge(source, i, in[i]);
    adder.add_edge(i, sink, out[i]);
    sum += out[i];
  }
  
  long flow = boost::push_relabel_max_flow(G, source, sink);
  
  if (flow == sum) {
    cout << "yes" << endl;
  } else {
    cout << "no" << endl;
  }
  
}

int main() {
  std::ios::sync_with_stdio(false);
  
  int T;  cin >> T;
  for (int i = 0; i < T; i++) {
    testcase();
  }
  
  return 0;
}
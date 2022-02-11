// Idea: max flow
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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

#define IN(x, y) (2 * ((y) * m + (x)))
#define OUT(x, y) (2 * ((y) * m + (x)) + 1)

const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};

void testcase() {
  int m, n, k, c;
  cin >> m >> n >> k >> c;
  
  graph G(2 * n * m);
  edge_adder adder(G);
  
  for (int x = 0; x < m; x++) {
    for (int y = 0; y < n; y++) {
      adder.add_edge(IN(x, y), OUT(x, y), c);
      for (int d = 0; d < 4; d++) {
        int tx = x + dx[d], ty = y + dy[d];
        if (tx >= 0 && tx < m && ty >= 0 && ty < n) {
          adder.add_edge(OUT(x, y), IN(tx, ty), 1);
        }
      }
    }
  }
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  for (int x = 0; x < m; x++) {
    adder.add_edge(OUT(x, 0), v_sink, 1);
    if (n > 1) adder.add_edge(OUT(x, n - 1), v_sink, 1);
  }
  for (int y = 0; y < n; y++) {
    adder.add_edge(OUT(0, y), v_sink, 1);
    if (m > 1) adder.add_edge(OUT(m - 1, y), v_sink, 1);
  }
  
  for (int d = 0; d < k; d++) {
    int x, y;
    cin >> x >> y;
    adder.add_edge(v_source, IN(x, y), 1);
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
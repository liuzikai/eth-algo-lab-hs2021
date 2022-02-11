// Idea: color the chessboard in two color (x + y is even/odd). Every knight can
//       only be in conflict of another knight on the other color. Therefore
//       the interference graph is bipartiate. Find the maximum independent set.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <queue>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;

typedef  boost::graph_traits<graph>::vertex_descriptor      vertex_desc;
typedef  boost::graph_traits<graph>::edge_descriptor        edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

#define IDX(i, j) ((i) * n + (j))

void testcase() {
  int n;
  cin >> n;
  
  vector<bool> board(n * n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int b;
      cin >> b;
      board[IDX(i, j)] = (b == 1);
    }
  }
  
  graph G(n * n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  const int di[8] = {-1, -1, +1, +1, -2, -2, +2, +2};
  const int dj[8] = {-2, +2, -2, +2, -1, +1, -1, +1};
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      
      if (!board[IDX(i, j)]) continue;
      
      int a = IDX(i, j);
      if ((i + j) % 2 == 0) {
        adder.add_edge(v_source, a, 1);
      } else {
        adder.add_edge(a, v_sink, 1);
      }
      
      for (int d = 0; d < 8; d++) {
        int ti = i + di[d], tj = j + dj[d];
        if (ti >= 0 && ti < n && tj >= 0 && tj < n && board[IDX(ti, tj)]) {
          if ((i + j) % 2 == 0) {
            adder.add_edge(a, IDX(ti, tj), 1);
          } else {
            adder.add_edge(IDX(ti, tj), a, 1);
          }
        }
      }
    }
  }
  
  boost::push_relabel_max_flow(G, v_source, v_sink);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  const int N = n * n;
  std::vector<int> vis(N, false);
  std::queue<int> Q;
  vis[v_source] = true;
  Q.push(v_source);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }

  int result = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[IDX(i, j)] && (vis[IDX(i, j)] ^ ((i + j) % 2 != 0))) result++;
    }
  }
  
  cout << result << endl;
}

int main()  {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
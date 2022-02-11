// Idea: articulation points. For every bridge u-v, introduce a virtual node
//       between u and v. If the node is an articulation point, the bridge
//       is a critical bridge.
// Note: the solution uses another idea, which is also a useful conclusion:
//       articulation edges (formally called bridges) are the one in 
//       biconnected components with single edge.
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
using std::cin;
using std::cout;
using std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

static void testcase() {
  size_t n, m;
  cin >> n >> m;
  
  std::vector<std::pair<int, int>> bridges;
  bridges.reserve(m);
  graph g(n + m);
  for (size_t i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    boost::add_edge(u, n + i, g);
    boost::add_edge(n + i, v, g);
    if (u > v) std::swap(u, v);
    bridges.emplace_back(u, v);
  }
  
  std::vector<vertex_desc> art_points;
  boost::articulation_points(g, std::back_inserter(art_points));
  
  std::vector<std::pair<int, int>> result;
  for (const auto &u : art_points) {
    if (u >= n) {
      result.emplace_back(bridges[u - n]);
    }
  }
  
  std::sort(result.begin(), result.end());
  
  cout << result.size() << endl;
  for (const auto &b : result) {
    cout << b.first << ' ' << b.second << endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
using std::cin;
using std::cout;
using std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

static void testcase() {
  int n, m;
  cin >> n >> m;
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    edge_desc e = boost::add_edge(u, v, G).first; 
    weights[e] = w;
  }
  
  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  int sum = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    sum += weights[*it];
  }
  
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  
  cout << sum << " " << *std::max_element(dist_map.begin(), dist_map.end()) << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
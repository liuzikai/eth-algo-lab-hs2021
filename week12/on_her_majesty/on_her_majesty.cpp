// Idea: shortest path + maximum matching + binary search. Find shortest paths 
//       for every pair of agent, shelter. Build agent - shelter bipartite
//       graph. Binary search on the alert time and add edges shorter than the
//       time. If the maximal matching = # of agents, the time works. To handle
//       c > 1, duplicate shelter nodes. The key idea is denoted below.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

// Custom edge adder class
class edge_adder {
 weighted_graph &G;
 weight_map weights;

 public:
  edge_adder(weighted_graph &G) : G(G), weights(boost::get(boost::edge_weight, G)) {}
  
  void add_edge(int from, int to, long cost) {
    edge_desc e = boost::add_edge(from, to, G).first; 
    weights[e] = cost;
  }
};

#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void testcase() {
  int n, m, a, s, c, d;
  cin >> n >> m >> a >> s >> c >> d;
  
  weighted_graph G(n);
  edge_adder adder(G);
  for (int i = 0; i < m; i++) {
    char w;
    int x, y, z;
    cin >> w >> x >> y >> z;
    adder.add_edge(x, y, z);
    if (w == 'L') adder.add_edge(y, x, z);
  }
  
  vector<int> agents;
  agents.reserve(a);
  for (int i = 0; i < a; i++) {
    int p;
    cin >> p;
    agents.emplace_back(p);
  }
  
  vector<int> shelters;
  shelters.reserve(s);
  for (int i = 0; i < s; i++) {
    int p;
    cin >> p;
    shelters.emplace_back(p);
  }
  
  // Compute the shortest routes for each agent
  typedef std::tuple<int, int, int> Route;  // agent, shelter, route time
  vector<Route> routes;
  routes.reserve(a * s);
  for (int i = 0; i < a; i++) {
    const int src = agents[i];
    std::vector<int> dist_map(boost::num_vertices(G));
  
    boost::dijkstra_shortest_paths(G, src,
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));
    
    for (int j = 0; j < s; j++) {
      const int dst = shelters[j];
      if (dist_map[dst] != std::numeric_limits<int>::max()) {
        
        // Re-index with i and j to handle multiple agents/shelters at the same location
        routes.emplace_back(i, j, dist_map[dst]);
      }
    }
  }
  
  std::sort(routes.begin(), routes.end(),
    [](const Route& e1, const Route& e2) -> bool {
      return std::get<2>(e1) < std::get<2>(e2);
  });
  
  int l = 0, u = std::get<2>(routes.back()) + d * c;
  while (l < u) {
    int m = (l + u) / 2;
    
    graph H(a + s * c);  // every shelter has c copy
    for (const auto &r : routes) {
      const int agent = std::get<0>(r), shelter = std::get<1>(r);
      for (int order = 0; order < c; order++) {  // the last but order enter the shelter
      
        // Here is the key: think reversely! For an agent to be the last one to
        // enter, arrival before m - d is needed. For last but one, m - 2 * d
        // is needed. It doesn't matter how the last agent arrive, as long as
        // she arrives before m - d. Even if she arrives early, she can simply
        // wait there and enjoy the avalanche ;)
        // Reference: official solution
        if (std::get<2>(r) + d * (order + 1) <= m) {
          boost::add_edge(agent, a + s * order + shelter, H);
        }
      }
    }
    
    // Maximum matching
    std::vector<vertex_desc> mate_map(boost::num_vertices(H));  // exterior property map
    boost::edmonds_maximum_cardinality_matching(H,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, H)));
    int matching_size = boost::matching_size(H,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, H)));
    
    if (matching_size == a) {
      u = m;
    } else {
      l = m + 1;
    }
  }
  
  cout << l << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
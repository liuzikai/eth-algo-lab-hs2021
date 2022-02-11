// Idea: maximum matching. Constrtuct a graph with edges between pairs of 
//       buddies with shared interest(s). To compute the intersection of
//       the interest(s), here I simply use sets of strings (log loopup).
//       Parallel walking through two sorted lists should also works.
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::unordered_set;
using std::string;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size;
}

static void testcase() {
  int n, c, f;
  cin >> n >> c >> f;
  
  graph g(n);
  
  vector<unordered_set<string>> chs;
  chs.reserve(n);
  for (int i = 0; i < n; i++) {
    
    unordered_set<string> ch;
    for (int j = 0; j < c; j++) {
      string s;
      cin >> s;
      ch.insert(std::move(s));
    }
    
    for (int j = 0; j < i; j++) {
      const auto &prev_ch = chs[j];
      int cnt = 0;
      for (const auto &c : ch) {
        if (prev_ch.find(c) != prev_ch.end()) cnt++;
      }
      if (cnt > f) boost::add_edge(i, j, g);
    }
    
    chs.emplace_back(std::move(ch));
  }
  
  // cout << maximum_matching(g) << endl;
  if (maximum_matching(g) == n / 2) {
    cout << "not optimal" << endl;
  } else {
    cout << "optimal" << endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
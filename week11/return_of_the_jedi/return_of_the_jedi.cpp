// Idea: (not sure if this is the expected solution...) find the minimal 
//       spanning tree on the original graph, which is Princess Leia’s solution.
//       Exclude every MST edge and re-run CUSTOMIZED Kruskal's algorithm. 
//       The minimal one is the solution.
// Note: Kruskal's algorithm required sorted edges. Using boost's version leads
//       to timeout. Customized version + sort edges only once works.
// Note: Princess Leia’s solution is the Prim's algoritm. It's not sure that
//       whether Kruskal's and Prim's generates the same solution, even if the
//       order of the planets are taken into consideration below.
#include <iostream>
#include <vector>
#include <set>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <boost/pending/disjoint_sets.hpp>

typedef std::size_t Index;
typedef std::tuple<Index, Index, int> Edge;

int n, s;
#define MAX_COST (1 << 30)

int kruskal(const vector<Edge> &edges, Index ignored = -1, 
            vector<Index> *selected = nullptr) {
  int sum = 0;
  boost::disjoint_sets_with_storage<> uf(n + 1);
  Index n_components = n;
  for (Index i = 0; i < edges.size(); i++) {
    if (i == ignored) continue;
    
    const auto &e = edges[i];
    Index c1 = uf.find_set(std::get<0>(e));
    Index c2 = uf.find_set(std::get<1>(e));
    
    if (c1 != c2) {
      uf.link(c1, c2);
      if (selected != nullptr) (*selected).emplace_back(i);
      sum += std::get<2>(e);
      if (--n_components == 1) break;
    }
  }
  return sum;
}

void testcase() {
  cin >> n >> s;
  
  vector<Edge> edges;
  edges.reserve(n * n / 2);  // roughly
  for (int j = 1; j <= n - 1; j++) {
    for (int k = 1; k <= n - j; k++) {
      int c;
      cin >> c;
      edges.emplace_back(j, j + k, c);
    }
  }
  
  std::sort(edges.begin(), edges.end(),
    [](const Edge& e1, const Edge& e2) -> bool {
      // Prioritize short edges
      if (std::get<2>(e1) != std::get<2>(e2)) {
        return std::get<2>(e1) < std::get<2>(e2);
      }
      // Prioritize important cities
      return std::min(std::get<0>(e1), std::get<1>(e1)) < 
             std::min(std::get<0>(e2), std::get<1>(e2));
    });
  
  vector<Index> selected; 
  kruskal(edges, -1, &selected);
  
  int result = MAX_COST;
  for (Index ignored : selected) {
    int cost = kruskal(edges, ignored);
    if (cost < result) result = cost;
  }
  
  cout << result << endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
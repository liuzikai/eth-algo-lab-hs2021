// Idea: growing disks + greedy. Build Delaunay triangulation of tents. Combine
//       them with incresing distance. Tents in the same components must be
//       assigned to the same family. Combine components to form as many >=k
//       groups as possible using the greedy strategy below.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

static Index combine_f(Index k, vector<Index> v /* copy */) {
  // Strategy: combine the max components with the min components
  // Works for k <= 4
  
  while (true) {
    Index u = k - 1, l = 1;  // Max/min non-empty sizes
    while (u > l && v[u] == 0) u--;
    while (u > l && v[l] == 0) l++;
    
    // Combine as many as possible
    // If u != l, one of them runs out. If u == l, half combined with half
    int delta = (u == l ? v[u] / 2 : std::min(v[u], v[l]));
    if (delta == 0) break;
    if (u + l < k) {
      // u + l will become the largest, which is going to be combined with l
      // Save a half of l to be combined with u + l
      if (delta > 1) delta /= 2;
    }
    
    v[std::min(k, u + l)] += delta;
    v[u] -= delta;
    v[l] -= delta;
  }
  return v[k];
}

void testcase() {
  
  Index n, k, f0;
  K::FT s0;
  cin >> n >> k >> f0 >> s0;
  
  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());

  EdgeV edges;
  edges.reserve(3 * n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second + 1) % 3)->info();
    Index i2 = e->first->vertex((e->second + 2) % 3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
    [](const Edge& e1, const Edge& e2) -> bool {
      return std::get<2>(e1) < std::get<2>(e2);
    });
    
  
  boost::disjoint_sets_with_storage<> uf(n);
  vector<Index> c_size(n, 1);  // component sizes
  
  vector<Index> k_counts(k + 1, 0);  // # of components of size 1, 2, ..., k - 1, >= k
  k_counts[1] = n;
  Index current_f = combine_f(k, k_counts);
  
  K::FT result_s = 0;
  Index result_f = (s0 == 0 ? current_f : 0);
  
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    const auto &dist = std::get<2>(*e);
    
    if (current_f >= f0) result_s = dist;
    if (dist >= s0 && current_f > result_f) result_f = current_f;
    
    if (c1 != c2) {
      
      k_counts[std::min(k, c_size[c1])]--;
      k_counts[std::min(k, c_size[c2])]--;
      uf.link(c1, c2);
      
      Index c = uf.find_set(std::get<0>(*e));
      c_size[c] = c_size[c1] + c_size[c2];
      k_counts[std::min(k, c_size[c])]++;
      
      current_f = combine_f(k, k_counts);
    }
  }
  
  if (/* infinite dist */ current_f > result_f) result_f = current_f;  
  
  cout << result_s << ' ' << result_f << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
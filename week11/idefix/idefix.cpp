// Idea: growing disks. Put trees into a Delaunay triangulation. Associate each
//       bones with the nearest tree. Sort tree-tree distances and 4 * bone-tree
//       distances from small to large. Grow disks by running two sorted vectors
//       in parallel.
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

void testcase() {
  Index n, m, k;
  K::FT s;
  std::cin >> n >> m >> s >> k;

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
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
            
  EdgeV dists;  // bone id, tree id, 4 * dist^2
  dists.reserve(m);
  for (Index i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    K::Point_2 bone(x, y);
    auto tree = t.nearest_vertex(bone);
    dists.emplace_back(i, tree->info(), CGAL::squared_distance(bone, tree->point()) * 4);
  }
  std::sort(dists.begin(), dists.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
  
  boost::disjoint_sets_with_storage<> uf(n);
  vector<Index> bones(n, 0);  // indexed by component
  
  EdgeV::const_iterator e = edges.begin();
  EdgeV::const_iterator d = dists.begin();

  Index result_a = 0;
  K::FT result_q = -1;
  while (e != edges.end() || d != dists.end()) {
    
    K::FT cur_4r2;
    Index b = 0;  // updated bone count
    if (e != edges.end() && (d == dists.end() || std::get<2>(*e) < std::get<2>(*d))) {
      cur_4r2 = std::get<2>(*e);
      
      // Merge trees
      Index c1 = uf.find_set(std::get<0>(*e));
      Index c2 = uf.find_set(std::get<1>(*e));
      if (c1 != c2) {
        b = bones[c1] + bones[c2];
        uf.link(c1, c2);
        bones[uf.find_set(std::get<0>(*e))] = b;
      }
      
      ++e;
    } else {
      cur_4r2 = std::get<2>(*d);
      
      // Merge bone and tree
      Index tree = uf.find_set(std::get<1>(*d));
      b = bones[tree] + 1;
      bones[tree] = b;
      
      ++d;
    }
    
    if (result_q == -1 && b >= k) result_q = cur_4r2;
    if (cur_4r2 <= s && b > result_a) result_a = b;
    
    if (result_q != -1 && cur_4r2 > s) break;
  }
  
  cout << result_a << ' ' << result_q << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
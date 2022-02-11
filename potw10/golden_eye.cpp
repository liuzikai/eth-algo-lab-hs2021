// Idea: growing disks
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;


typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;


void testcase() {
  Index n, m;
  K::FT p;
  std::cin >> n >> m >> p;

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


  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    K::FT squared_length = std::get<2>(*e);
    if (squared_length > p) break;
    
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  
  
  K::FT max_all_d = 0, max_p_d = 0;
  boost::disjoint_sets_with_storage<> all_uf(n);
  boost::disjoint_sets_with_storage<> p_uf(n);
  EdgeV::const_iterator all_it = edges.begin();
  EdgeV::const_iterator p_it = edges.begin();
  
  for (Index i = 0; i < m; ++i) {
    
    int x0, y0, x1, y1;
    std::cin >> x0 >> y0 >> x1 >> y1;
    
    auto s_p = K::Point_2(x0, y0);
    auto s_v = t.nearest_vertex(s_p);
    Index s_i = s_v->info();
    
    
    auto t_p = K::Point_2(x1, y1);
    auto t_v = t.nearest_vertex(t_p);
    Index t_i = t_v->info();
    
    K::FT d = std::max(CGAL::squared_distance(s_p, s_v->point()),
                       CGAL::squared_distance(t_p, t_v->point())) * K::FT(4);
    
    
    
    if (d <= p && uf.find_set(s_i) == uf.find_set(t_i)) {
          
      cout << 'y';
      if (d > max_p_d) max_p_d = d;
      
      while(p_it != edges.end() && p_uf.find_set(s_i) != p_uf.find_set(t_i)) {
        Index c1 = p_uf.find_set(std::get<0>(*p_it));
        Index c2 = p_uf.find_set(std::get<1>(*p_it));
        if (c1 != c2) {
          p_uf.link(c1, c2);
        }
        p_it++;
      }

      
    } else {
      cout << 'n';
    }
    
    if (d > max_all_d) max_all_d = d;
    while(all_it != edges.end() && all_uf.find_set(s_i) != all_uf.find_set(t_i)) {
      Index c1 = all_uf.find_set(std::get<0>(*all_it));
      Index c2 = all_uf.find_set(std::get<1>(*all_it));
      if (c1 != c2) {
        all_uf.link(c1, c2);
      }
      all_it++;
    }
  }

  cout << endl;
  
  if (all_it != edges.begin()) {
    max_all_d = std::max(max_all_d, std::get<2>(*(all_it - 1)));
  }
  if (p_it != edges.begin()) {
    max_p_d = std::max(max_p_d, std::get<2>(*(p_it - 1)));
  }
  cout << max_all_d << endl << max_p_d << endl;
  
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
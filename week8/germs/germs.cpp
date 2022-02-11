// Idea: growing disks (EMST.cpp). Handle the box boundary the same way.
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

#define square(x) ((x) * (x))

int main()  {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while (true) {
    Index n;
    std::cin >> n;
    if (n == 0) break;
  
    long L, B, R, T;
    cin >> L >> B >> R >> T;
    
    EdgeV edges;
    edges.reserve(4 * n);
    
    typedef std::pair<K::Point_2,Index> IPoint;
    std::vector<IPoint> points;
    points.reserve(n);
    for (Index i = 0; i < n; ++i) {
      int x, y;
      std::cin >> x >> y;
      points.emplace_back(K::Point_2(x, y), i);
      
      // Point n is the virtual node of "death"
      edges.emplace_back(i, n, 4 * square(std::min({x - L, R - x, y - B, T - y})));
    }
    
    Delaunay t;
    t.insert(points.begin(), points.end());
    
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
  
    vector<bool> dead(n + 1, false);
    dead[n] = true;
    Index alive_count = n;
    
    EdgeV::const_iterator e = edges.begin();
    for (Index t = 0; alive_count > 0; t++) {
      
      for (; e != edges.end() && std::get<2>(*e) <= square(K::FT(2) * t * t + 1); ++e) {
        Index a = std::get<0>(*e);
        Index b = std::get<1>(*e);
        Index delta = !dead[a] + !dead[b];
        
        if (delta) {
          if (alive_count == n) cout << t << " ";
          if (alive_count * 2 >= n && (alive_count - delta) * 2 < n) cout << t << " ";
          if (alive_count - delta == 0) cout << t << endl;
          
          alive_count -= delta;
          dead[a] = dead[b] = true;
          
        }
      }
    }
    
  }
  
  return 0;
}
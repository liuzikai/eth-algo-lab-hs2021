// Idea: find the nearest vertices using Delaunay Triangulation
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
  

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while (true) {
    Index n;
    std::cin >> n;
    if (n == 0) break;
    
    std::vector<K::Point_2> points;
    points.reserve(n);
    for (Index i = 0; i < n; ++i) {
      int x, y;
      std::cin >> x >> y;
      points.emplace_back(x, y);
    }
    
    Delaunay t;
    t.insert(points.begin(), points.end());
    
    Index m;
    std::cin >> m;
    for (Index i = 0; i < m; ++i) {
      int x, y;
      std::cin >> x >> y;
      K::Point_2 u(x, y);
      cout << CGAL::squared_distance(u, t.nearest_vertex(u)->point()) << endl;
    }
  }
  
  return 0;
}
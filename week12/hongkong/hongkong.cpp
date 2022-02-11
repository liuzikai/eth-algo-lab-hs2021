// Idea: motion planning
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>           Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                 Delaunay;

typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

#define square(x) ((x) * (x))

void testcase() {
  Index n, m;
  K::FT r;
  cin >> n >> m >> r;

  // Delaunay triangulation of trees
  std::vector<K::Point_2> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    points.emplace_back(x, y);
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  // Voronoi points
  vector<K::FT> clearance;
  clearance.reserve(t.number_of_faces());
  const K::FT MAX_CLEARANCE = square(K::FT(2E52));
  
  Index face_index = 1;  // 0 for infinite face
  clearance.emplace_back(MAX_CLEARANCE);
  
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    if (t.is_infinite(f)) {
      f->info() = 0;
    } else {
      f->info() = face_index++;
      K::Point_2 p = t.dual(f);
      clearance.emplace_back(CGAL::squared_distance(p, f->vertex(0)->point()));
    }
  }
  
  // Edges between trees
  EdgeV edges;  // face, face, squared length
  edges.reserve(3 * n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->info();
    Index i2 = e->first->neighbor(e->second)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
    [](const Edge& e1, const Edge& e2) -> bool {
      return std::get<2>(e1) > std::get<2>(e2);
    });
  
  
  vector<bool> result(m, false);
  
  // Input ballon
  typedef std::tuple<Index, Index, K::FT> Ballon;  // id, face, (2r + 2s)^2
  typedef vector<Ballon> BallonV;
  BallonV ballons;
  ballons.reserve(m);
  for (Index i = 0; i < m; ++i) {
    long x, y, s;
    std::cin >> x >> y >> s;
    
    K::Point_2 b(x, y);
    K::FT d = square(r + K::FT(s));  // (r + s)^2
    if (CGAL::squared_distance(b, t.nearest_vertex(b)->point()) < d) {
      // Cannot inflate, result default to false
    } else {
      d *= 4;  // (2r + 2s)^2
      auto f = t.locate(b);
      if (clearance[f->info()] >= d) {  // already clear
        result[i] = true;
      } else {
        ballons.emplace_back(i, f->info(), d);
      }
    }
  }
  
  // Sort ballons from large to small
  std::sort(ballons.begin(), ballons.end(),
    [](const Ballon& b1, const Ballon& b2) -> bool {
      return std::get<2>(b1) > std::get<2>(b2);
    });
  
  
  BallonV::const_iterator b = ballons.begin();
  boost::disjoint_sets_with_storage<> uf(face_index);
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    const K::FT &d = std::get<2>(*e);
    
    while (b != ballons.end() && std::get<2>(*b) > d) {  // b can no longer move
      result[std::get<0>(*b)] =  // the maximal clearance b can reach
        (clearance[uf.find_set(std::get<1>(*b))] >= std::get<2>(*b));
      ++b;
    }
    if (b == ballons.end()) break;
    
    Index f1 = std::get<0>(*e), f2 = std::get<1>(*e);
    Index c1 = uf.find_set(f1), c2 = uf.find_set(f2);
    if (c1 != c2) {
      uf.link(c1, c2);
      clearance[uf.find_set(f1)] = std::max(clearance[c1], clearance[c2]);
    }
  }
  
  // Handle remaining ballons
  while (b != ballons.end()) {
    result[std::get<0>(*b)] =  // the maximal clearance b can reach
      (clearance[uf.find_set(std::get<1>(*b))] >= std::get<2>(*b));
    ++b;
  }
  
  // Output
  for (bool r : result) {
    cout << (r ? 'y' : 'n');
  }
  cout << endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
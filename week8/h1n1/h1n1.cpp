// Idea: motion planning, same as the test exam.
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#define sqaure(x) ((x) * (x))

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>           Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                 Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;


void testcase(Index n) {
  
  std::vector<K::Point_2> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(x, y);
  }

  Delaunay t;
  t.insert(points.begin(), points.end());

  Index face_count = 1;  // face 0 for infinite
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    f->info() = face_count++;
  }

  EdgeV edges;
  edges.reserve(3 * n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    const auto f1 = e->first;
    const auto f2 = f1->neighbor(e->second);
    Index i1 = t.is_infinite(f1) ? 0 : f1->info();
    Index i2 = t.is_infinite(f2) ? 0 : f2->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  
  // From large edge length to small
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) > std::get<2>(e2);
            });

  typedef std::tuple<K::FT, Index, Index> Query;  
  // 4d^2, query index, cloest vertex
  typedef vector<Query> QueryV;
  
  Index m;
  cin >> m;
  
  vector<bool> result(m);
  QueryV queries;
  
  for (Index i = 0; i < m; ++i) {
    int x, y;
    long d;
    std::cin >> x >> y >> d;
    
    K::Point_2 p(x, y);
    if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < K::FT(d)) {
      result[i] = false;
    } else {
      auto f = t.locate(p);
      if (t.is_infinite(f)) {
        result[i] = true;
      } else {
        queries.emplace_back(K::FT(4) * K::FT(d), i, f->info());
      }
    }
  }
  
  // From large d to small
  std::sort(queries.begin(), queries.end(),
      [](const Query& q1, const Query& q2) -> bool {
        return std::get<0>(q1) > std::get<0>(q2);
          });

  boost::disjoint_sets_with_storage<> uf(face_count);
  auto q = queries.cbegin();
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    
    while (q != queries.cend() && std::get<0>(*q) > std::get<2>(*e)) {
      result[std::get<1>(*q)] = uf.find_set(std::get<2>(*q)) == uf.find_set(0U);
      ++q;
    }
    if (q == queries.cend()) break;
    
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
    }
  }
  
  while (q != queries.cend()) {
    result[std::get<1>(*q)] = uf.find_set(std::get<2>(*q)) == uf.find_set(0U);
    ++q;
  }
  
  for (bool b : result) {
    cout << (b ? 'y' : 'n');
  }
  cout << endl;
}

int main() 
{
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t n;
  for (std::cin >> n; n != 0; std::cin >> n) testcase(n);
  return 0;
}
// Idea: motion planning. Construct Delaunay triangulation of Dementors. Move
//       along the Voronoi triangulation.
#include <iostream>
#include <vector>
#include <map>
using std::cout; 
using std::cin; 
using std::endl;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

// We want to store an index with each face
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

static const K::FT MAX_D2 = K::FT(2E54) * K::FT(2E54);

void testcase() {
  
  Index n, m;
  K::FT d;
  std::cin >> n >> m >> d;
  
  // Read Dementors points
  std::vector<K::Point_2> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    double x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y));
  }
  
  // Construct Delaunay triangulation
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  // Assign each face with an index and calculate distance from Voronoi vertex
  Index face_count = 0;
  std::vector<K::FT> face_d2;
  face_d2.reserve(t.number_of_faces());
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    f->info() = face_count;
    
    if (t.is_infinite(f)) {
      face_d2.emplace_back(MAX_D2);
    } else {
      auto p = t.dual(f);
      face_d2.emplace_back(CGAL::squared_distance(p, f->vertex(0)->point()));
    }
    
    face_count++;
  }
  
  // Initialize result vectors
  std::vector<int> f_res(m, 0);  // -: n, 0: unknown, +: y
  
  // Read fugitives points
  typedef std::tuple<K::Point_2, K::FT, Delaunay::Face_handle, Index> FPoint;
  std::vector<FPoint> f_points;
  f_points.reserve(m);
  for (Index i = 0; i < m; ++i) {
    double x, y;
    K::FT s;
    std::cin >> x >> y >> s;
    K::Point_2 f_point(x, y);
    
    K::FT val = (d + s) * (d + s);
    if (CGAL::compare_squared_distance(
          f_point, t.nearest_vertex(f_point)->point(), val) == CGAL::SMALLER) {
      f_res[i] = -1;  // initial position is too close to Dementor
    } else {
      Delaunay::Face_handle f = t.locate(f_point);
      val *= K::FT(4);
      if (face_d2[f->info()] >= val) {
        f_res[i] = 2;  // just need to stay in the current face
      } else {
        f_points.emplace_back(f_point, val, f, i);
      }
    }
  }
  
  // Sort f_points by decreasing 4 * (d + s)^2
  std::sort(f_points.begin(), f_points.end(),
      [](const FPoint& e1, const FPoint& e2) -> bool {
        return std::get<1>(e1) > std::get<1>(e2);
            });
  
  // Extract edges and sort by decreasing squared length
  EdgeV edges;
  edges.reserve(3 * n + 1); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index f1 = e->first->info();
    Index f2 = e->first->neighbor(e->second)->info();
    edges.emplace_back(f1, f2, t.segment(e).squared_length());
  }
  edges.emplace_back(0, 0, 0);
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) > std::get<2>(e2);
            });
  
  
  // Setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(face_count);
  Index f_index = 0;
  
  // Process edges in order of decreasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    
    // For all fugitives that can only moves in channel wider than this 
    // (squared) value, its union-find set shows the reachable faces
    K::FT channel_width2 = std::get<2>(*e);
    
    while (f_index < f_points.size()) {
      const auto &f = f_points[f_index];
      if (std::get<1>(f) <= channel_width2) break;
      K::FT max_d2 = face_d2[uf.find_set(std::get<2>(f)->info())];
      f_res[std::get<3>(f)] = (max_d2 >= std::get<1>(f) ? 1 : -1);
      f_index++;
    }
    
    
    if (c1 != c2) { // this edge connects two different components
      K::FT d1 = face_d2[c1], d2 = face_d2[c2];
      uf.link(c1, c2);
      
      // All faces in this components can be used to stay with max{d1, d2}
      face_d2[c1] = face_d2[c2] = (d1 > d2 ? d1 : d2);
    }
  }
  
  // Print results
  for (Index i = 0; i < m; i++) {
    if (f_res[i] < 0) {
      std::cout << 'n';
    } else if (f_res[i] > 0) {
      std::cout << 'y';
    } else {
      std::cout << "?" << i << endl;
      exit(1);
    }
    
  }
  
  std::cout << std::endl;
}

int main() {
  std::ios::sync_with_stdio(false);
  int T;  
  std::cin >> T;
  for (int i = 0; i < T; i++) {
    testcase();
  }
  return 0;
}
// Idea: Build an interference graph where every pair of interfering stations has an edge.
//       Edges are added by iterating incident vertices of each vertex and compare distance.
//       If the graph can be bipartiated, a network without interference exists.
//       Stations in each connected components can communicate with each other.
// Note: Growing disks (EMST) does not work! Stations can be collinear. If A - B - C
//       are collinear, there is no finite face ABC. Many stations can be collinear,
//       so the search should be iterative. One thing to notice is that the search
//       can stop if the point is r^2 away from the current station (prove?)
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
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

typedef std::tuple<Index,Index> Edge;
typedef std::vector<Edge> EdgeV;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

static void testcase() {
  Index n, m, r;
  std::cin >> n >> m >> r;
  
  K::FT r2 = r * r;

  // Interference graph
  graph G(n);

  // Input station locations
  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    
    K::Point_2 u(x, y);
    
    // for (const auto &elem: points) {
    //   if (CGAL::compare_squared_distance(u, elem.first, r2) != CGAL::LARGER) {
    //     boost::add_edge(i, elem.second, G);
    //   }
    // }
    
    points.emplace_back(u, i);
  }
  
  // Construct Delaunay triangulation
  Delaunay t;
  t.insert(points.begin(), points.end());

  boost::disjoint_sets_with_storage<> uf(n);
  
  for (auto u = t.finite_vertices_begin(); u != t.finite_vertices_end(); ++u) {
    Index i1 = u->info();
    
    std::set<Index> visited;
    std::queue<Tds::Vertex_handle> q;
    
    q.push(u);
    visited.insert(u->info());
    while (!q.empty()) {
      auto s = q.front();
      q.pop();
      
      Delaunay::Vertex_circulator v = t.incident_vertices(s);
      do {
        if (t.is_infinite(v) || s == v) continue;
        
        Index i2 = v->info();
        
        if (CGAL::compare_squared_distance(u->point(), v->point(), r2) != CGAL::LARGER && 
            visited.find(v->info()) == visited.end()) {
              
          Index c1 = uf.find_set(i1);
          Index c2 = uf.find_set(i2);
          if (c1 != c2) {
            uf.link(c1, c2);
          }
          
          boost::add_edge(i1, i2, G);
          
          q.push(v);
          visited.insert(i2);
        }
      } while (++v != t.incident_vertices(s));
    }
  }
  
  // Check bipartition
  bool bipartite = boost::is_bipartite(G);
  
  // Input clues
  for (Index i = 0; i < m; ++i) {
    int ax, ay, bx, by;
    std::cin >> ax >> ay >> bx >> by;
    K::Point_2 a(ax, ay), b(bx, by);
    
    if (!bipartite) {
      cout << 'n';
      continue;
    }
    
    if (CGAL::compare_squared_distance(a, b, r2) != CGAL::LARGER) {
      cout << 'y';
      continue;
    }
    
    auto va = t.nearest_vertex(a);
    if (CGAL::compare_squared_distance(a, va->point(), r2) == CGAL::LARGER) {
      cout << 'n';
      continue;
    }
    
    auto vb = t.nearest_vertex(b);
    if (CGAL::compare_squared_distance(b, vb->point(), r2) == CGAL::LARGER) {
      cout << 'n';
      continue;
    }
    
    cout << (uf.find_set(va->info()) == uf.find_set(vb->info()) ? 'y' : 'n');
  }
  
  
  cout << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}

// Idea: find nearest vertex + binary search for the last lamp before everyone is out.
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

#define square(x) ((x) * (x))

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef std::pair<K::Point_2, long> Player;

K::FT h;

bool has_winners(const std::vector<K::Point_2> &points, 
                 const std::vector<Player> &players, Index until_light) {
  Delaunay t;
  t.insert(points.begin(), points.begin() + until_light);
  for (Index i = 0; i < players.size(); ++i) {
    const auto &p = players[i].first;
    if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) >= square(h + players[i].second)) {
      return true;
    }
  }
  return false;
}

void print_winners(const std::vector<K::Point_2> &points, 
                   const std::vector<Player> &players, Index until_light) {
  Delaunay t;
  t.insert(points.begin(), points.begin() + until_light);
  for (Index i = 0; i < players.size(); ++i) {
    const auto &p = players[i].first;
    if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) >= square(h + players[i].second)) {
      cout << i << ' ';
    }
  }
  cout << endl;
}

void testcase() {
  Index m, n;
  std::cin >> m >> n;
  
  std::vector<Player> players;
  players.reserve(m);
  for (Index i = 0; i < m; ++i) {
    int x, y;
    long r;
    std::cin >> x >> y >> r;
    players.emplace_back(K::Point_2(x, y), r);
  }
  
  cin >> h;

  std::vector<K::Point_2> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(x, y);
  }
  
  if (has_winners(points, players, n)) {
    
    print_winners(points, players, n);
    
  } else {
    
    Index l = 0, u = n - 1;
    while (l < u) {
      Index m = (l + u + 1) / 2;
      if (has_winners(points, players, m)) {
        l = m;
      } else {
        u = m - 1;
      }
    }
    
    if (l == 0) {
      for (Index i = 0; i < players.size(); ++i) cout << i << ' ';
      cout << endl;
    } else {
      print_winners(points, players, l);
    }
  }
}

int main() 
{
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
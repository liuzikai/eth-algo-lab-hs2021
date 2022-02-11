#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
using std::cin;
using std::cout;
using std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

int main() {
  
  std::ios::sync_with_stdio(false);
  
  while (true) {
    int n;
    cin >> n;
    if (n == 0) break;
    
    long x, y, a, b;
    cin >> x >> y >> a >> b;
    
    R ray(P(x, y), P(a, b));
    
    bool intersect = false;
    for (int i = 0; i < n; i++) {
      long r, s, t, u;
      cin >> r >> s >> t >> u;
      
      if (!intersect) {
        if (CGAL::do_intersect(ray, S(P(r, s), P(t, u)))) {
          intersect = true;
        }
      }
    }
    
    cout << (intersect ? "yes" : "no") << endl;
  }
  
  return 0;
}
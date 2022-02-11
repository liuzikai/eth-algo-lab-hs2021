#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  K::Point_2                      P;

int main() {
  
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  
  while (true) {
    int n;
    cin >> n;
    if (n == 0) break;
    
    std::vector<K::Point_2> P;
    P.reserve(n);
    for (int i = 0; i < n; ++i) {
      long x, y;
      cin >> x >> y;
      P.emplace_back(x, y);
    }
  
    Min_circle mc(P.begin(), P.end(), true);
    Traits::Circle c = mc.circle();
    
    double r = std::floor(std::sqrt(CGAL::to_double(c.squared_radius())));
    while (r > 0 && K::FT(r - 1) * K::FT(r - 1) >= c.squared_radius()) r -= 1;
    while (K::FT(r) * K::FT(r) < c.squared_radius()) r += 1;
    std::cout << r << endl;
  }
  
  return 0;
  
}
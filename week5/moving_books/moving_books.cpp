// Idea: greedily takes as many book up to the ceil(remaining books/remaining persion) books
//       as the person can.
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;

static inline int div_ceil(int a, int b) {
  int c = a / b;
  if (c * b == a) return c;
  return c + 1;
}

int s[300000], w[300000];

static void testcase() {
  int n, m;
  cin >> n >> m;
  for (int i = 0; i < n; i++) cin >> s[i];
  for (int i = 0; i < m; i++) cin >> w[i];
  
  std::sort(s, s + n);
  std::sort(w, w + m);
  
  if (s[n - 1] < w[m - 1]) {
    cout << "impossible" << endl;
    return;
  }
  
  int i = 0, l = 0, u = 0;   // person i takes u - l book(s) [l, u)
  int bpp = std::max(1, div_ceil(m, n));  // book per person
  while(i < n - 1) {
    
    // Take as many books up to bpp as the person can
    while (u < m && s[i] >= w[u] && u - l < bpp) u++;
    
    // Switch to next person
    l = u;
    i++;
    bpp = std::max(bpp, div_ceil(m - u, n - i));
    // cout << "bpp = " << bpp << endl;
  }
  
  cout << bpp * 3 - 1 << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
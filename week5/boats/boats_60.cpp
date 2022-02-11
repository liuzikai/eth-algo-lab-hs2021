#include <iostream>
#include <algorithm>
#include <vector>
#define MAX_N 200000
using std::cin;
using std::cout;
using std::endl;

int v[MAX_N], l[MAX_N], p[MAX_N];
int offset;

static bool compare(int a, int b) {
  int x = std::max(offset + l[a], p[a]);
  int y = std::max(offset + l[b], p[b]);
  if (x != y) return x < y;
  return a < b;
}

static void testcase() {
  
  int n;
  cin >> n;
  
  for (int i = 0; i < n; i++) {
    v[i] = i;
    cin >> l[i] >> p[i];
  }
  
  offset = -10000000;
  
  int result = 0;
  int i = 0;
  while (i < n) {
    std::sort(v + i, v + n, compare);
    
    result++;
    offset = std::max(offset + l[v[i]], p[v[i]]);
    i++;
    
    while (i < n && p[v[i]] < offset) i++;
  }
  
  cout << result << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
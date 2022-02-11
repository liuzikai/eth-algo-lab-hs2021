#include <iostream>
#include <algorithm>
#include <vector>
#define MAX_N 200000
using std::cin;
using std::cout;
using std::endl;

int s[MAX_N], l[MAX_N], p[MAX_N];
int offset;

/**
 * 1. Boat with smallest p should be place leftmost ending at p. This boat can
 *    always be selected. Next boat at least starts at p (offset).
 * 2. p is the left point when the boat shifts to rightmost. If p < offset, 
 *    the boat cannot be placed.
 * 3. After discarding the invalid boats, the first boat is a candidate. If
 *    it is selected, the new offset will be std::max(offset + l[v], p[v]).
 * 4. Try to find another boat that leads to smaller end point (greedy).
 *    A smaller new offset can at least lead to equally good solution later.
 *    !! Stop when p >= new_offset !! Because the boat is compatible with the
 *    current solution AND it (and the following boats) can never lead to a
 *    better new_offset.
 */ 

static void testcase() {
  
  int n;
  cin >> n;
  
  for (int i = 0; i < n; i++) {
    s[i] = i;
    cin >> l[i] >> p[i];
  }
  
  std::sort(s, s + n, [](int a, int b) { 
    return (p[a] != p[b] ? p[a] < p[b] : a < b); 
  });
  
  int result = 1;
  int offset = p[s[0]];
  int i = 1;
  while (i < n) {
    
    // Discard boats that cannot start after offset
    while (i < n && p[s[i]] < offset) i++;
    if (i == n) break;
    
    // This boat is a candidate
    int v = s[i];
    int new_offset = std::max(offset + l[v], p[v]);
    i++;
    
    // Try to find a boat with smaller end point
    while (i < n) {
      
      v = s[i];
      
      if (p[v] >= new_offset) {
        // This boat can compatible with the current solution
        // And continue searching can only lead to end point > new_offset
        break;
      }
      
      new_offset = std::min(new_offset, std::max(offset + l[v], p[v]));
      i++;
    }
    
    result++;
    offset = new_offset;
    
  }
  
  cout << result << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
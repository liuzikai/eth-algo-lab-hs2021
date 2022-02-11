// Idea: sliding window. To find a window across two water ways, maintain a map
//       from cost to count of islands that start at the second island in
//       previous waterways. If current window includes island 0, the window
//       can be combined with another window whose total cost is k - cost.
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;

void testcase() {
  int n, w;
  long k;
  cin >> n >> k >> w;
  
  vector<int> c;
  c.reserve(n);
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    c.emplace_back(x);
  }
  
  map<long, int> prefix;  // sum of cost -> count of islands
  
  int result = 0;
  for (int i = 0; i < w; i++) {
    int len;
    cin >> len;
    
    vector<int> id;
    id.reserve(len);
    for (int j = 0; j < len; j++) {
      int x;
      cin >> x;
      id.emplace_back(x);
    }
    
    int l = 0, u = 0;  // select idlands [l, u)
    long sum = 0;  
    while (l < len) {
      // cout << "l = " << l << ", u = " << u << ", sum = " << sum << endl;
      if (sum == k && u - l > result) {
        result = u - l;
      }
      if (l == 0 && u > 0) {
        const auto it = prefix.find(k - sum);
        if (it != prefix.end() && u - l + it->second > result) {
          result = u - l + it->second;
        }
      }
      if (u < len && (l == u || sum < k)) {
        sum += c[id[u]];
        u++;
      }
      else {
        sum -= c[id[l]];
        l++;
      }
    }
    
    sum = 0;
    for (int j = 1; j < len; j++) {
      sum += c[id[j]];
      if (sum > k) break;
      
      auto it = prefix.find(sum);
      if (it == prefix.end() || it->second < j) {
        prefix[sum] = j;
      }
    }
    
  }
  
  cout << result << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
// Idea: prefix sum + binary search. We want to find a continuous region i to
//       j where the sum is as close to k as possible. Here we compute the
//       prefix sum, where sum[x] is the sum from 0 to x. So the sum from i
//       to j (exclusive) is sum[j] - sum[i]. For every sum[i], binary search
//       for a sum[j] that is closest to k + sum[i] (sum[j] - sum[i] = k =>
//       sum[j] = k + sum[i]). O(n log n).
// Note: A better approach is sliding window. Maintain two pointers (indices)
//       i and j. When the sum is less than k, forward j. When the sum is 
//       greater than k, forward i. O(2n) = O(n).
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

vector<long long> sum;

int inline binary_search(long long v, int l) {
  
  int h = sum.size() - 1;
  while (h - l > 1) {
    
    int m = (h + l) / 2;
    long long u = sum[m];
    
    if (u < v) {
      l = m;
    } else {
      h = m;
    }
  }
  
  if (abs(sum[h] - v) < abs(sum[l] - v)) {
    return h;
  } else {
    return l;
  }
}

void inline work_on_test_case() {
  int n, k;
  cin >> n >> k;
  
  sum.clear();
  if (n) sum.reserve(n);
  
  int v;
  if (n) {
    cin >> v;
    sum.emplace_back(v);
  }
  
  for (int i = 1; i < n; i++) {
    cin >> v;
    sum.emplace_back(sum.back() + v);
  }
  
  // Initialize to i = 0;
  int result_i = 0;
  int result_j = binary_search(k, 0);
  long long min_diff = abs(sum[result_j] - k);
  
  
  for (int i = 1; i < n; i++) {
    
    int j = binary_search(k + sum[i - 1], i);
    if (abs(sum[j] - sum[i - 1] - k) < min_diff) {
      result_i = i;
      result_j = j;
      min_diff = abs(sum[j] - sum[i - 1] - k);
    }
    
  }
  
  cout << result_i << " " << result_j << endl;
  
}

int main() {
  ios::sync_with_stdio(false);
  
  int t;
  cin >> t;
  for (int g = 0; g < t; g++) {
    work_on_test_case();
  }
  
  return 0;
}
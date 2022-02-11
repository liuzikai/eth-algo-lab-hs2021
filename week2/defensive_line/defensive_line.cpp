// Idea: sliding window to find subsequences with sum k. DP to maximize
//       the value.
#include<iostream>
#include<cstring>
using namespace std;

#define MAX_N (100000 + 10)
#define MAX_M (100 + 10)

int n, m, k;
int v[MAX_N];
int w[MAX_N];
int d[MAX_N][MAX_M];

void work_on_test_case() {
  
  cin >> n >> m >> k;
  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }
  
  // Siding window
  {
    memset(w, 0, sizeof(w));
    int i = 0, j = 0, sum = v[0];
    while (true) {
      // cout << i << " " << j << endl;
      if (sum == k) {
        w[i] = (j - i + 1);
        j++;
        if (j >= n) break;
        sum += v[j];
      } else if (sum < k || i == j) {
        j++;
        if (j >= n) break;
        sum += v[j];
      } else {
        sum -= v[i];
        i++;
      }
    }
  }
  
  // DP
  {
    for (int i = 0; i <= n; i++) {
      d[i][0] = 0;
      for (int j = 1; j <= m; j++) {
        d[i][j] = -MAX_N;
      }
    }
    
    for (int j = 1; j <= m; j++) {
      for (int i = n - 1; i >= 0; i--) {
        if (w[i] != 0) {
          d[i][j] = max(d[i + 1][j], d[i + w[i]][j - 1] + w[i]);
        } else {
          d[i][j] = d[i + 1][j];
        }
      }
    }
  }
  
  if (d[0][m] < 0) {
    cout << "fail" << endl;
  } else {
    cout << d[0][m] << endl;
  }
}

int main() {
  ios::sync_with_stdio(false);
  
  int t; cin >> t;
  for (int g = 0; g < t; g++) {
    work_on_test_case();
  }
  
  return 0;
}
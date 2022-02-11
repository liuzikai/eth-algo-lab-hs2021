// Idea: dynamic programming. When passenger p_k takes turn, maximize the
//       value. Otherwise, minimize it.
#include <iostream>
#include <vector>
#include <cstring>
#define MAX_N (1000 + 10)
#define MAX_RESULT (1000 * 1024 + 10)
using namespace std;

int n, m, k;
int s[MAX_N];
int w[MAX_N][MAX_N];

void inline work_on_test_case() {
  
  cin >> n >> m >> k;
  s[0] = s[n + 1] = -MAX_RESULT;
  for (int i = 1; i <= n; i++) {
    cin >> s[i];
  }
  
  for (int i = 0; i <= n + 1; i++) {
    for (int j = 0; j <= n + 1; j++) {
      w[i][j] = -MAX_RESULT;
    }
  }
  
  for (int i = 1; i <= n + 1; i++) {
    w[i][i - 1] = 0;
  }
  
  for (int r = n; r >= 1; r--) {
    for (int i = 1; i <= r; i++) {
      int j = n - r + i;
      if ((r - 1) % m == k) {
        int a = s[i] + w[i + 1][j], b = s[j] + w[i][j - 1];
        if (a >= 0 && b < 0) w[i][j] = a;
        else if (a < 0 && b >= 0) w[i][j] = b;
        else w[i][j] = max(a, b);
      } else {
        int a = w[i + 1][j], b = w[i][j - 1];
        if (a >= 0 && b < 0) w[i][j] = a;
        else if (a < 0 && b >= 0) w[i][j] = b;
        else w[i][j] = min(a, b);
      }
    }
  }
  
  cout << w[1][n] << endl;
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
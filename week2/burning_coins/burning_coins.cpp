// Idea: DP. At your turn, take the maximum. At the other's turn, take the
//       minimum.
#include<iostream>
using namespace std;

#define MAX_N 2510

int n;
int v[MAX_N];
int w[MAX_N][MAX_N];

void work_on_test_case() {
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> v[i];
  }
  
  for (int i = 0; i < n; i++) {
    w[i][i] = (n % 2 == 0) ? 0 : v[i];
  }
  
  for (int l = 1; l < n; l++) {
    for (int i = 0; i < n - l; i++) {
      int j = i + l;
      if ((i + (n - 1) - j) % 2 == 0) {
        w[i][j] = max(v[i] + w[i + 1][j], v[j] + w[i][j - 1]);
      } else {
        w[i][j] = min(w[i + 1][j], w[i][j - 1]);
      }
    }
  }
  
  cout << w[0][n - 1] << endl;
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
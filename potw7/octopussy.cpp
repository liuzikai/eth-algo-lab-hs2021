// Idea: greedily tear down the bomb and its subtree with minimal explosion time.
#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#define MAX_N 65536
#define parent(n) (n / 2)
#define left_child(n) (n * 2 + 1)
#define right_child(n) (n * 2 + 2)
using namespace std;

int N;
int T[MAX_N];
bool deactivated[MAX_N];

struct T_more {
  bool operator() (int a, int b) const {
    if (T[a] != T[b]) return (T[a] > T[b]);
    return a > b;
  }
};

int tear_down(int u) {
  if (u >= N || deactivated[u]) return 0;
  deactivated[u] = true;
  return tear_down(left_child(u)) + tear_down(right_child(u)) + 1;
}


void testcase() {

  cin >> N;
  for (int i = 0; i < N; i++) cin >> T[i];
  memset(deactivated, 0, sizeof(deactivated));

  // Use priority_queue here due to some early implementation. Vector should also work.
  priority_queue<int, vector<int>, T_more> Q;
  for (int i = 0; i < N; i++) {
    Q.push(i);
  }

  int t = 0;
  while (!Q.empty()) {
    int u = Q.top();  Q.pop();
    // cout << u << "(" << T[u] << ") ";
    if (!deactivated[u]) {
      t += tear_down(u);
      if (t > T[u]) {
        cout << "no" << endl;
        return;
      }
      deactivated[u] = true;
    }
  }
  
  cout << "yes" << endl;
}

int main() {
  ios::sync_with_stdio(false);
  
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
  
  return 0;
}
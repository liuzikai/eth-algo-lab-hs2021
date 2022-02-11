// Idea: binary search on DFS path. When DFS at node u, a continuous path from 
//       the root to u can be constructed, on which the binary search can be
//       performed.
// Note: Didn't figure it out myself. Inspired by the official solution.
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;
using std::unordered_map;

vector<int> age;
vector<vector<int>> children;
vector<vector<pair<int, int>>> queries;  // query id, b
vector<int> result;

static int binary_search(int b, const vector<int>& path) {
  int l = 0, u = path.size() - 1;
  while (l < u) {
    int m = (l + u) / 2;
    if (age[path[m]] <= b) {
      u = m;
    } else {
      l = m + 1;
    }
  }
  return path[l];
}

static void dfs(int u, vector<int> &path) {
  path.emplace_back(u);
  for (const auto &elem : queries[u]) {
    result[elem.first] = binary_search(elem.second, path);
  }
  for (int v : children[u]) {
    dfs(v, path);
  }
  path.pop_back();
}

static void testcase() {
  int n, q;
  cin >> n >> q;
  
  age.clear();
  age.reserve(n);
  
  vector<string> name;
  unordered_map<string, int> index;
  name.reserve(n);
  for (int i = 0; i < n; i++) {
    string s;
    int a;
    cin >> s >> a;
    age.emplace_back(a);
    index[s] = i;
    name.emplace_back(std::move(s));
  }
  
  children = vector<vector<int>>(n, vector<int>());
  for (int i = 0; i < n - 1; i++) {
    string s, p;
    cin >> s >> p;
    children[index[p]].emplace_back(index[s]);
  }
  
  queries = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>());
  for (int i = 0; i < q; i++) {
    string s;
    int b;
    cin >> s >> b;
    queries[index[s]].emplace_back(i, b);
  }
  
  result = vector<int>(q);
  const int root = std::max_element(age.begin(), age.end()) - age.begin();
  vector<int> path;
  dfs(root, path);
  
  
  for (int i = 0; i < q; i++) {
    cout << name[result[i]] << ' ';
  }
  cout << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) testcase();
  return 0;
}
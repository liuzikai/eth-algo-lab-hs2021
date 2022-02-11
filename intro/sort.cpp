#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <set>
using namespace std;

void work_on_test_case() {
    int n;
    cin >> n;

    vector<int> v;
    if (n) v.reserve(n);

    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        v.emplace_back(a);
    }

    int x;
    cin >> x;

    if (x) {
        std::sort(v.begin(), v.end(), std::greater<int>());
    } else {
        std::sort(v.begin(), v.end(), std::less<int>());
    }

    for (const auto &a : v) {
        cout << a << " ";
    }
    cout << endl;
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
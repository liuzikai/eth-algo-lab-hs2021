#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    int t;
    cin >> t;
    for (int g = 0; g < t; g++) {

        int n;
        cin >> n;

        vector<int> v;
        v.reserve(n);

        for (int i = 0; i < n; i++) {
            int a;
            cin >> a;
            v.emplace_back(a);
        }

        int d; 
        cin >> d;
        v.erase(v.begin() + d);

        int a, b;
        cin >> a >> b;
        v.erase(v.begin() + a, v.begin() + b + 1);

        if (v.empty()) {
            cout << "Empty";
        } else {
            for (const auto &a : v) {
                cout << a << " ";
            }
        }
        cout << endl;
    }

    return 0;
}
#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    int t;
    cin >> t;
    for (int tt = 0; tt < t; tt++) {
        int n, a, sum = 0;
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a;
            sum += a;
        }
        cout << sum << endl;
    }

    return 0;
}
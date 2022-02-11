#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);

    int t;
    cin >> t;
    for (int g = 0; g < t; g++) {
        string a, b;
        cin >> a >> b;

        cout << a.length() << " " << b.length() << endl;
        cout << a << b << endl;

        std::reverse(a.begin(), a.end());
        std::reverse(b.begin(), b.end());
        std::swap(a[0], b[0]);
        cout << a << " " << b << endl;
    }
    
    return 0;
}
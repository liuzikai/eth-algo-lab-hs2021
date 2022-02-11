#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    
    int t;
    cin >> t;

    int a;
    long long b;
    string c;
    double d;
    for (int i = 0; i < t; i++) {
        cin >> a >> b >> c >> d;
        cout << a << " " << b << " " << c << " " << std::fixed << std::setprecision(2) << d << endl;
    }

    return 0;
}
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    
    ios::sync_with_stdio(false);

    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {

        int n;
        cin >> n;
        
        int a, sum = 0;
        for (int j = 0; j < n; j++) {
            cin >> a;
            sum += a;
        }

        cout << sum << endl;
    }

    return 0;
}
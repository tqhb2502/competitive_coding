#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    const int total = 1 << n;
    for (int i = 0; i < total; ++i) {
        const int gray = i ^ (i >> 1);
        string code(n, '0');
        for (int bit = 0; bit < n; ++bit) {
            if (gray & (1 << bit)) {
                code[n - 1 - bit] = '1';
            }
        }
        cout << code << '\n';
    }

    return 0;
}

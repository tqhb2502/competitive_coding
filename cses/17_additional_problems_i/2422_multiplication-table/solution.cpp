#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;
    const long long target = (n * n + 1) / 2;

    long long low = 1;
    long long high = n * n;
    while (low < high) {
        long long middle = low + (high - low) / 2;
        long long count = 0;
        for (long long row = 1; row <= n; ++row) {
            count += min(n, middle / row);
        }

        if (count >= target) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    cout << low << '\n';
}

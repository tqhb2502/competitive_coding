#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    long long total = 0;
    long long longest = 0;
    for (int i = 0; i < n; ++i) {
        long long time;
        cin >> time;
        total += time;
        longest = max(longest, time);
    }

    cout << max(total, 2 * longest) << '\n';
    return 0;
}

#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<int, int> lastPosition;
    lastPosition.reserve(2 * n);
    lastPosition.max_load_factor(0.7F);

    int left = 0;
    long long answer = 0;
    for (int right = 0; right < n; ++right) {
        int value;
        cin >> value;
        const auto found = lastPosition.find(value);
        if (found != lastPosition.end()) {
            left = max(left, found->second + 1);
        }
        answer += right - left + 1;
        lastPosition[value] = right;
    }

    cout << answer << '\n';
    return 0;
}

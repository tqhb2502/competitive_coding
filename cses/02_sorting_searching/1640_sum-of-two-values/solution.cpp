#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long target;
    cin >> n >> target;

    unordered_map<long long, int> seen;
    seen.reserve(static_cast<size_t>(n) * 2);
    seen.max_load_factor(0.7F);

    for (int index = 1; index <= n; ++index) {
        long long value;
        cin >> value;

        const auto match = seen.find(target - value);
        if (match != seen.end()) {
            cout << match->second << ' ' << index << '\n';
            return 0;
        }

        seen.emplace(value, index);
    }

    cout << "IMPOSSIBLE\n";
    return 0;
}

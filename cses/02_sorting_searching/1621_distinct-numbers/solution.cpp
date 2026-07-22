#include <iostream>
#include <unordered_set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_set<long long> distinct;
    distinct.reserve(static_cast<size_t>(n) * 2);
    distinct.max_load_factor(0.7F);

    for (int i = 0; i < n; ++i) {
        long long value;
        cin >> value;
        distinct.insert(value);
    }

    cout << distinct.size() << '\n';
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> values(static_cast<size_t>(n));
    for (long long& value : values) {
        cin >> value;
    }
    vector<long long> coordinates = values;
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()), coordinates.end());
    vector<int> lastPosition(coordinates.size(), 0);

    long long answer = 0;
    for (int position = 1; position <= n; ++position) {
        long long value = values[static_cast<size_t>(position - 1)];
        size_t index = static_cast<size_t>(lower_bound(coordinates.begin(), coordinates.end(), value)
                                           - coordinates.begin());
        int previous = lastPosition[index];
        answer += static_cast<long long>(position - previous) * (n - position + 1LL);
        lastPosition[index] = position;
    }

    cout << answer << '\n';
}

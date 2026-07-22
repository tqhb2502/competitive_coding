#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<long long, int>> elements;
    elements.reserve(static_cast<size_t>(n));

    for (int position = 0; position < n; ++position) {
        long long value;
        cin >> value;
        elements.emplace_back(value, position);
    }

    sort(elements.begin(), elements.end());
    int rounds = 0;
    for (int sortedPosition = 0; sortedPosition < n; ++sortedPosition) {
        rounds = max(rounds,
                     elements[static_cast<size_t>(sortedPosition)].second - sortedPosition);
    }

    cout << rounds << '\n';
}

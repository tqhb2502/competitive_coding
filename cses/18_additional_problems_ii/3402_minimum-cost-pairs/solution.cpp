#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size = 0;
    cin >> size;
    vector<long long> values(size);
    for (long long& value : values) {
        cin >> value;
    }
    sort(values.begin(), values.end());

    constexpr long long INFINITY_COST = 3'000'000'000'000'000'000LL;
    vector<long long> cost(size + 1, INFINITY_COST);
    vector<int> left(size + 1, 0);
    vector<int> right(size + 1, size);
    vector<int> version(size + 1, 0);
    vector<char> alive(size + 1, true);

    using Entry = tuple<long long, int, int>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> queue;

    for (int edge = 0; edge <= size; ++edge) {
        left[edge] = max(0, edge - 1);
        right[edge] = min(size, edge + 1);
    }
    for (int edge = 1; edge < size; ++edge) {
        cost[edge] = values[edge] - values[edge - 1];
        queue.emplace(cost[edge], edge, version[edge]);
    }

    long long totalCost = 0;
    for (int pairCount = 1; pairCount <= size / 2; ++pairCount) {
        Entry best{};
        while (true) {
            best = queue.top();
            queue.pop();
            const auto [storedCost, edge, storedVersion] = best;
            if (alive[edge] && storedVersion == version[edge] &&
                storedCost == cost[edge]) {
                break;
            }
        }

        const auto [chosenCost, edge, storedVersion] = best;
        static_cast<void>(storedVersion);
        totalCost += chosenCost;
        cout << totalCost << (pairCount == size / 2 ? '\n' : ' ');

        const int leftEdge = left[edge];
        const int rightEdge = right[edge];
        const int outsideLeft = left[leftEdge];
        const int outsideRight = right[rightEdge];

        alive[leftEdge] = false;
        alive[rightEdge] = false;
        cost[edge] = cost[leftEdge] + cost[rightEdge] - cost[edge];
        ++version[edge];

        left[edge] = outsideLeft;
        right[edge] = outsideRight;
        right[outsideLeft] = edge;
        left[outsideRight] = edge;
        queue.emplace(cost[edge], edge, version[edge]);
    }
}

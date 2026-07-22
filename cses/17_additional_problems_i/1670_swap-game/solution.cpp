#include <array>
#include <cstdint>
#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

uint64_t swapCells(uint64_t state, int first, int second) {
    int firstShift = 4 * first;
    int secondShift = 4 * second;
    uint64_t difference = ((state >> firstShift) ^ (state >> secondShift)) & 15ULL;
    return state ^ (difference << firstShift) ^ (difference << secondShift);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint64_t start = 0;
    for (int cell = 0; cell < 9; ++cell) {
        int value;
        cin >> value;
        start |= static_cast<uint64_t>(value - 1) << (4 * cell);
    }

    uint64_t target = 0;
    for (int cell = 0; cell < 9; ++cell) {
        target |= static_cast<uint64_t>(cell) << (4 * cell);
    }

    constexpr array<pair<int, int>, 12> SWAPS{{
        {0, 1}, {1, 2}, {3, 4}, {4, 5}, {6, 7}, {7, 8},
        {0, 3}, {3, 6}, {1, 4}, {4, 7}, {2, 5}, {5, 8}
    }};

    unordered_map<uint64_t, int> distance;
    distance.reserve(400'000);
    distance.max_load_factor(0.7F);
    queue<uint64_t> states;
    distance[start] = 0;
    states.push(start);

    while (!states.empty()) {
        uint64_t state = states.front();
        states.pop();
        int currentDistance = distance[state];

        if (state == target) {
            cout << currentDistance << '\n';
            return 0;
        }

        for (auto [first, second] : SWAPS) {
            uint64_t next = swapCells(state, first, second);
            if (distance.emplace(next, currentDistance + 1).second) {
                states.push(next);
            }
        }
    }
}

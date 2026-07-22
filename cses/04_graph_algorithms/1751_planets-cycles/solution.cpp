#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> successor(n + 1);
    for (int planet = 1; planet <= n; ++planet) {
        cin >> successor[planet];
    }

    vector<int> state(n + 1, 0);
    vector<int> position(n + 1, 0);
    vector<int> answer(n + 1, 0);

    for (int start = 1; start <= n; ++start) {
        if (state[start] != 0) {
            continue;
        }

        vector<int> chain;
        int planet = start;
        while (state[planet] == 0) {
            state[planet] = 1;
            position[planet] = static_cast<int>(chain.size());
            chain.push_back(planet);
            planet = successor[planet];
        }

        if (state[planet] == 1) {
            const int cycleStart = position[planet];
            const int cycleLength = static_cast<int>(chain.size()) - cycleStart;
            for (int i = cycleStart; i < static_cast<int>(chain.size()); ++i) {
                answer[chain[i]] = cycleLength;
                state[chain[i]] = 2;
            }

            int length = cycleLength;
            for (int i = cycleStart - 1; i >= 0; --i) {
                answer[chain[i]] = ++length;
                state[chain[i]] = 2;
            }
        } else {
            int length = answer[planet];
            for (int i = static_cast<int>(chain.size()) - 1; i >= 0; --i) {
                answer[chain[i]] = ++length;
                state[chain[i]] = 2;
            }
        }
    }

    for (int planet = 1; planet <= n; ++planet) {
        if (planet > 1) {
            cout << ' ';
        }
        cout << answer[planet];
    }
    cout << '\n';
    return 0;
}

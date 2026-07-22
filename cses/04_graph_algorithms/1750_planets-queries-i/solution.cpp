#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, queries;
    cin >> n >> queries;
    constexpr int LEVELS = 30;
    vector<vector<int>> successor(LEVELS, vector<int>(n + 1));
    for (int planet = 1; planet <= n; ++planet) {
        cin >> successor[0][planet];
    }
    for (int level = 1; level < LEVELS; ++level) {
        for (int planet = 1; planet <= n; ++planet) {
            successor[level][planet] =
                successor[level - 1][successor[level - 1][planet]];
        }
    }

    while (queries--) {
        int planet, steps;
        cin >> planet >> steps;
        for (int level = 0; level < LEVELS; ++level) {
            if (steps & (1 << level)) {
                planet = successor[level][planet];
            }
        }
        cout << planet << '\n';
    }
    return 0;
}

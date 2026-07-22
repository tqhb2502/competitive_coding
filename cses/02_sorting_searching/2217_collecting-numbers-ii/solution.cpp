#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, operations;
    cin >> n >> operations;
    vector<int> values(n + 1);
    vector<int> position(n + 1);
    for (int index = 1; index <= n; ++index) {
        cin >> values[index];
        position[values[index]] = index;
    }

    int breaks = 0;
    for (int value = 1; value < n; ++value) {
        if (position[value] > position[value + 1]) {
            ++breaks;
        }
    }

    while (operations--) {
        int firstPosition, secondPosition;
        cin >> firstPosition >> secondPosition;
        const int firstValue = values[firstPosition];
        const int secondValue = values[secondPosition];

        array<int, 4> affected = {
            firstValue - 1, firstValue, secondValue - 1, secondValue
        };
        sort(affected.begin(), affected.end());

        int previous = -1;
        for (const int value : affected) {
            if (value < 1 || value >= n || value == previous) {
                continue;
            }
            breaks -= position[value] > position[value + 1];
            previous = value;
        }

        swap(values[firstPosition], values[secondPosition]);
        position[firstValue] = secondPosition;
        position[secondValue] = firstPosition;

        previous = -1;
        for (const int value : affected) {
            if (value < 1 || value >= n || value == previous) {
                continue;
            }
            breaks += position[value] > position[value + 1];
            previous = value;
        }

        cout << breaks + 1 << '\n';
    }

    return 0;
}

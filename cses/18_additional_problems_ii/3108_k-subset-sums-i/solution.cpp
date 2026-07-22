#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

using Integer = long long;
using State = pair<Integer, int>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;
    int count;
    cin >> size >> count;
    vector<Integer> values(static_cast<size_t>(size));
    Integer base = 0;
    for (Integer &value : values) {
        cin >> value;
        if (value < 0) {
            base += value;
            value = -value;
        }
    }
    sort(values.begin(), values.end());

    priority_queue<State, vector<State>, greater<State>> queue;
    cout << base;
    --count;
    if (count > 0) {
        queue.emplace(values[0], 0);
    }
    while (count-- > 0) {
        const auto [sum, index] = queue.top();
        queue.pop();
        cout << ' ' << base + sum;
        if (index + 1 < size) {
            queue.emplace(sum + values[static_cast<size_t>(index + 1)], index + 1);
            queue.emplace(sum - values[static_cast<size_t>(index)]
                              + values[static_cast<size_t>(index + 1)],
                          index + 1);
        }
    }
    cout << '\n';
}

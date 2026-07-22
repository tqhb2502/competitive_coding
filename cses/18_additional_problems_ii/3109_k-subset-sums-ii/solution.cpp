#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

using Integer = long long;

struct State {
    Integer sum;
    int parts;
    int last;
    int previous;

    bool operator>(const State &other) const {
        return sum > other.sum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;
    int subsetSize;
    int count;
    cin >> size >> subsetSize >> count;
    vector<Integer> values(static_cast<size_t>(size));
    for (Integer &value : values) {
        cin >> value;
    }
    sort(values.begin(), values.end());

    Integer initial = 0;
    for (int index = 0; index < subsetSize; ++index) {
        initial += values[static_cast<size_t>(index)];
    }

    priority_queue<State, vector<State>, greater<State>> queue;
    queue.push({initial, 0, 0, 0});
    for (int answer = 0; answer < count; ++answer) {
        const State current = queue.top();
        queue.pop();
        if (answer != 0) {
            cout << ' ';
        }
        cout << current.sum;

        if (current.parts < subsetSize) {
            const int index = subsetSize - current.parts - 1;
            queue.push({current.sum + values[static_cast<size_t>(index + 1)]
                                      - values[static_cast<size_t>(index)],
                        current.parts + 1, 1, current.last});
        }

        if (current.parts > 0
            && (current.parts == 1 || current.last < current.previous)) {
            const int baseIndex = subsetSize - current.parts;
            const int valueIndex = baseIndex + current.last;
            if (valueIndex + 1 < size) {
                queue.push({current.sum + values[static_cast<size_t>(valueIndex + 1)]
                                          - values[static_cast<size_t>(valueIndex)],
                            current.parts, current.last + 1, current.previous});
            }
        }
    }
    cout << '\n';
}

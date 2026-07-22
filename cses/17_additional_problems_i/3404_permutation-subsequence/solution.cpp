#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> position(n + 1, -1);
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        position[value] = i;
    }

    vector<pair<int, int>> sequence;
    sequence.reserve(min(n, m));
    for (int i = 0; i < m; ++i) {
        int value;
        cin >> value;
        if (value <= n) {
            sequence.emplace_back(position[value], value);
        }
    }

    const int length = static_cast<int>(sequence.size());
    vector<int> predecessor(length, -1);
    vector<int> tailValue;
    vector<int> tailIndex;
    tailValue.reserve(length);
    tailIndex.reserve(length);

    for (int i = 0; i < length; ++i) {
        const int current = sequence[i].first;
        const int place = static_cast<int>(
            lower_bound(tailValue.begin(), tailValue.end(), current) - tailValue.begin());
        if (place > 0) {
            predecessor[i] = tailIndex[place - 1];
        }
        if (place == static_cast<int>(tailValue.size())) {
            tailValue.push_back(current);
            tailIndex.push_back(i);
        } else {
            tailValue[place] = current;
            tailIndex[place] = i;
        }
    }

    vector<int> answer;
    int currentIndex = tailIndex.back();
    while (currentIndex != -1) {
        answer.push_back(sequence[currentIndex].second);
        currentIndex = predecessor[currentIndex];
    }
    reverse(answer.begin(), answer.end());

    cout << answer.size() << '\n';
    for (int value : answer) {
        cout << value << ' ';
    }
    cout << '\n';
}

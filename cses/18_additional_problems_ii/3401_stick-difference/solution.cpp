#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>

using namespace std;

struct HeapState {
    int value;
    int stick;
    int pieces;

    bool operator<(const HeapState& other) const {
        if (value != other.value) {
            return value < other.value;
        }
        return stick < other.stick;
    }
};

struct BadInterval {
    int start;
    int finish;
    int lowerRepair;
    int upperRepair;
    int coordinate = -1;
};

class PrefixMaximumTree {
public:
    explicit PrefixMaximumTree(const vector<int>& coordinates)
        : coordinates_(coordinates),
          maximumHigh_(coordinates.size() * 4U, negativeInfinity()),
          maximumActiveLow_(coordinates.size() * 4U, negativeInfinity()) {}

    void update(int index, int high) {
        update(1, 0, static_cast<int>(coordinates_.size()) - 1, index, high);
    }

    long long query(int initialMaximum, int finalLower) const {
        if (coordinates_.empty() || maximumActiveLow_[1] == negativeInfinity()) {
            return static_cast<long long>(initialMaximum) - finalLower;
        }
        int prefixMaximum = initialMaximum;
        long long answer = numeric_limits<long long>::max();
        process(1, 0, static_cast<int>(coordinates_.size()) - 1,
                prefixMaximum, answer);
        answer = min(answer,
                     static_cast<long long>(prefixMaximum) - finalLower);
        return answer;
    }

private:
    static constexpr int negativeInfinity() {
        return numeric_limits<int>::min() / 4;
    }

    void update(int node, int left, int right, int index, int high) {
        if (left == right) {
            maximumHigh_[node] = high;
            maximumActiveLow_[node] =
                high == negativeInfinity() ? negativeInfinity()
                                           : coordinates_[left];
            return;
        }
        const int middle = left + (right - left) / 2;
        if (index <= middle) {
            update(node * 2, left, middle, index, high);
        } else {
            update(node * 2 + 1, middle + 1, right, index, high);
        }
        maximumHigh_[node] =
            max(maximumHigh_[node * 2], maximumHigh_[node * 2 + 1]);
        maximumActiveLow_[node] = max(maximumActiveLow_[node * 2],
                                      maximumActiveLow_[node * 2 + 1]);
    }

    void process(int node, int left, int right, int& prefixMaximum,
                 long long& answer) const {
        if (maximumActiveLow_[node] == negativeInfinity()) {
            return;
        }
        if (answer != numeric_limits<long long>::max() &&
            static_cast<long long>(prefixMaximum) -
                    maximumActiveLow_[node] >=
                answer) {
            prefixMaximum = max(prefixMaximum, maximumHigh_[node]);
            return;
        }
        if (maximumHigh_[node] <= prefixMaximum) {
            answer = min(answer,
                         static_cast<long long>(prefixMaximum) -
                             maximumActiveLow_[node]);
            return;
        }
        if (left == right) {
            answer = min(answer,
                         static_cast<long long>(prefixMaximum) -
                             coordinates_[left]);
            prefixMaximum = max(prefixMaximum, maximumHigh_[node]);
            return;
        }
        const int middle = left + (right - left) / 2;
        process(node * 2, left, middle, prefixMaximum, answer);
        process(node * 2 + 1, middle + 1, right, prefixMaximum, answer);
    }

    const vector<int>& coordinates_;
    vector<int> maximumHigh_;
    vector<int> maximumActiveLow_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int stickCount = 0;
    int maximumCuts = 0;
    cin >> stickCount >> maximumCuts;
    vector<int> length(stickCount);
    for (int& value : length) {
        cin >> value;
    }

    vector<int> minimumUpper(maximumCuts + 1, 0);
    priority_queue<HeapState> upperQueue;
    for (int stick = 0; stick < stickCount; ++stick) {
        upperQueue.push({length[stick], stick, 1});
    }
    minimumUpper[0] = upperQueue.top().value;
    for (int cuts = 1; cuts <= maximumCuts; ++cuts) {
        HeapState state = upperQueue.top();
        upperQueue.pop();
        ++state.pieces;
        state.value =
            (length[state.stick] + state.pieces - 1) / state.pieces;
        upperQueue.push(state);
        minimumUpper[cuts] = upperQueue.top().value;
    }

    vector<int> maximumLower(maximumCuts + 1, 0);
    priority_queue<HeapState> lowerQueue;
    maximumLower[0] = *min_element(length.begin(), length.end());
    for (int stick = 0; stick < stickCount; ++stick) {
        lowerQueue.push({length[stick] / 2, stick, 2});
    }
    for (int cuts = 1; cuts <= maximumCuts; ++cuts) {
        HeapState state = lowerQueue.top();
        lowerQueue.pop();
        maximumLower[cuts] = min(maximumLower[cuts - 1], state.value);
        ++state.pieces;
        state.value = length[state.stick] / state.pieces;
        lowerQueue.push(state);
    }

    const auto firstIndex = [maximumCuts](const function<bool(int)>& predicate) {
        int left = 0;
        int right = maximumCuts + 1;
        while (left < right) {
            const int middle = left + (right - left) / 2;
            if (middle <= maximumCuts && predicate(middle)) {
                right = middle;
            } else {
                left = middle + 1;
            }
        }
        return left;
    };

    vector<BadInterval> intervals;
    intervals.reserve(maximumCuts);
    vector<int> coordinates;
    coordinates.reserve(maximumCuts);

    for (const int value : length) {
        const int finalRequiredPieces =
            (value + minimumUpper[maximumCuts] - 1) /
            minimumUpper[maximumCuts];
        for (int pieces = 1; pieces < finalRequiredPieces; ++pieces) {
            const int firstLower = firstIndex([&](int cuts) {
                return value / maximumLower[cuts] >= pieces;
            });
            const int afterLower = firstIndex([&](int cuts) {
                return value / maximumLower[cuts] >= pieces + 1;
            });
            const int firstUpper = firstIndex([&](int cuts) {
                return (value + minimumUpper[cuts] - 1) /
                           minimumUpper[cuts] >=
                       pieces + 1;
            });
            const int afterUpper = firstIndex([&](int cuts) {
                return (value + minimumUpper[cuts] - 1) /
                           minimumUpper[cuts] >=
                       pieces + 2;
            });

            const int start = max({1, firstLower, firstUpper});
            const int finish =
                min({maximumCuts, afterLower - 1, afterUpper - 1});
            if (start > finish) {
                continue;
            }
            if (value / maximumLower[start] != pieces ||
                (value + minimumUpper[start] - 1) /
                        minimumUpper[start] !=
                    pieces + 1) {
                continue;
            }

            const int lowerRepair = value / (pieces + 1);
            const int upperRepair = (value + pieces - 1) / pieces;
            intervals.push_back(
                {start, finish, lowerRepair, upperRepair, -1});
            coordinates.push_back(lowerRepair);
        }
    }

    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()),
                      coordinates.end());

    vector<vector<int>> additions(maximumCuts + 2);
    vector<vector<int>> removals(maximumCuts + 2);
    for (int index = 0; index < static_cast<int>(intervals.size()); ++index) {
        BadInterval& interval = intervals[index];
        interval.coordinate = static_cast<int>(
            lower_bound(coordinates.begin(), coordinates.end(),
                        interval.lowerRepair) -
            coordinates.begin());
        additions[interval.start].push_back(index);
        removals[interval.finish + 1].push_back(index);
    }

    PrefixMaximumTree tree(coordinates);
    vector<multiset<int>> activeHigh(coordinates.size());
    constexpr int NEGATIVE_INFINITY = numeric_limits<int>::min() / 4;

    for (int cuts = 1; cuts <= maximumCuts; ++cuts) {
        for (const int index : removals[cuts]) {
            const BadInterval& interval = intervals[index];
            auto& values = activeHigh[interval.coordinate];
            const auto iterator = values.find(interval.upperRepair);
            values.erase(iterator);
            tree.update(interval.coordinate,
                        values.empty() ? NEGATIVE_INFINITY : *values.rbegin());
        }
        for (const int index : additions[cuts]) {
            const BadInterval& interval = intervals[index];
            auto& values = activeHigh[interval.coordinate];
            values.insert(interval.upperRepair);
            tree.update(interval.coordinate, *values.rbegin());
        }

        cout << tree.query(minimumUpper[cuts], maximumLower[cuts])
             << (cuts == maximumCuts ? '\n' : ' ');
    }
}

#include <iostream>
#include <iterator>
#include <set>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string bits;
    cin >> bits;
    const int n = static_cast<int>(bits.size());

    set<int> boundaries{0, n};
    for (int i = 1; i < n; ++i) {
        if (bits[i - 1] != bits[i]) {
            boundaries.insert(i);
        }
    }

    multiset<int> lengths;
    for (auto it = next(boundaries.begin()); it != boundaries.end(); ++it) {
        lengths.insert(*it - *prev(it));
    }

    const auto eraseOneLength = [&lengths](int value) {
        lengths.erase(lengths.find(value));
    };

    const auto toggleBoundary = [&boundaries, &lengths, &eraseOneLength](int point) {
        const auto found = boundaries.find(point);
        if (found == boundaries.end()) {
            const auto rightIterator = boundaries.upper_bound(point);
            const int right = *rightIterator;
            const int left = *prev(rightIterator);
            eraseOneLength(right - left);
            lengths.insert(point - left);
            lengths.insert(right - point);
            boundaries.insert(point);
        } else {
            const int left = *prev(found);
            const int right = *next(found);
            eraseOneLength(point - left);
            eraseOneLength(right - point);
            lengths.insert(right - left);
            boundaries.erase(found);
        }
    };

    int changes;
    cin >> changes;
    for (int query = 0; query < changes; ++query) {
        int position;
        cin >> position;
        --position;
        if (position > 0) {
            toggleBoundary(position);
        }
        if (position + 1 < n) {
            toggleBoundary(position + 1);
        }
        cout << *lengths.rbegin() << (query + 1 == changes ? '\n' : ' ');
    }
}

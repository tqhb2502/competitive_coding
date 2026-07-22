#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Range {
    int left;
    int right;
    int index;
};

void printFlags(const vector<int>& flags) {
    for (size_t i = 0; i < flags.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << flags[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Range> ranges(n);
    for (int i = 0; i < n; ++i) {
        cin >> ranges[i].left >> ranges[i].right;
        ranges[i].index = i;
    }

    vector<int> contains(n, 0);
    vector<int> contained(n, 0);

    sort(ranges.begin(), ranges.end(), [](const Range& first, const Range& second) {
        if (first.left != second.left) {
            return first.left < second.left;
        }
        return first.right > second.right;
    });

    int maximumRight = -1;
    for (const Range& range : ranges) {
        if (range.right <= maximumRight) {
            contained[range.index] = 1;
        }
        maximumRight = max(maximumRight, range.right);
    }

    sort(ranges.begin(), ranges.end(), [](const Range& first, const Range& second) {
        if (first.left != second.left) {
            return first.left > second.left;
        }
        return first.right < second.right;
    });

    int minimumRight = 1'000'000'001;
    for (const Range& range : ranges) {
        if (range.right >= minimumRight) {
            contains[range.index] = 1;
        }
        minimumRight = min(minimumRight, range.right);
    }

    printFlags(contains);
    printFlags(contained);
    return 0;
}

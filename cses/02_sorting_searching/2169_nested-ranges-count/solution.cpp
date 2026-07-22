#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Range {
    int left;
    int right;
    int index;
    int rightRank;
};

class FenwickTree {
public:
    explicit FenwickTree(int size) : tree(size + 1, 0) {}

    void add(int index) {
        for (int i = index; i < static_cast<int>(tree.size()); i += i & -i) {
            ++tree[i];
        }
    }

    int prefixSum(int index) const {
        int result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree[i];
        }
        return result;
    }

private:
    vector<int> tree;
};

void printCounts(const vector<int>& counts) {
    for (size_t i = 0; i < counts.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << counts[i];
    }
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Range> ranges(n);
    vector<int> rightEndpoints(n);
    for (int i = 0; i < n; ++i) {
        cin >> ranges[i].left >> ranges[i].right;
        ranges[i].index = i;
        rightEndpoints[i] = ranges[i].right;
    }

    sort(rightEndpoints.begin(), rightEndpoints.end());
    rightEndpoints.erase(unique(rightEndpoints.begin(), rightEndpoints.end()),
                         rightEndpoints.end());
    for (Range& range : ranges) {
        range.rightRank = static_cast<int>(
            lower_bound(rightEndpoints.begin(), rightEndpoints.end(), range.right) -
            rightEndpoints.begin()) + 1;
    }

    vector<int> contains(n, 0);
    vector<int> contained(n, 0);

    sort(ranges.begin(), ranges.end(), [](const Range& first, const Range& second) {
        if (first.left != second.left) {
            return first.left > second.left;
        }
        return first.right < second.right;
    });

    FenwickTree containsTree(static_cast<int>(rightEndpoints.size()));
    for (const Range& range : ranges) {
        contains[range.index] = containsTree.prefixSum(range.rightRank);
        containsTree.add(range.rightRank);
    }

    sort(ranges.begin(), ranges.end(), [](const Range& first, const Range& second) {
        if (first.left != second.left) {
            return first.left < second.left;
        }
        return first.right > second.right;
    });

    FenwickTree containedTree(static_cast<int>(rightEndpoints.size()));
    int processed = 0;
    for (const Range& range : ranges) {
        contained[range.index] =
            processed - containedTree.prefixSum(range.rightRank - 1);
        containedTree.add(range.rightRank);
        ++processed;
    }

    printCounts(contains);
    printCounts(contained);
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Một đoạn [left, right]; index là thứ tự nhập ban đầu để in đúng vị trí,
// rightRank là rank của right sau khi nén tọa độ (dùng làm chỉ số cho BIT).
struct Range {
    int left;
    int right;
    int index;
    int rightRank;
};

// Fenwick tree (BIT) đánh chỉ số theo rank của b, lưu số đoạn đã chèn.
class FenwickTree {
public:
    explicit FenwickTree(int size) : tree(size + 1, 0) {}

    // Chèn thêm một đoạn có right ở vị trí index.
    void add(int index) {
        for (int i = index; i < static_cast<int>(tree.size()); i += i & -i) {
            ++tree[i];
        }
    }

    // Đếm số đoạn đã chèn có rank <= index (prefix sum).
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

    // Nén tọa độ cho các giá trị right rồi gán rank 1-indexed cho mỗi đoạn.
    sort(rightEndpoints.begin(), rightEndpoints.end());
    rightEndpoints.erase(unique(rightEndpoints.begin(), rightEndpoints.end()),
                         rightEndpoints.end());
    for (Range& range : ranges) {
        range.rightRank = static_cast<int>(
            lower_bound(rightEndpoints.begin(), rightEndpoints.end(), range.right) -
            rightEndpoints.begin()) + 1;
    }

    vector<int> contains(n, 0);   // số đoạn mà i chứa
    vector<int> contained(n, 0);  // số đoạn chứa i

    // Pass 1 - tính contains[i] = số j thỏa a_i <= a_j và b_j <= b_i.
    // Sắp xếp theo a giảm dần, hòa thì b tăng dần; khi xử lý i mọi j đã chèn
    // đều thỏa a_i <= a_j, chỉ cần đếm prefix b_j <= b_i.
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

    // Pass 2 - tính contained[i] = số j thỏa a_j <= a_i và b_i <= b_j.
    // Sắp xếp theo a tăng dần, hòa thì b giảm dần; khi xử lý i mọi j đã chèn
    // đều thỏa a_j <= a_i. Số b_j >= b_i = (số đã chèn) - (số b_j < b_i).
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

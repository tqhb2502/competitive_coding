#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Range {
    int left;
    int right;
    int index;  // vị trí ban đầu để in kết quả đúng thứ tự nhập
};

// In một dòng gồm các cờ 0/1, ngăn cách bởi dấu cách.
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

    vector<int> contains(n, 0);   // đoạn i có chứa đoạn khác?
    vector<int> contained(n, 0);  // đoạn i có bị đoạn khác chứa?

    // (A) "bị chứa": sắp x tăng dần, nếu x bằng thì y giảm dần.
    sort(ranges.begin(), ranges.end(), [](const Range& first, const Range& second) {
        if (first.left != second.left) {
            return first.left < second.left;
        }
        return first.right > second.right;
    });

    // Quét một lượt giữ max right; đoạn nào có right <= max thì bị đoạn trước bao trọn.
    int maximumRight = -1;
    for (const Range& range : ranges) {
        if (range.right <= maximumRight) {
            contained[range.index] = 1;
        }
        maximumRight = max(maximumRight, range.right);
    }

    // (B) "chứa đoạn khác": sắp x giảm dần, nếu x bằng thì y tăng dần.
    sort(ranges.begin(), ranges.end(), [](const Range& first, const Range& second) {
        if (first.left != second.left) {
            return first.left > second.left;
        }
        return first.right < second.right;
    });

    // Quét một lượt giữ min right; đoạn nào có right >= min thì bao trọn đoạn trước.
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

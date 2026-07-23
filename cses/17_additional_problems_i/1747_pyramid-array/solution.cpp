#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// Fenwick tree để đếm số phần tử còn hoạt động trong một khoảng tiền tố.
class FenwickTree {
public:
    explicit FenwickTree(int size) : tree(size + 1, 0) {}

    void add(int index, int value) {
        const int size = static_cast<int>(tree.size());
        for (int i = index; i < size; i += i & -i) {
            tree[i] += value;
        }
    }

    // Tổng tiền tố (prefix sum) từ vị trí 1 tới index.
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Lưu (giá trị, vị trí gốc) để sắp xếp theo giá trị tăng dần.
    vector<pair<long long, int>> elements;
    elements.reserve(n);
    for (int position = 1; position <= n; ++position) {
        long long value;
        cin >> value;
        elements.emplace_back(value, position);
    }
    sort(elements.begin(), elements.end());

    // Ban đầu mọi vị trí đều đang hoạt động (đánh dấu 1 trên Fenwick tree).
    FenwickTree active(n);
    for (int position = 1; position <= n; ++position) {
        active.add(position, 1);
    }

    long long answer = 0;
    int remaining = n;
    // Duyệt phần tử theo giá trị tăng dần: phần tử nhỏ nhất còn lại phải ra biên.
    for (const auto &[value, position] : elements) {
        static_cast<void>(value);
        // Số phần tử còn hoạt động ở bên trái và bên phải vị trí hiện tại.
        const int left = active.prefixSum(position - 1);
        const int right = remaining - 1 - left;
        // Đưa ra đầu gần hơn tốn min(left, right) lần hoán đổi kề.
        answer += min(left, right);
        // Xóa vị trí này khỏi tập đang hoạt động.
        active.add(position, -1);
        --remaining;
    }

    cout << answer << '\n';
}

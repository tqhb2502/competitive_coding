#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

// Mỗi node lưu 4 đại lượng cho đoạn nó quản lý:
//   total  = tổng cả đoạn
//   prefix = tiền tố lớn nhất (đã kẹp >= 0)
//   suffix = hậu tố lớn nhất (đã kẹp >= 0)
//   best   = tổng đoạn con lớn nhất trong đoạn (đã kẹp >= 0)
struct Node {
    long long total = 0;
    long long prefix = 0;
    long long suffix = 0;
    long long best = 0;
};

// Hợp hai con trái/phải thành node cha; best xét thêm đoạn vắt qua ranh giới
// (hậu tố của L nối tiền tố của R).
Node merge_nodes(const Node& left, const Node& right) {
    Node result;
    result.total = left.total + right.total;
    result.prefix = std::max(left.prefix, left.total + right.prefix);
    result.suffix = std::max(right.suffix, right.total + left.suffix);
    result.best = std::max({left.best, right.best, left.suffix + right.prefix});
    return result;
}

// Lá ứng với giá trị value; các đại lượng dương được kẹp >= 0 để cho phép đoạn rỗng.
Node make_leaf(long long value) {
    const long long nonnegative = std::max(0LL, value);
    return {value, nonnegative, nonnegative, nonnegative};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;

    // Kích thước cây làm tròn lên lũy thừa của 2; lá đệm mang phần tử trung hòa.
    std::size_t size = 1;
    while (size < n) {
        size *= 2;
    }
    std::vector<Node> tree(2 * size);
    for (std::size_t i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        tree[size + i] = make_leaf(value);
    }
    // Dựng cây từ dưới lên: node cha = hợp của hai con.
    for (std::size_t node = size; node-- > 1;) {
        tree[node] = merge_nodes(tree[2 * node], tree[2 * node + 1]);
    }

    for (std::size_t update = 0; update < m; ++update) {
        std::size_t position = 0;
        long long value = 0;
        std::cin >> position >> value;

        // Point update: sửa 1 lá rồi đi ngược lên gốc cập nhật các node cha.
        std::size_t node = size + position - 1;
        tree[node] = make_leaf(value);
        for (node /= 2; node >= 1; node /= 2) {
            tree[node] = merge_nodes(tree[2 * node], tree[2 * node + 1]);
        }
        // Đáp án của toàn mảng nằm ở node gốc.
        std::cout << tree[1].best << '\n';
    }
    return 0;
}

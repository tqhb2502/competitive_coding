#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;

    // size = lũy thừa 2 nhỏ nhất >= n (kích thước tầng lá của segment tree)
    std::size_t size = 1;
    while (size < n) {
        size *= 2;
    }

    // Max segment tree: nạp số phòng trống vào các lá rồi dựng cây từ dưới lên
    std::vector<long long> tree(2 * size, 0);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> tree[size + i];
    }
    for (std::size_t node = size; node-- > 1;) {
        tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
    }

    for (std::size_t query = 0; query < m; ++query) {
        long long rooms = 0;
        std::cin >> rooms;

        std::size_t answer = 0;
        // Nếu max toàn mảng (gốc) < rooms thì không khách sạn nào đủ -> in 0
        if (tree[1] >= rooms) {
            // Đi xuống (descent) tìm lá trái nhất có số phòng trống >= rooms:
            // ưu tiên con trái nếu con trái còn đủ phòng
            std::size_t node = 1;
            while (node < size) {
                if (tree[2 * node] >= rooms) {
                    node *= 2;
                } else {
                    node = 2 * node + 1;
                }
            }

            // node là chỉ số lá; khách sạn đánh số từ 1
            answer = node - size + 1;

            // Point update: trừ rooms vào lá rồi cập nhật lại max các tổ tiên
            tree[node] -= rooms;
            for (node /= 2; node >= 1; node /= 2) {
                tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
            }
        }

        if (query > 0) {
            std::cout << ' ';
        }
        std::cout << answer;
    }
    std::cout << '\n';
    return 0;
}

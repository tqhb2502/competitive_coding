#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long skipped;  // số đứa bị bỏ qua mỗi lượt (k), có thể tới 10^9
    std::cin >> n >> skipped;

    // Kích thước lá của segment tree: lũy thừa 2 nhỏ nhất >= n
    int size = 1;
    while (size < n) {
        size *= 2;
    }

    // Order-statistics segment tree: mỗi lá 1..n = 1 (còn tồn tại), node trong = tổng hai con
    std::vector<int> tree(2 * size, 0);
    for (int i = 0; i < n; ++i) {
        tree[size + i] = 1;
    }
    for (int node = size - 1; node >= 1; --node) {
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long position = 0;  // chỉ số 0-based điểm bắt đầu đếm trong các phần tử còn lại
    for (int remaining = n; remaining >= 1; --remaining) {
        // Chỉ số 0-based của đứa bị loại lượt này, rồi đổi sang thứ hạng 1-based
        const long long removed_index = (position + skipped) % remaining;
        int target = static_cast<int>(removed_index) + 1;

        // Đi xuống một lần: vừa tìm phần tử thứ target còn tồn tại vừa xóa nó dọc đường
        int node = 1;
        --tree[node];
        while (node < size) {
            const int left = 2 * node;
            if (tree[left] >= target) {
                node = left;                 // đủ phần tử bên trái -> rẽ trái
            } else {
                target -= tree[left];        // bỏ qua nhánh trái -> rẽ phải
                node = left + 1;
            }
            --tree[node];
        }

        // Ghi số hiệu đứa vừa bị loại (node - size + 1 là lá 1-based)
        if (remaining < n) {
            std::cout << ' ';
        }
        std::cout << node - size + 1;

        // Đứa ngay sau chiếm đúng chỉ số removed_index trong mảng còn lại mới
        position = removed_index;
    }
    std::cout << '\n';

    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n, query_count;
    std::cin >> n >> query_count;

    // Số mức của bảng nâng nhị phân: đủ để bao phủ độ sâu lớn nhất
    std::size_t levels = 1;
    while ((std::size_t{1} << levels) <= n) {
        ++levels;
    }

    // ancestor[0][v] = sếp trực tiếp của v; depth[v] = độ sâu của v (gốc sâu 0)
    std::vector<std::size_t> depth(n + 1, 0);
    std::vector<std::vector<std::size_t>> ancestor(
        levels, std::vector<std::size_t>(n + 1, 1));
    ancestor[0][1] = 1;  // gốc tự trỏ chính nó để phép nâng luôn dừng ở gốc
    // Vì sếp có chỉ số nhỏ hơn nên duyệt tăng dần tính được depth ngay, không cần DFS
    for (std::size_t employee = 2; employee <= n; ++employee) {
        std::cin >> ancestor[0][employee];
        depth[employee] = depth[ancestor[0][employee]] + 1;
    }

    // Dựng bảng: ancestor[k][v] = tổ tiên thứ 2^k của v (lặp, không đệ quy)
    for (std::size_t level = 1; level < levels; ++level) {
        for (std::size_t employee = 1; employee <= n; ++employee) {
            ancestor[level][employee] =
                ancestor[level - 1][ancestor[level - 1][employee]];
        }
    }

    while (query_count-- > 0) {
        std::size_t a, b;
        std::cin >> a >> b;
        if (depth[a] < depth[b]) {
            std::swap(a, b);  // giữ a là nút sâu hơn
        }

        // Đưa a lên cùng độ sâu với b theo các bit của hiệu độ sâu
        std::size_t difference = depth[a] - depth[b];
        for (std::size_t level = 0; level < levels; ++level) {
            if ((difference & (std::size_t{1} << level)) != 0) {
                a = ancestor[level][a];
            }
        }

        // Nếu a != b, nâng đồng thời từ bit cao xuống thấp tới ngay dưới LCA
        if (a != b) {
            for (std::size_t level = levels; level-- > 0;) {
                if (ancestor[level][a] != ancestor[level][b]) {
                    a = ancestor[level][a];
                    b = ancestor[level][b];
                }
            }
            a = ancestor[0][a];  // sếp của a chính là LCA
        }

        std::cout << a << '\n';
    }
}

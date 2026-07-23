#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n, query_count;
    std::cin >> n >> query_count;

    // Chọn số tầng LOG sao cho 2^levels > n, đủ biểu diễn mọi k <= n
    std::size_t levels = 1;
    while ((std::size_t{1} << levels) <= n) {
        ++levels;
    }

    // Bảng binary lifting: ancestor[j][v] = tổ tiên cách v đúng 2^j bậc
    // (node ảo 0 dùng cho trường hợp "rơi" ra ngoài cây)
    std::vector<std::vector<std::size_t>> ancestor(
        levels, std::vector<std::size_t>(n + 1, 0));

    // Tầng 0 chính là cha trực tiếp; ancestor[0][1] = 0 vì gốc không có cha
    for (std::size_t employee = 2; employee <= n; ++employee) {
        std::cin >> ancestor[0][employee];
    }

    // Xây các tầng cao theo công thức 2^j = 2^(j-1) + 2^(j-1)
    for (std::size_t level = 1; level < levels; ++level) {
        for (std::size_t employee = 1; employee <= n; ++employee) {
            ancestor[level][employee] =
                ancestor[level - 1][ancestor[level - 1][employee]];
        }
    }

    while (query_count-- > 0) {
        std::size_t employee, steps;
        std::cin >> employee >> steps;

        // Nhảy theo từng bit được bật của steps; dừng sớm nếu đã rơi về node ảo 0
        for (std::size_t level = 0; level < levels && employee != 0; ++level) {
            if ((steps & (std::size_t{1} << level)) != 0) {
                employee = ancestor[level][employee];
            }
        }

        // employee == 0 nghĩa là tổ tiên không tồn tại
        if (employee == 0) {
            std::cout << -1 << '\n';
        } else {
            std::cout << employee << '\n';
        }
    }
}

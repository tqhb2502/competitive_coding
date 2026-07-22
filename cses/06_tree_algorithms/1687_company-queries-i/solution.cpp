#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n, query_count;
    std::cin >> n >> query_count;

    std::size_t levels = 1;
    while ((std::size_t{1} << levels) <= n) {
        ++levels;
    }

    std::vector<std::vector<std::size_t>> ancestor(
        levels, std::vector<std::size_t>(n + 1, 0));
    for (std::size_t employee = 2; employee <= n; ++employee) {
        std::cin >> ancestor[0][employee];
    }

    for (std::size_t level = 1; level < levels; ++level) {
        for (std::size_t employee = 1; employee <= n; ++employee) {
            ancestor[level][employee] =
                ancestor[level - 1][ancestor[level - 1][employee]];
        }
    }

    while (query_count-- > 0) {
        std::size_t employee, steps;
        std::cin >> employee >> steps;

        for (std::size_t level = 0; level < levels && employee != 0; ++level) {
            if ((steps & (std::size_t{1} << level)) != 0) {
                employee = ancestor[level][employee];
            }
        }

        if (employee == 0) {
            std::cout << -1 << '\n';
        } else {
            std::cout << employee << '\n';
        }
    }
}

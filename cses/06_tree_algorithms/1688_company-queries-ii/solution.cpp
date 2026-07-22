#include <algorithm>
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

    std::vector<std::size_t> depth(n + 1, 0);
    std::vector<std::vector<std::size_t>> ancestor(
        levels, std::vector<std::size_t>(n + 1, 1));
    ancestor[0][1] = 1;
    for (std::size_t employee = 2; employee <= n; ++employee) {
        std::cin >> ancestor[0][employee];
        depth[employee] = depth[ancestor[0][employee]] + 1;
    }

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
            std::swap(a, b);
        }

        std::size_t difference = depth[a] - depth[b];
        for (std::size_t level = 0; level < levels; ++level) {
            if ((difference & (std::size_t{1} << level)) != 0) {
                a = ancestor[level][a];
            }
        }

        if (a != b) {
            for (std::size_t level = levels; level-- > 0;) {
                if (ancestor[level][a] != ancestor[level][b]) {
                    a = ancestor[level][a];
                    b = ancestor[level][b];
                }
            }
            a = ancestor[0][a];
        }

        std::cout << a << '\n';
    }
}

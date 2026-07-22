#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    const int width = m + 2;
    std::vector<char> floor((n + 2) * width, false);
    for (int row = 1; row <= n; ++row) {
        std::string line;
        std::cin >> line;
        for (int column = 1; column <= m; ++column) {
            floor[row * width + column] = (line[column - 1] == '.');
        }
    }

    int rooms = 0;
    std::vector<int> stack;
    stack.reserve(n * m);

    for (int start = 0; start < static_cast<int>(floor.size()); ++start) {
        if (!floor[start]) {
            continue;
        }

        ++rooms;
        floor[start] = false;
        stack.push_back(start);

        while (!stack.empty()) {
            const int cell = stack.back();
            stack.pop_back();

            const int neighbors[4] = {cell - 1, cell + 1, cell - width,
                                      cell + width};
            for (int neighbor : neighbors) {
                if (floor[neighbor]) {
                    floor[neighbor] = false;
                    stack.push_back(neighbor);
                }
            }
        }
    }

    std::cout << rooms << '\n';
    return 0;
}

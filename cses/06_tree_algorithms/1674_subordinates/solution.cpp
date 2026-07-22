#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n;
    std::cin >> n;

    std::vector<std::size_t> parent(n + 1, 0);
    std::vector<std::vector<std::size_t>> children(n + 1);
    for (std::size_t employee = 2; employee <= n; ++employee) {
        std::cin >> parent[employee];
        children[parent[employee]].push_back(employee);
    }

    std::vector<std::size_t> order;
    order.reserve(n);
    std::vector<std::size_t> stack = {1};
    while (!stack.empty()) {
        std::size_t employee = stack.back();
        stack.pop_back();
        order.push_back(employee);
        for (std::size_t subordinate : children[employee]) {
            stack.push_back(subordinate);
        }
    }

    std::vector<std::size_t> subordinate_count(n + 1, 0);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        std::size_t employee = *it;
        if (parent[employee] != 0) {
            subordinate_count[parent[employee]] += subordinate_count[employee] + 1;
        }
    }

    for (std::size_t employee = 1; employee <= n; ++employee) {
        if (employee > 1) {
            std::cout << ' ';
        }
        std::cout << subordinate_count[employee];
    }
    std::cout << '\n';
}

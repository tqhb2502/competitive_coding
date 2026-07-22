#include <deque>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::deque<int> children;
    for (int child = 1; child <= n; ++child) {
        children.push_back(child);
    }

    std::vector<int> order;
    order.reserve(n);
    while (!children.empty()) {
        children.push_back(children.front());
        children.pop_front();
        order.push_back(children.front());
        children.pop_front();
    }

    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << order[i];
    }
    std::cout << '\n';

    return 0;
}

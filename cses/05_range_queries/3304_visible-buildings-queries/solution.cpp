#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> height(n + 2, 0);
    for (int building = 1; building <= n; ++building) {
        std::cin >> height[building];
    }

    const int sentinel = n + 1;
    std::vector<int> next_greater(n + 2, sentinel);
    std::vector<int> stack;
    stack.reserve(n);
    for (int building = n; building >= 1; --building) {
        while (!stack.empty() && height[stack.back()] <= height[building]) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            next_greater[building] = stack.back();
        }
        stack.push_back(building);
    }
    next_greater[sentinel] = sentinel;

    int levels = 1;
    while ((1 << levels) < n + 1) {
        ++levels;
    }
    std::vector<std::vector<int>> jump(
        levels, std::vector<int>(n + 2, sentinel)
    );
    jump[0] = next_greater;
    for (int level = 1; level < levels; ++level) {
        for (int building = 1; building <= sentinel; ++building) {
            jump[level][building] =
                jump[level - 1][jump[level - 1][building]];
        }
    }

    while (q-- > 0) {
        int first, last;
        std::cin >> first >> last;
        int visible = 1;
        int building = first;
        for (int level = levels - 1; level >= 0; --level) {
            if (jump[level][building] <= last) {
                building = jump[level][building];
                visible += 1 << level;
            }
        }
        std::cout << visible << '\n';
    }
    return 0;
}

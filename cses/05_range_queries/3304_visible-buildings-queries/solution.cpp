#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    // Chiều cao các tòa nhà, đánh chỉ số từ 1.
    std::vector<int> height(n + 2, 0);
    for (int building = 1; building <= n; ++building) {
        std::cin >> height[building];
    }

    // next_greater[i] = tòa đầu tiên bên phải cao hơn NGHIÊM NGẶT tòa i,
    // nếu không có thì bằng sentinel = n+1. Tính bằng monotonic stack O(n).
    const int sentinel = n + 1;
    std::vector<int> next_greater(n + 2, sentinel);
    std::vector<int> stack;
    stack.reserve(n);
    for (int building = n; building >= 1; --building) {
        // Loại các tòa không cao hơn tòa hiện tại khỏi đỉnh stack.
        while (!stack.empty() && height[stack.back()] <= height[building]) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            next_greater[building] = stack.back();
        }
        stack.push_back(building);
    }
    next_greater[sentinel] = sentinel;  // trạng thái hấp thụ cho sentinel

    // Số tầng binary lifting: LOG ~ ceil(log2(n)).
    int levels = 1;
    while ((1 << levels) < n + 1) {
        ++levels;
    }

    // jump[level][i] = vị trí sau khi nhảy 2^level bước theo con trỏ next_greater.
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

    // Trả lời truy vấn: đếm số bước nhảy còn nằm trong [first, last].
    while (q-- > 0) {
        int first, last;
        std::cin >> first >> last;
        int visible = 1;          // tòa first luôn visible
        int building = first;
        // Duyệt tầng từ cao xuống thấp, nhảy khi vẫn còn trong đoạn.
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

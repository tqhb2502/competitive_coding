#include <deque>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Đưa tất cả n đứa trẻ vào deque theo thứ tự 1..n để mô phỏng vòng tròn.
    std::deque<int> children;
    for (int child = 1; child <= n; ++child) {
        children.push_back(child);
    }

    // Lặp cho tới khi deque rỗng: bỏ qua đứa đầu (đưa xuống cuối) rồi loại đứa tiếp theo.
    std::vector<int> order;
    order.reserve(n);
    while (!children.empty()) {
        // Bỏ qua (skip): chuyển phần tử đầu xuống cuối hàng, nó vẫn còn trong vòng.
        children.push_back(children.front());
        children.pop_front();
        // Loại (remove): phần tử đầu tiếp theo bị loại, ghi vào thứ tự kết quả.
        order.push_back(children.front());
        children.pop_front();
    }

    // Xuất thứ tự bị loại, các số cách nhau bởi dấu cách.
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << order[i];
    }
    std::cout << '\n';

    return 0;
}

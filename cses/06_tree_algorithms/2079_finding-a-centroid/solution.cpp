#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // Đọc n-1 cạnh vô hướng và lưu cây bằng adjacency list.
    std::vector<std::vector<int>> graph(n + 1);
    for (int edge = 0; edge < n - 1; ++edge) {
        int first, second;
        std::cin >> first >> second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    // Iterative DFS từ gốc 1 (dùng stack tường minh, không đệ quy) để tính
    // parent của mỗi đỉnh và thứ tự duyệt preorder.
    std::vector<int> parent(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack{1};
    parent[1] = -1;

    while (!stack.empty()) {
        const int node = stack.back();
        stack.pop_back();
        order.push_back(node);
        for (const int next : graph[node]) {
            if (next == parent[node]) {
                continue;
            }
            parent[next] = node;
            stack.push_back(next);
        }
    }

    // Tính size[u] = số đỉnh trong subtree gốc u bằng cách duyệt ngược
    // preorder và cộng dồn kích thước con lên cha.
    std::vector<int> subtree_size(n + 1, 1);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int node = *iterator;
        if (parent[node] > 0) {
            subtree_size[parent[node]] += subtree_size[node];
        }
    }

    // Với mỗi đỉnh, tìm thành phần liên thông lớn nhất khi xóa nó: gồm phần
    // "phía trên" (n - size[node]) và mỗi subtree con (size[next]).
    // Centroid là đỉnh đầu tiên có thành phần lớn nhất <= floor(n/2).
    const int half = n / 2;
    for (int node = 1; node <= n; ++node) {
        int largest_component = n - subtree_size[node];
        for (const int next : graph[node]) {
            if (parent[next] == node) {
                largest_component = std::max(
                    largest_component, subtree_size[next]
                );
            }
        }
        if (largest_component <= half) {
            std::cout << node << '\n';
            break;
        }
    }
    return 0;
}

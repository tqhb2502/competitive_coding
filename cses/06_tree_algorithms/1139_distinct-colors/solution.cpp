#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n;
    std::cin >> n;

    std::vector<int> color(n + 1);
    for (std::size_t node = 1; node <= n; ++node) {
        std::cin >> color[node];
    }

    std::vector<std::vector<std::size_t>> adjacency(n + 1);
    for (std::size_t edge = 0; edge + 1 < n; ++edge) {
        std::size_t a, b;
        std::cin >> a >> b;
        adjacency[a].push_back(b);
        adjacency[b].push_back(a);
    }

    // DFS lặp (không đệ quy): tính parent[] và order[] sao cho cha luôn đứng trước con.
    std::vector<std::size_t> parent(n + 1, 0);
    std::vector<std::size_t> order;
    order.reserve(n);
    std::vector<std::size_t> stack = {1};
    while (!stack.empty()) {
        std::size_t node = stack.back();
        stack.pop_back();
        order.push_back(node);

        for (std::size_t next : adjacency[node]) {
            if (next == parent[node]) {
                continue;
            }
            parent[next] = node;
            stack.push_back(next);
        }
    }

    std::vector<std::unique_ptr<std::unordered_set<int>>> colors(n + 1);
    std::vector<std::size_t> answer(n + 1);

    // Duyệt order theo thứ tự đảo: con luôn được xử lý trước cha (small-to-large).
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        std::size_t node = *it;
        // Khởi tạo set của node với màu của chính nó.
        colors[node] = std::make_unique<std::unordered_set<int>>();
        colors[node]->insert(color[node]);

        for (std::size_t next : adjacency[node]) {
            if (parent[next] != node) {
                continue;
            }
            // Luôn gộp set NHỎ vào set LỚN: nếu con lớn hơn thì hoán đổi trước.
            if (colors[node]->size() < colors[next]->size()) {
                colors[node].swap(colors[next]);
            }
            colors[node]->insert(colors[next]->begin(), colors[next]->end());
            colors[next].reset();  // giải phóng set của con đã gộp xong
        }
        // Kích thước set = số màu phân biệt trong subtree của node.
        answer[node] = colors[node]->size();
    }

    for (std::size_t node = 1; node <= n; ++node) {
        if (node > 1) {
            std::cout << ' ';
        }
        std::cout << answer[node];
    }
    std::cout << '\n';
}

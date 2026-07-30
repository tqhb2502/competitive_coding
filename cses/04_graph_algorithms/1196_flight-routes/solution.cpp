#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

struct Edge {
    int destination;
    long long cost;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, k;
    std::cin >> n >> m >> k;

    // Đồ thị có hướng dạng adjacency list: cạnh a -> b với giá vé cost.
    std::vector<std::vector<Edge>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long cost;
        std::cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
    }

    // Dijkstra mở rộng cho K shortest paths.
    // State = (khoảng cách, đỉnh); min-heap luôn lấy khoảng cách nhỏ nhất trước.
    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;
    // finalized_count[u] = số lần đỉnh u đã được pop và finalize (tối đa k lần).
    std::vector<int> finalized_count(n + 1, 0);
    std::vector<long long> answers;
    answers.reserve(k);
    queue.emplace(0, 1);

    // Dừng khi đã thu đủ k đường đi rẻ nhất tới đỉnh n.
    while (!queue.empty() && static_cast<int>(answers.size()) < k) {
        const auto [distance, node] = queue.top();
        queue.pop();
        // Đỉnh này đã có đủ k khoảng cách tốt nhất, không cần xử lý thêm.
        if (finalized_count[node] >= k) {
            continue;
        }

        // Lần pop hợp lệ thứ i của node cho đường đi ngắn thứ i tới node.
        ++finalized_count[node];
        if (node == n) {
            answers.push_back(distance);
        }

        // Mở rộng sang các đỉnh kề chưa đủ k lần finalize.
        for (const Edge& edge : graph[node]) {
            if (finalized_count[edge.destination] < k) {
                queue.emplace(distance + edge.cost, edge.destination);
            }
        }
    }

    // In k tổng giá theo thứ tự không giảm (heap đã bảo đảm thứ tự này).
    for (int i = 0; i < static_cast<int>(answers.size()); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << answers[i];
    }
    std::cout << '\n';
    return 0;
}

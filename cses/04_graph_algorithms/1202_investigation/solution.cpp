#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
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

    int n, m;
    std::cin >> n >> m;

    // Dựng danh sách kề của đồ thị có hướng, có trọng số dương.
    std::vector<std::vector<Edge>> graph(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b;
        long long cost;
        std::cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
    }

    constexpr int MOD = 1'000'000'007;
    constexpr long long INF = std::numeric_limits<long long>::max() / 4;

    // 4 mảng trạng thái DP đi kèm Dijkstra cho mỗi đỉnh:
    //   distance     : chi phí nhỏ nhất từ đỉnh 1
    //   ways         : số đường đi rẻ nhất (mod 1e9+7)
    //   minimum_edges: số cạnh ít nhất trên một đường rẻ nhất
    //   maximum_edges: số cạnh nhiều nhất trên một đường rẻ nhất
    std::vector<long long> distance(n + 1, INF);
    std::vector<int> ways(n + 1, 0);
    std::vector<int> minimum_edges(n + 1, 0);
    std::vector<int> maximum_edges(n + 1, 0);

    // Khởi tạo đỉnh xuất phát 1 và min-heap của Dijkstra.
    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;
    distance[1] = 0;
    ways[1] = 1;
    queue.emplace(0, 1);

    while (!queue.empty()) {
        const auto [current_distance, node] = queue.top();
        queue.pop();
        // Bỏ qua bản ghi cũ; chỉ xử lý khi đỉnh đã được cố định.
        if (current_distance != distance[node]) {
            continue;
        }

        // Relax mọi cạnh (node -> destination) và gộp DP theo thứ tự topo của DAG.
        for (const Edge& edge : graph[node]) {
            const long long candidate = current_distance + edge.cost;
            const int candidate_minimum = minimum_edges[node] + 1;
            const int candidate_maximum = maximum_edges[node] + 1;

            if (candidate < distance[edge.destination]) {
                // Tìm được đường rẻ hơn: đặt lại toàn bộ trạng thái của đỉnh đích.
                distance[edge.destination] = candidate;
                ways[edge.destination] = ways[node];
                minimum_edges[edge.destination] = candidate_minimum;
                maximum_edges[edge.destination] = candidate_maximum;
                queue.emplace(candidate, edge.destination);
            } else if (candidate == distance[edge.destination]) {
                // Thêm một đường cùng chi phí nhỏ nhất: cộng dồn số đường,
                // cập nhật số cạnh ít nhất và nhiều nhất.
                ways[edge.destination] += ways[node];
                if (ways[edge.destination] >= MOD) {
                    ways[edge.destination] -= MOD;
                }
                minimum_edges[edge.destination] =
                    std::min(minimum_edges[edge.destination], candidate_minimum);
                maximum_edges[edge.destination] =
                    std::max(maximum_edges[edge.destination], candidate_maximum);
            }
        }
    }

    // Kết quả cho đỉnh n: chi phí, số đường, số cạnh ít nhất, số cạnh nhiều nhất.
    std::cout << distance[n] << ' ' << ways[n] << ' ' << minimum_edges[n] << ' '
              << maximum_edges[n] << '\n';
    return 0;
}

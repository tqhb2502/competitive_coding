#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
    int source;
    int destination;
    long long weight;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<Edge> edges(m);
    for (Edge& edge : edges) {
        std::cin >> edge.source >> edge.destination >> edge.weight;
    }

    // Khởi tạo dist = 0 cho MỌI đỉnh: tương đương thêm một đỉnh nguồn ảo nối
    // tới tất cả các đỉnh bằng cạnh trọng số 0, nhờ vậy phát hiện được chu
    // trình âm ở bất kỳ đâu, kể cả các thành phần không đến được từ đỉnh 1.
    std::vector<long long> distance(n + 1, 0);
    std::vector<int> parent(n + 1, -1);
    int changed_vertex = -1;

    // Bellman-Ford: lặp đúng n vòng, mỗi vòng relax toàn bộ m cạnh.
    for (int iteration = 0; iteration < n; ++iteration) {
        changed_vertex = -1;
        for (const Edge& edge : edges) {
            const long long candidate = distance[edge.source] + edge.weight;
            if (candidate < distance[edge.destination]) {
                distance[edge.destination] = candidate;
                parent[edge.destination] = edge.source;
                changed_vertex = edge.destination;  // ghi lại đỉnh vừa relax
            }
        }
        // Không còn relax được cạnh nào -> đã hội tụ -> không có chu trình âm.
        if (changed_vertex == -1) {
            break;
        }
    }

    // Còn relax được ở vòng thứ n nghĩa là chắc chắn có chu trình âm.
    if (changed_vertex == -1) {
        std::cout << "NO\n";
        return 0;
    }

    // Đỉnh vừa relax có thể chỉ đến được từ chu trình; đi ngược parent đúng n
    // bước để chắc chắn rơi vào một đỉnh nằm bên trong chu trình âm.
    int cycle_vertex = changed_vertex;
    for (int i = 0; i < n; ++i) {
        cycle_vertex = parent[cycle_vertex];
    }

    // Đi theo chuỗi parent từ đỉnh này cho tới khi quay lại nó để thu các đỉnh
    // của chu trình, rồi đảo lại vì parent biểu diễn hướng ngược.
    std::vector<int> cycle;
    int current = cycle_vertex;
    do {
        cycle.push_back(current);
        current = parent[current];
    } while (current != cycle_vertex);
    cycle.push_back(cycle_vertex);
    std::reverse(cycle.begin(), cycle.end());

    std::cout << "YES\n";
    for (int vertex : cycle) {
        std::cout << vertex << ' ';
    }
    std::cout << '\n';
    return 0;
}

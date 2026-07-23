#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

struct Edge {
    int to;
    long long weight;
};

using Graph = vector<vector<Edge>>;

void addDirectedEdge(Graph& graph, int from, int to, long long weight = 1) {
    graph[from].push_back({to, weight});
}

void addUndirectedEdge(Graph& graph, int first, int second,
                       long long weight = 1) {
    addDirectedEdge(graph, first, second, weight);
    addDirectedEdge(graph, second, first, weight);
}

// Dijkstra yêu cầu mọi trọng số cạnh không âm.
vector<long long> dijkstra(const Graph& graph, int source) {
    const long long infinity = numeric_limits<long long>::max() / 4;
    vector<long long> distance(graph.size(), infinity);

    using QueueEntry = pair<long long, int>;
    priority_queue<QueueEntry, vector<QueueEntry>, greater<QueueEntry>> queue;
    distance[source] = 0;
    queue.push({0, source});

    while (!queue.empty()) {
        const auto [currentDistance, vertex] = queue.top();
        queue.pop();
        if (currentDistance != distance[vertex]) {
            continue;
        }

        for (const Edge& edge : graph[vertex]) {
            const long long candidate = currentDistance + edge.weight;
            if (candidate < distance[edge.to]) {
                distance[edge.to] = candidate;
                queue.push({candidate, edge.to});
            }
        }
    }
    return distance;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Khởi tạo Graph và bổ sung phần đọc input theo yêu cầu của bài.
    return 0;
}

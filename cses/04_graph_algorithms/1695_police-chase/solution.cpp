#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

// Dinic cho đồ thị vô hướng: mỗi cạnh gốc thành hai cung ngược chiều capacity 1,
// hai cung là reverse edge của nhau (chỉ số liên tiếp nên edge ^ 1 là cung ngược).
class UndirectedDinic {
public:
    explicit UndirectedDinic(int vertexCount)
        : graph(vertexCount + 1), level(vertexCount + 1), nextEdge(vertexCount + 1) {}

    // Thêm một con đường vô hướng: hai cung a->b và b->a, mỗi cung capacity 1.
    void addEdge(int first, int second) {
        graph[first].push_back(static_cast<int>(destination.size()));
        destination.push_back(second);
        capacity.push_back(1);
        graph[second].push_back(static_cast<int>(destination.size()));
        destination.push_back(first);
        capacity.push_back(1);
    }

    // Tính max-flow từ source đến sink bằng Dinic; giá trị này bằng min cut.
    long long maximumFlow(int source, int sink) {
        long long result = 0;
        while (buildLevels(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);
            while (const long long pushed =
                       sendFlow(source, sink, numeric_limits<long long>::max() / 4)) {
                result += pushed;
            }
        }
        return result;
    }

    // BFS trên residual graph (chỉ theo cung còn cap > 0): tập S của min cut.
    vector<char> reachableFrom(int source) const {
        vector<char> visited(graph.size(), false);
        queue<int> pending;
        visited[source] = true;
        pending.push(source);
        while (!pending.empty()) {
            const int vertex = pending.front();
            pending.pop();
            for (const int edge : graph[vertex]) {
                const int next = destination[edge];
                if (capacity[edge] > 0 && !visited[next]) {
                    visited[next] = true;
                    pending.push(next);
                }
            }
        }
        return visited;
    }

private:
    vector<vector<int>> graph;
    vector<int> destination;
    vector<long long> capacity;
    vector<int> level;
    vector<size_t> nextEdge;

    // BFS phân tầng: gán level theo khoảng cách từ source trong residual graph.
    bool buildLevels(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> pending;
        level[source] = 0;
        pending.push(source);
        while (!pending.empty()) {
            const int vertex = pending.front();
            pending.pop();
            for (const int edge : graph[vertex]) {
                const int next = destination[edge];
                if (capacity[edge] > 0 && level[next] == -1) {
                    level[next] = level[vertex] + 1;
                    pending.push(next);
                }
            }
        }
        return level[sink] != -1;
    }

    // DFS đẩy luồng dọc theo blocking flow trên đồ thị phân tầng.
    long long sendFlow(int vertex, int sink, long long pushed) {
        if (vertex == sink || pushed == 0) {
            return pushed;
        }
        for (size_t& index = nextEdge[vertex]; index < graph[vertex].size(); ++index) {
            const int edge = graph[vertex][index];
            const int next = destination[edge];
            if (capacity[edge] == 0 || level[next] != level[vertex] + 1) {
                continue;
            }
            const long long sent = sendFlow(next, sink, min(pushed, capacity[edge]));
            if (sent > 0) {
                // Cập nhật residual: giảm cung xuôi, tăng cung ngược (edge ^ 1).
                capacity[edge] -= sent;
                capacity[edge ^ 1] += sent;
                return sent;
            }
        }
        return 0;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    UndirectedDinic network(n);
    // Đọc và lưu các con đường vô hướng, đồng thời dựng mạng luồng capacity 1.
    vector<pair<int, int>> streets(m);
    for (auto& [first, second] : streets) {
        cin >> first >> second;
        network.addEdge(first, second);
    }

    // Max-flow từ ngân hàng (1) đến bến cảng (n) = số con đường tối thiểu phải đóng.
    network.maximumFlow(1, n);
    const vector<char> reachable = network.reachableFrom(1);
    // Con đường cần đóng là cạnh có đúng một đầu thuộc S (nằm trên lát cắt).
    vector<pair<int, int>> cut;
    for (const auto& [first, second] : streets) {
        if (reachable[first] != reachable[second]) {
            cut.emplace_back(first, second);
        }
    }

    cout << cut.size() << '\n';
    for (const auto& [first, second] : cut) {
        cout << first << ' ' << second << '\n';
    }
    return 0;
}

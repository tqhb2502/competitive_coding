#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

// Thuật toán Dinic tìm luồng cực đại (max-flow) trên đồ thị có hướng.
class Dinic {
public:
    explicit Dinic(int vertexCount)
        : graph(vertexCount + 1), level(vertexCount + 1), nextEdge(vertexCount + 1) {}

    // Thêm cạnh có hướng from -> to; cạnh nghịch (residual) capacity 0 đứng liền kề,
    // nên cạnh đối của cạnh chỉ số e chính là e ^ 1.
    void addDirectedEdge(int from, int to, long long edgeCapacity) {
        graph[from].push_back(static_cast<int>(destination.size()));
        destination.push_back(to);
        capacity.push_back(edgeCapacity);
        graph[to].push_back(static_cast<int>(destination.size()));
        destination.push_back(from);
        capacity.push_back(0);
    }

    long long maximumFlow(int source, int sink) {
        // Trường hợp suy biến: source trùng sink thì luồng bằng 0.
        if (source == sink) {
            return 0;
        }
        long long result = 0;
        // Lặp: dựng level graph bằng BFS, rồi đẩy blocking flow bằng DFS.
        while (buildLevels(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);
            while (const long long pushed =
                       sendFlow(source, sink, numeric_limits<long long>::max() / 4)) {
                result += pushed;
            }
        }
        return result;
    }

private:
    vector<vector<int>> graph;        // danh sách kề: graph[u] chứa các chỉ số cạnh
    vector<int> destination;          // destination[e] = đỉnh đích của cạnh e
    vector<long long> capacity;       // capacity[e] = dung lượng còn lại (residual)
    vector<int> level;                // level[u] = độ sâu của u trong level graph
    vector<size_t> nextEdge;          // iterator bỏ qua các cạnh đã bão hòa khi DFS

    // BFS gán level cho từng đỉnh; trả về true nếu vẫn tới được sink.
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
                // Chỉ đi theo cạnh còn dung lượng và tới đỉnh chưa được gán level.
                if (capacity[edge] > 0 && level[next] == -1) {
                    level[next] = level[vertex] + 1;
                    pending.push(next);
                }
            }
        }
        return level[sink] != -1;
    }

    // DFS đẩy luồng dọc theo level graph, tìm một augmenting path và trả về lượng đẩy.
    long long sendFlow(int vertex, int sink, long long pushed) {
        if (vertex == sink || pushed == 0) {
            return pushed;
        }
        // nextEdge[vertex] ghi nhớ vị trí đang xét để không lặp lại cạnh bão hòa.
        for (size_t& index = nextEdge[vertex]; index < graph[vertex].size(); ++index) {
            const int edge = graph[vertex][index];
            const int next = destination[edge];
            // Chỉ đi theo cạnh còn dung lượng và tiến đúng một mức trong level graph.
            if (capacity[edge] == 0 || level[next] != level[vertex] + 1) {
                continue;
            }
            const long long sent = sendFlow(next, sink, min(pushed, capacity[edge]));
            if (sent > 0) {
                // Cập nhật residual: giảm cạnh thuận, tăng cạnh nghịch (edge ^ 1).
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
    Dinic network(n);
    // Đọc m kết nối có hướng và thêm vào mạng luồng.
    for (int edge = 0; edge < m; ++edge) {
        int from, to;
        long long capacity;
        cin >> from >> to >> capacity;
        network.addDirectedEdge(from, to, capacity);
    }
    // Tốc độ tải tối đa = luồng cực đại từ máy 1 tới máy n.
    cout << network.maximumFlow(1, n) << '\n';
    return 0;
}

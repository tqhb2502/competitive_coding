#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

class Dinic {
public:
    explicit Dinic(int vertexCount)
        : graph(vertexCount + 1), level(vertexCount + 1), nextEdge(vertexCount + 1) {}

    void addDirectedEdge(int from, int to, long long edgeCapacity) {
        graph[from].push_back(static_cast<int>(destination.size()));
        destination.push_back(to);
        capacity.push_back(edgeCapacity);
        graph[to].push_back(static_cast<int>(destination.size()));
        destination.push_back(from);
        capacity.push_back(0);
    }

    long long maximumFlow(int source, int sink) {
        if (source == sink) {
            return 0;
        }
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

private:
    vector<vector<int>> graph;
    vector<int> destination;
    vector<long long> capacity;
    vector<int> level;
    vector<size_t> nextEdge;

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
    for (int edge = 0; edge < m; ++edge) {
        int from, to;
        long long capacity;
        cin >> from >> to >> capacity;
        network.addDirectedEdge(from, to, capacity);
    }
    cout << network.maximumFlow(1, n) << '\n';
    return 0;
}

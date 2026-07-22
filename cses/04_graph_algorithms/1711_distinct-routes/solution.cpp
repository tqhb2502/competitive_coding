#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

class UnitDinic {
public:
    explicit UnitDinic(int vertexCount)
        : graph(vertexCount + 1), level(vertexCount + 1), nextEdge(vertexCount + 1) {}

    int addEdge(int from, int to) {
        const int forward = static_cast<int>(destination.size());
        graph[from].push_back(forward);
        destination.push_back(to);
        capacity.push_back(1);
        graph[to].push_back(forward + 1);
        destination.push_back(from);
        capacity.push_back(0);
        return forward;
    }

    int maximumFlow(int source, int sink) {
        int result = 0;
        while (buildLevels(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);
            while (sendFlow(source, sink)) {
                ++result;
            }
        }
        return result;
    }

    vector<vector<int>> graph;
    vector<int> destination;
    vector<int> capacity;

private:
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

    bool sendFlow(int vertex, int sink) {
        if (vertex == sink) {
            return true;
        }
        for (size_t& index = nextEdge[vertex]; index < graph[vertex].size(); ++index) {
            const int edge = graph[vertex][index];
            const int next = destination[edge];
            if (capacity[edge] == 0 || level[next] != level[vertex] + 1) {
                continue;
            }
            if (sendFlow(next, sink)) {
                --capacity[edge];
                ++capacity[edge ^ 1];
                return true;
            }
        }
        return false;
    }
};

struct OriginalEdge {
    int from;
    int to;
    int id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    UnitDinic network(n);
    vector<OriginalEdge> originalEdges;
    originalEdges.reserve(m);
    for (int edge = 0; edge < m; ++edge) {
        int from, to;
        cin >> from >> to;
        originalEdges.push_back({from, to, network.addEdge(from, to)});
    }

    const int routeCount = network.maximumFlow(1, n);
    vector<vector<int>> flowEdges(n + 1);
    for (const OriginalEdge& edge : originalEdges) {
        if (network.capacity[edge.id] == 0) {
            flowEdges[edge.from].push_back(edge.id);
        }
    }

    vector<char> consumed(network.destination.size(), false);
    vector<vector<int>> routes;
    routes.reserve(routeCount);

    for (int routeIndex = 0; routeIndex < routeCount; ++routeIndex) {
        vector<int> parentVertex(n + 1, -1);
        vector<int> parentEdge(n + 1, -1);
        queue<int> pending;
        parentVertex[1] = 1;
        pending.push(1);

        while (!pending.empty() && parentVertex[n] == -1) {
            const int vertex = pending.front();
            pending.pop();
            for (const int edge : flowEdges[vertex]) {
                if (consumed[edge]) {
                    continue;
                }
                const int next = network.destination[edge];
                if (parentVertex[next] == -1) {
                    parentVertex[next] = vertex;
                    parentEdge[next] = edge;
                    pending.push(next);
                }
            }
        }

        vector<int> route;
        int vertex = n;
        while (vertex != 1) {
            route.push_back(vertex);
            const int edge = parentEdge[vertex];
            consumed[edge] = true;
            vertex = parentVertex[vertex];
        }
        route.push_back(1);
        reverse(route.begin(), route.end());
        routes.push_back(std::move(route));
    }

    cout << routeCount << '\n';
    for (const vector<int>& route : routes) {
        cout << route.size() << '\n';
        for (size_t i = 0; i < route.size(); ++i) {
            if (i > 0) {
                cout << ' ';
            }
            cout << route[i];
        }
        cout << '\n';
    }
    return 0;
}

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Edge {
    int to;
    int reverseIndex;
    int capacity;
};

class Dinic {
public:
    explicit Dinic(int vertexCount)
        : graph(vertexCount), level(vertexCount), nextEdge(vertexCount) {}

    int addEdge(int from, int to, int capacity) {
        const int index = static_cast<int>(graph[from].size());
        const int reverseIndex = static_cast<int>(graph[to].size());
        graph[from].push_back({to, reverseIndex, capacity});
        graph[to].push_back({from, index, 0});
        return index;
    }

    int maximumFlow(int source, int sink) {
        int result = 0;
        while (buildLevels(source, sink)) {
            fill(nextEdge.begin(), nextEdge.end(), 0);
            while (const int pushed =
                       sendFlow(source, sink, numeric_limits<int>::max())) {
                result += pushed;
            }
        }
        return result;
    }

    const Edge& getEdge(int vertex, int index) const {
        return graph[vertex][index];
    }

private:
    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> nextEdge;

    bool buildLevels(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> searchQueue;
        level[source] = 0;
        searchQueue.push(source);
        while (!searchQueue.empty()) {
            const int vertex = searchQueue.front();
            searchQueue.pop();
            for (const Edge& edge : graph[vertex]) {
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[vertex] + 1;
                    searchQueue.push(edge.to);
                }
            }
        }
        return level[sink] != -1;
    }

    int sendFlow(int vertex, int sink, int available) {
        if (vertex == sink) {
            return available;
        }
        for (int& index = nextEdge[vertex];
             index < static_cast<int>(graph[vertex].size()); ++index) {
            Edge& edge = graph[vertex][index];
            if (edge.capacity == 0 || level[edge.to] != level[vertex] + 1) {
                continue;
            }
            const int pushed =
                sendFlow(edge.to, sink, min(available, edge.capacity));
            if (pushed != 0) {
                edge.capacity -= pushed;
                graph[edge.to][edge.reverseIndex].capacity += pushed;
                return pushed;
            }
        }
        return 0;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> rowDemand(n);
    vector<int> columnDemand(n);
    int rowSum = 0;
    int columnSum = 0;
    for (int& value : rowDemand) {
        cin >> value;
        rowSum += value;
    }
    for (int& value : columnDemand) {
        cin >> value;
        columnSum += value;
    }
    if (rowSum != columnSum) {
        cout << -1 << '\n';
        return 0;
    }

    const int source = 2 * n;
    const int sink = source + 1;
    Dinic flowNetwork(sink + 1);
    for (int row = 0; row < n; ++row) {
        flowNetwork.addEdge(source, row, rowDemand[row]);
    }
    for (int column = 0; column < n; ++column) {
        flowNetwork.addEdge(n + column, sink, columnDemand[column]);
    }
    vector<vector<int>> cellEdge(n, vector<int>(n));
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            cellEdge[row][column] =
                flowNetwork.addEdge(row, n + column, 1);
        }
    }

    if (flowNetwork.maximumFlow(source, sink) != rowSum) {
        cout << -1 << '\n';
        return 0;
    }
    for (int row = 0; row < n; ++row) {
        string answerRow(n, '.');
        for (int column = 0; column < n; ++column) {
            if (flowNetwork.getEdge(row, cellEdge[row][column]).capacity == 0) {
                answerRow[column] = 'X';
            }
        }
        cout << answerRow << '\n';
    }
}

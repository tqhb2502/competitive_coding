#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct CostEdge {
    int to;
    int reverseIndex;
    int capacity;
    int cost;
};

class MinCostFlow {
public:
    explicit MinCostFlow(int vertexCount) : graph(vertexCount) {}

    int addEdge(int from, int to, int capacity, int cost) {
        const int index = static_cast<int>(graph[from].size());
        const int reverseIndex = static_cast<int>(graph[to].size());
        graph[from].push_back({to, reverseIndex, capacity, cost});
        graph[to].push_back({from, index, 0, -cost});
        return index;
    }

    pair<int, long long> run(int source, int sink, int requestedFlow) {
        const int vertexCount = static_cast<int>(graph.size());
        const long long infinity = numeric_limits<long long>::max() / 4;
        vector<long long> potential(vertexCount, 0);
        vector<long long> distance(vertexCount);
        vector<int> parentVertex(vertexCount);
        vector<int> parentEdge(vertexCount);
        int totalFlow = 0;
        long long totalCost = 0;

        while (totalFlow < requestedFlow) {
            fill(distance.begin(), distance.end(), infinity);
            distance[source] = 0;
            using QueueEntry = pair<long long, int>;
            priority_queue<QueueEntry, vector<QueueEntry>,
                           greater<QueueEntry>> queue;
            queue.push({0, source});
            while (!queue.empty()) {
                const auto [currentDistance, vertex] = queue.top();
                queue.pop();
                if (currentDistance != distance[vertex]) {
                    continue;
                }
                for (int index = 0;
                     index < static_cast<int>(graph[vertex].size()); ++index) {
                    const CostEdge& edge = graph[vertex][index];
                    if (edge.capacity == 0) {
                        continue;
                    }
                    const long long newDistance =
                        currentDistance + edge.cost + potential[vertex] -
                        potential[edge.to];
                    if (newDistance < distance[edge.to]) {
                        distance[edge.to] = newDistance;
                        parentVertex[edge.to] = vertex;
                        parentEdge[edge.to] = index;
                        queue.push({newDistance, edge.to});
                    }
                }
            }
            if (distance[sink] == infinity) {
                break;
            }
            for (int vertex = 0; vertex < vertexCount; ++vertex) {
                if (distance[vertex] != infinity) {
                    potential[vertex] += distance[vertex];
                }
            }

            int pushed = requestedFlow - totalFlow;
            for (int vertex = sink; vertex != source;
                 vertex = parentVertex[vertex]) {
                pushed = min(pushed,
                             graph[parentVertex[vertex]][parentEdge[vertex]]
                                 .capacity);
            }
            for (int vertex = sink; vertex != source;
                 vertex = parentVertex[vertex]) {
                CostEdge& edge =
                    graph[parentVertex[vertex]][parentEdge[vertex]];
                totalCost += static_cast<long long>(pushed) * edge.cost;
                edge.capacity -= pushed;
                graph[edge.to][edge.reverseIndex].capacity += pushed;
            }
            totalFlow += pushed;
        }
        return {totalFlow, totalCost};
    }

    const CostEdge& getEdge(int vertex, int index) const {
        return graph[vertex][index];
    }

private:
    vector<vector<CostEdge>> graph;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> rowDemand(n);
    vector<int> columnDemand(n);
    int requiredFlow = 0;
    int columnSum = 0;
    for (int& value : rowDemand) {
        cin >> value;
        requiredFlow += value;
    }
    for (int& value : columnDemand) {
        cin >> value;
        columnSum += value;
    }
    vector<vector<int>> coins(n, vector<int>(n));
    for (auto& row : coins) {
        for (int& value : row) {
            cin >> value;
        }
    }
    if (requiredFlow != columnSum) {
        cout << -1 << '\n';
        return 0;
    }

    const int source = 2 * n;
    const int sink = source + 1;
    MinCostFlow flowNetwork(sink + 1);
    for (int row = 0; row < n; ++row) {
        flowNetwork.addEdge(source, row, rowDemand[row], 0);
    }
    for (int column = 0; column < n; ++column) {
        flowNetwork.addEdge(n + column, sink, columnDemand[column], 0);
    }
    vector<vector<int>> cellEdge(n, vector<int>(n));
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            cellEdge[row][column] = flowNetwork.addEdge(
                row, n + column, 1, 1000 - coins[row][column]);
        }
    }

    const auto [flow, minimumCost] =
        flowNetwork.run(source, sink, requiredFlow);
    if (flow != requiredFlow) {
        cout << -1 << '\n';
        return 0;
    }
    const long long answer =
        1000LL * static_cast<long long>(requiredFlow) - minimumCost;
    cout << answer << '\n';
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

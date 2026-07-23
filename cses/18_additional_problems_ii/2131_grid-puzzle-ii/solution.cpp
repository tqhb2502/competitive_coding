#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Cạnh có chi phí trong mạng luồng, kèm chỉ số cạnh ngược để cập nhật luồng.
struct CostEdge {
    int to;
    int reverseIndex;
    int capacity;
    int cost;
};

// Min-cost max-flow bằng đường tăng ngắn nhất với thế đỉnh (Dijkstra + reduced cost).
class MinCostFlow {
public:
    explicit MinCostFlow(int vertexCount) : graph(vertexCount) {}

    // Thêm cạnh thuận (capacity, cost) cùng cạnh ngược (0, -cost); trả về chỉ số cạnh.
    int addEdge(int from, int to, int capacity, int cost) {
        const int index = static_cast<int>(graph[from].size());
        const int reverseIndex = static_cast<int>(graph[to].size());
        graph[from].push_back({to, reverseIndex, capacity, cost});
        graph[to].push_back({from, index, 0, -cost});
        return index;
    }

    // Đẩy tối đa requestedFlow đơn vị luồng; trả về (luồng thực đẩy, tổng chi phí).
    pair<int, long long> run(int source, int sink, int requestedFlow) {
        const int vertexCount = static_cast<int>(graph.size());
        const long long infinity = numeric_limits<long long>::max() / 4;
        vector<long long> potential(vertexCount, 0);  // thế đỉnh để giữ chi phí rút gọn không âm
        vector<long long> distance(vertexCount);
        vector<int> parentVertex(vertexCount);
        vector<int> parentEdge(vertexCount);
        int totalFlow = 0;
        long long totalCost = 0;

        while (totalFlow < requestedFlow) {
            // Dijkstra trên chi phí rút gọn tìm đường tăng chi phí nhỏ nhất.
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
                    continue;  // bỏ qua bản ghi cũ đã lỗi thời
                }
                for (int index = 0;
                     index < static_cast<int>(graph[vertex].size()); ++index) {
                    const CostEdge& edge = graph[vertex][index];
                    if (edge.capacity == 0) {
                        continue;  // cạnh đã bão hòa
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
                break;  // không còn đường tăng, đích không tới được
            }
            // Cập nhật thế đỉnh theo khoảng cách vừa tìm được.
            for (int vertex = 0; vertex < vertexCount; ++vertex) {
                if (distance[vertex] != infinity) {
                    potential[vertex] += distance[vertex];
                }
            }

            // Tìm sức chứa nhỏ nhất dọc đường tăng (lượng luồng có thể đẩy).
            int pushed = requestedFlow - totalFlow;
            for (int vertex = sink; vertex != source;
                 vertex = parentVertex[vertex]) {
                pushed = min(pushed,
                             graph[parentVertex[vertex]][parentEdge[vertex]]
                                 .capacity);
            }
            // Đẩy luồng dọc đường và cộng dồn chi phí.
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

    // Truy xuất cạnh để đọc lại sức chứa còn lại sau khi chạy luồng.
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
    // rowDemand[i] = số ô phải chọn ở hàng i; columnDemand[j] = số ô phải chọn ở cột j.
    vector<int> rowDemand(n);
    vector<int> columnDemand(n);
    int requiredFlow = 0;
    int columnSum = 0;
    for (int& value : rowDemand) {
        cin >> value;
        requiredFlow += value;  // tổng ô cần chọn theo hàng
    }
    for (int& value : columnDemand) {
        cin >> value;
        columnSum += value;  // tổng ô cần chọn theo cột
    }
    vector<vector<int>> coins(n, vector<int>(n));
    for (auto& row : coins) {
        for (int& value : row) {
            cin >> value;
        }
    }
    // Điều kiện cần: tổng theo hàng phải bằng tổng theo cột, nếu không thì vô nghiệm.
    if (requiredFlow != columnSum) {
        cout << -1 << '\n';
        return 0;
    }

    // Dựng mạng: nguồn -> hàng -> cột -> đích.
    const int source = 2 * n;
    const int sink = source + 1;
    MinCostFlow flowNetwork(sink + 1);
    for (int row = 0; row < n; ++row) {
        // nguồn -> hàng i: sức chứa a_i, chi phí 0.
        flowNetwork.addEdge(source, row, rowDemand[row], 0);
    }
    for (int column = 0; column < n; ++column) {
        // cột j -> đích: sức chứa b_j, chi phí 0.
        flowNetwork.addEdge(n + column, sink, columnDemand[column], 0);
    }
    vector<vector<int>> cellEdge(n, vector<int>(n));
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            // hàng i -> cột j: sức chứa 1, chi phí 1000 - c_ij (nhỏ nhất khi xu nhiều nhất).
            cellEdge[row][column] = flowNetwork.addEdge(
                row, n + column, 1, 1000 - coins[row][column]);
        }
    }

    const auto [flow, minimumCost] =
        flowNetwork.run(source, sink, requiredFlow);
    // Không đẩy đủ luồng nghĩa là không tồn tại bảng thỏa mọi ràng buộc.
    if (flow != requiredFlow) {
        cout << -1 << '\n';
        return 0;
    }
    // Đổi ngược từ chi phí nhỏ nhất về tổng xu lớn nhất.
    const long long answer =
        1000LL * static_cast<long long>(requiredFlow) - minimumCost;
    cout << answer << '\n';
    // Ô có cạnh đã bão hòa (sức chứa còn lại 0) là ô được chọn -> đánh dấu 'X'.
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

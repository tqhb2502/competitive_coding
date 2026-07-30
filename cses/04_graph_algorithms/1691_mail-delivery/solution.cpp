#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Một phần tử trong adjacency list: đỉnh kề và id của cạnh (dùng chung cho cả hai chiều).
struct AdjacentEdge {
    int destination;
    int id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // Đọc đồ thị vô hướng: mỗi cạnh id được thêm vào adjacency của cả hai đầu mút.
    vector<vector<AdjacentEdge>> graph(n + 1);
    vector<int> degree(n + 1, 0);
    for (int id = 0; id < m; ++id) {
        int first, second;
        cin >> first >> second;
        graph[first].push_back({second, id});
        graph[second].push_back({first, id});
        ++degree[first];
        ++degree[second];
    }

    // Điều kiện cần của chu trình Euler: mọi đỉnh phải có bậc chẵn.
    for (int vertex = 1; vertex <= n; ++vertex) {
        if (degree[vertex] % 2 != 0) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    vector<char> used(m, false);        // đánh dấu cạnh đã đi qua
    vector<size_t> nextEdge(n + 1, 0);  // con trỏ duyệt adjacency của mỗi đỉnh
    vector<int> stack{1};               // Hierholzer iterative, bắt đầu từ đỉnh 1
    vector<int> circuit;
    circuit.reserve(m + 1);

    // Thuật toán Hierholzer: đi theo cạnh chưa dùng cho tới khi kẹt, rồi lùi (backtrack)
    // và ghi đỉnh vào circuit.
    while (!stack.empty()) {
        const int vertex = stack.back();
        // Bỏ qua các cạnh đã đi qua nhờ con trỏ nextEdge (mỗi cạnh chỉ bị bỏ qua O(1) lần).
        while (nextEdge[vertex] < graph[vertex].size() &&
               used[graph[vertex][nextEdge[vertex]].id]) {
            ++nextEdge[vertex];
        }

        if (nextEdge[vertex] == graph[vertex].size()) {
            // Không còn cạnh nào: chốt đỉnh này vào circuit và lùi lại.
            circuit.push_back(vertex);
            stack.pop_back();
        } else {
            // Đi tiếp qua một cạnh chưa dùng, đánh dấu cạnh và tiến sang đỉnh kề.
            const AdjacentEdge edge = graph[vertex][nextEdge[vertex]++];
            used[edge.id] = true;
            stack.push_back(edge.destination);
        }
    }

    // Chu trình hợp lệ phải đi qua tất cả m cạnh -> có đúng m + 1 đỉnh.
    // Nếu thiếu nghĩa là đồ thị không liên thông (còn cạnh ở thành phần khác).
    if (static_cast<int>(circuit.size()) != m + 1) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    // circuit đang theo thứ tự ngược, đảo lại để có lộ trình đi đúng chiều.
    reverse(circuit.begin(), circuit.end());
    for (size_t i = 0; i < circuit.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << circuit[i];
    }
    cout << '\n';
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc đồ thị có hướng: n đỉnh, m cạnh; đồng thời đếm bậc vào/ra mỗi đỉnh.
    int n, m;
    cin >> n >> m;
    vector<vector<int>> graph(n + 1);
    vector<int> inDegree(n + 1, 0);
    vector<int> outDegree(n + 1, 0);
    for (int edge = 0; edge < m; ++edge) {
        int from, to;
        cin >> from >> to;
        graph[from].push_back(to);
        ++outDegree[from];
        ++inDegree[to];
    }

    // Điều kiện bậc cho đường đi Euler có hướng từ đỉnh 1 tới đỉnh n:
    //   đỉnh 1 dư đúng một cạnh ra (+1), đỉnh n dư đúng một cạnh vào (-1),
    //   mọi đỉnh khác phải cân bằng bậc vào/ra (0). Vi phạm => IMPOSSIBLE.
    for (int vertex = 1; vertex <= n; ++vertex) {
        const int difference = outDegree[vertex] - inDegree[vertex];
        const int required = vertex == 1 ? 1 : (vertex == n ? -1 : 0);
        if (difference != required) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    // Hierholzer kiểu iterative xuất phát từ đỉnh 1; nextEdge[v] là con trỏ tới
    // cạnh chưa dùng kế tiếp của v nên mỗi cạnh chỉ được xét đúng một lần.
    vector<size_t> nextEdge(n + 1, 0);
    vector<int> stack{1};
    vector<int> path;
    path.reserve(m + 1);
    while (!stack.empty()) {
        const int vertex = stack.back();
        if (nextEdge[vertex] < graph[vertex].size()) {
            // Còn cạnh chưa dùng: đi theo cạnh đó, đẩy đỉnh kề vào stack.
            stack.push_back(graph[vertex][nextEdge[vertex]++]);
        } else {
            // Hết cạnh: pop đỉnh và thêm vào path (thứ tự ngược).
            path.push_back(vertex);
            stack.pop_back();
        }
    }

    // len(path) phải bằng m + 1 thì mới dùng hết mọi cạnh; đây cũng chính là
    // kiểm tra điều kiện liên thông.
    if (static_cast<int>(path.size()) != m + 1) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    // Đảo ngược để có thứ tự đi thực sự; xác nhận đầu là 1 và cuối là n.
    reverse(path.begin(), path.end());
    if (path.front() != 1 || path.back() != n) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            cout << ' ';
        }
        cout << path[i];
    }
    cout << '\n';
    return 0;
}

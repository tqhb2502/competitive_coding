#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

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

    for (int vertex = 1; vertex <= n; ++vertex) {
        const int difference = outDegree[vertex] - inDegree[vertex];
        const int required = vertex == 1 ? 1 : (vertex == n ? -1 : 0);
        if (difference != required) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    vector<size_t> nextEdge(n + 1, 0);
    vector<int> stack{1};
    vector<int> path;
    path.reserve(m + 1);
    while (!stack.empty()) {
        const int vertex = stack.back();
        if (nextEdge[vertex] < graph[vertex].size()) {
            stack.push_back(graph[vertex][nextEdge[vertex]++]);
        } else {
            path.push_back(vertex);
            stack.pop_back();
        }
    }

    if (static_cast<int>(path.size()) != m + 1) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

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

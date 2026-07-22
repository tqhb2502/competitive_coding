#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct AdjacentEdge {
    int destination;
    int id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
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

    for (int vertex = 1; vertex <= n; ++vertex) {
        if (degree[vertex] % 2 != 0) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    vector<char> used(m, false);
    vector<size_t> nextEdge(n + 1, 0);
    vector<int> stack{1};
    vector<int> circuit;
    circuit.reserve(m + 1);

    while (!stack.empty()) {
        const int vertex = stack.back();
        while (nextEdge[vertex] < graph[vertex].size() &&
               used[graph[vertex][nextEdge[vertex]].id]) {
            ++nextEdge[vertex];
        }

        if (nextEdge[vertex] == graph[vertex].size()) {
            circuit.push_back(vertex);
            stack.pop_back();
        } else {
            const AdjacentEdge edge = graph[vertex][nextEdge[vertex]++];
            used[edge.id] = true;
            stack.push_back(edge.destination);
        }
    }

    if (static_cast<int>(circuit.size()) != m + 1) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

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

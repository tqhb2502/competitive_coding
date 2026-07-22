#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<long long> coins(n);
    for (long long& value : coins) {
        std::cin >> value;
    }

    std::vector<std::vector<int>> adj(n);
    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        adj[a].push_back(b);
        edges.emplace_back(a, b);
    }

    // Iterative Tarjan. A component is numbered when it is popped; therefore
    // every edge between SCCs goes from a larger id to a smaller id.
    std::vector<int> index(n, -1);
    std::vector<int> low(n, 0);
    std::vector<int> component(n, -1);
    std::vector<std::size_t> next_edge(n, 0);
    std::vector<char> on_stack(n, false);
    std::vector<int> scc_stack;
    std::vector<int> dfs_stack;
    scc_stack.reserve(n);
    dfs_stack.reserve(n);

    int timer = 0;
    int component_count = 0;
    for (int start = 0; start < n; ++start) {
        if (index[start] != -1) {
            continue;
        }

        index[start] = low[start] = timer++;
        on_stack[start] = true;
        scc_stack.push_back(start);
        dfs_stack.push_back(start);

        while (!dfs_stack.empty()) {
            const int node = dfs_stack.back();
            if (next_edge[node] < adj[node].size()) {
                const int next = adj[node][next_edge[node]++];
                if (index[next] == -1) {
                    index[next] = low[next] = timer++;
                    on_stack[next] = true;
                    scc_stack.push_back(next);
                    dfs_stack.push_back(next);
                } else if (on_stack[next]) {
                    low[node] = std::min(low[node], index[next]);
                }
                continue;
            }

            dfs_stack.pop_back();
            if (low[node] == index[node]) {
                while (true) {
                    const int member = scc_stack.back();
                    scc_stack.pop_back();
                    on_stack[member] = false;
                    component[member] = component_count;
                    if (member == node) {
                        break;
                    }
                }
                ++component_count;
            }
            if (!dfs_stack.empty()) {
                const int parent = dfs_stack.back();
                low[parent] = std::min(low[parent], low[node]);
            }
        }
    }

    std::vector<long long> component_coins(component_count, 0);
    for (int room = 0; room < n; ++room) {
        component_coins[component[room]] += coins[room];
    }

    std::vector<std::vector<int>> condensed(component_count);
    for (const auto& [from, to] : edges) {
        const int from_component = component[from];
        const int to_component = component[to];
        if (from_component != to_component) {
            condensed[from_component].push_back(to_component);
        }
    }

    // SCC ids are in reverse topological order, so all successors of c have
    // already been processed when c is reached.
    std::vector<long long> best(component_count, 0);
    long long answer = 0;
    for (int c = 0; c < component_count; ++c) {
        long long best_successor = 0;
        for (const int next : condensed[c]) {
            best_successor = std::max(best_successor, best[next]);
        }
        best[c] = component_coins[c] + best_successor;
        answer = std::max(answer, best[c]);
    }

    std::cout << answer << '\n';
    return 0;
}

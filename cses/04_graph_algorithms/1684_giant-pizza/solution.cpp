#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int clause_count, variable_count;
    std::cin >> clause_count >> variable_count;

    const int node_count = 2 * variable_count;
    std::vector<std::vector<int>> adj(node_count);
    for (int i = 0; i < clause_count; ++i) {
        char first_sign, second_sign;
        int first_variable, second_variable;
        std::cin >> first_sign >> first_variable
                 >> second_sign >> second_variable;

        const int first = 2 * (first_variable - 1) +
                          (first_sign == '+' ? 0 : 1);
        const int second = 2 * (second_variable - 1) +
                           (second_sign == '+' ? 0 : 1);

        // (first OR second) is equivalent to these two implications.
        adj[first ^ 1].push_back(second);
        adj[second ^ 1].push_back(first);
    }

    // Iterative Tarjan. Components are numbered as they are popped, so sink
    // SCCs receive smaller numbers (reverse topological order).
    std::vector<int> index(node_count, -1);
    std::vector<int> low(node_count, 0);
    std::vector<int> component(node_count, -1);
    std::vector<std::size_t> next_edge(node_count, 0);
    std::vector<char> on_stack(node_count, false);
    std::vector<int> scc_stack;
    std::vector<int> dfs_stack;
    scc_stack.reserve(node_count);
    dfs_stack.reserve(node_count);

    int timer = 0;
    int component_count = 0;
    for (int start = 0; start < node_count; ++start) {
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

    for (int variable = 0; variable < variable_count; ++variable) {
        const int positive = 2 * variable;
        const int negative = positive + 1;
        if (component[positive] == component[negative]) {
            std::cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    for (int variable = 0; variable < variable_count; ++variable) {
        if (variable != 0) {
            std::cout << ' ';
        }
        const int positive = 2 * variable;
        const int negative = positive + 1;
        std::cout << (component[positive] < component[negative] ? '+' : '-');
    }
    std::cout << '\n';
    return 0;
}

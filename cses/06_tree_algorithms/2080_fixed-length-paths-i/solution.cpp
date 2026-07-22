#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, target_length;
    std::cin >> n >> target_length;

    std::vector<std::vector<int>> graph(n + 1);
    for (int edge = 0; edge < n - 1; ++edge) {
        int first, second;
        std::cin >> first >> second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    std::vector<int> parent(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack{1};
    parent[1] = -1;
    while (!stack.empty()) {
        const int node = stack.back();
        stack.pop_back();
        order.push_back(node);
        for (const int next : graph[node]) {
            if (next == parent[node]) {
                continue;
            }
            parent[next] = node;
            stack.push_back(next);
        }
    }

    // height[node] is the longest downward distance. The child attaining it
    // is inherited without copying its distance-count array.
    std::vector<int> height(n + 1, 0);
    std::vector<int> heavy_child(n + 1, 0);
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int node = *iterator;
        const int ancestor = parent[node];
        if (ancestor > 0 && height[node] + 1 > height[ancestor]) {
            height[ancestor] = height[node] + 1;
            heavy_child[ancestor] = node;
        }
    }

    // Every heavy chain owns one contiguous block. For a heavy edge u->v,
    // pointer[u] + 1 == pointer[v], so cnt[u][d] reuses cnt[v][d-1].
    std::vector<int> pointer(n + 1, 0);
    int pool_size = 0;
    for (const int node : order) {
        if (node != 1 && heavy_child[parent[node]] == node) {
            continue;
        }
        int current = node;
        int offset = pool_size;
        pool_size += height[node] + 1;
        while (current != 0) {
            pointer[current] = offset++;
            current = heavy_child[current];
        }
    }

    std::vector<long long> distance_count(pool_size + 1, 0);
    long long answer = 0;
    const int complement_base = target_length - 1;

    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int node = *iterator;
        const int node_pointer = pointer[node];
        distance_count[node_pointer] = 1;

        for (const int child : graph[node]) {
            if (parent[child] != node || child == heavy_child[node]) {
                continue;
            }

            const int child_pointer = pointer[child];
            for (int child_depth = 0;
                 child_depth <= height[child]; ++child_depth) {
                const int previous_depth = complement_base - child_depth;
                if (previous_depth < 1) {
                    break;
                }
                if (previous_depth <= height[node]) {
                    answer += distance_count[child_pointer + child_depth] *
                              distance_count[node_pointer + previous_depth];
                }
            }

            for (int child_depth = 0;
                 child_depth <= height[child]; ++child_depth) {
                distance_count[node_pointer + child_depth + 1] +=
                    distance_count[child_pointer + child_depth];
            }
        }

        if (target_length <= height[node]) {
            answer += distance_count[node_pointer + target_length];
        }
    }

    std::cout << answer << '\n';
    return 0;
}

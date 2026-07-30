#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> next(n + 1);
    for (int v = 1; v <= n; ++v) {
        std::cin >> next[v];
    }

    // Bước 1: tìm mọi cycle bằng cách duyệt functional graph theo kiểu iterative.
    // state: 0 = chưa thăm, 1 = đang trên đường đi hiện tại, 2 = đã xong.
    std::vector<int> state(n + 1, 0);
    std::vector<int> path_index(n + 1, 0);
    std::vector<char> on_cycle(n + 1, false);

    for (int start = 1; start <= n; ++start) {
        if (state[start] != 0) {
            continue;
        }

        std::vector<int> path;
        int v = start;
        while (state[v] == 0) {
            state[v] = 1;
            path_index[v] = static_cast<int>(path.size());
            path.push_back(v);
            v = next[v];
        }

        // Gặp lại một đỉnh đang trên đường đi hiện tại -> phát hiện cycle mới.
        if (state[v] == 1) {
            for (int i = path_index[v]; i < static_cast<int>(path.size()); ++i) {
                on_cycle[path[i]] = true;
            }
        }
        for (int node : path) {
            state[node] = 2;
        }
    }

    // Bước 2: đánh số các cycle và ghi lại vị trí của từng đỉnh theo thứ tự đi vòng.
    std::vector<int> cycle_id(n + 1, -1);
    std::vector<int> cycle_position(n + 1, 0);
    std::vector<int> cycle_length;

    for (int v = 1; v <= n; ++v) {
        if (!on_cycle[v] || cycle_id[v] != -1) {
            continue;
        }

        const int id = static_cast<int>(cycle_length.size());
        int current = v;
        int position = 0;
        do {
            cycle_id[current] = id;
            cycle_position[current] = position++;
            current = next[current];
        } while (current != v);
        cycle_length.push_back(position);
    }

    // Bước 3: bỏ các cạnh cycle -> rừng cây với gốc là các đỉnh cycle.
    // Xây danh sách con theo cạnh ngược: u là con của next[u] nếu u không nằm trên cycle.
    std::vector<std::vector<int>> children(n + 1);
    for (int v = 1; v <= n; ++v) {
        if (!on_cycle[v]) {
            children[next[v]].push_back(v);
        }
    }

    // DFS iterative từ mỗi đỉnh cycle để tính depth, root và Euler tour (tin/tout).
    std::vector<int> depth(n + 1, 0);
    std::vector<int> root(n + 1, 0);
    std::vector<int> tin(n + 1, 0);
    std::vector<int> tout(n + 1, 0);
    std::vector<int> child_index(n + 1, 0);
    int timer = 0;

    for (int cycle_vertex = 1; cycle_vertex <= n; ++cycle_vertex) {
        if (!on_cycle[cycle_vertex]) {
            continue;
        }

        root[cycle_vertex] = cycle_vertex;
        tin[cycle_vertex] = timer++;
        std::vector<int> stack{cycle_vertex};

        while (!stack.empty()) {
            const int node = stack.back();
            if (child_index[node] < static_cast<int>(children[node].size())) {
                const int child = children[node][child_index[node]++];
                depth[child] = depth[node] + 1;
                root[child] = cycle_vertex;
                tin[child] = timer++;
                stack.push_back(child);
            } else {
                tout[node] = timer - 1;
                stack.pop_back();
            }
        }
    }

    // Bước 4: trả lời từng truy vấn (a, b).
    while (q-- > 0) {
        int a, b;
        std::cin >> a >> b;

        if (on_cycle[b]) {
            // b nằm trên cycle: chỉ tới được nếu a và b cùng component.
            const int entry = root[a];
            if (cycle_id[entry] != cycle_id[b]) {
                std::cout << -1 << '\n';
                continue;
            }

            // depth[a] bước tới đỉnh vào cycle, cộng khoảng cách đi vòng tới b.
            const int length = cycle_length[cycle_id[b]];
            const int around =
                (cycle_position[b] - cycle_position[entry] + length) % length;
            std::cout << depth[a] + around << '\n';
        } else if (tin[b] <= tin[a] && tin[a] <= tout[b]) {
            // b nằm trong cây và là tổ tiên của a: đáp án là hiệu độ sâu.
            std::cout << depth[a] - depth[b] << '\n';
        } else {
            std::cout << -1 << '\n';
        }
    }

    return 0;
}

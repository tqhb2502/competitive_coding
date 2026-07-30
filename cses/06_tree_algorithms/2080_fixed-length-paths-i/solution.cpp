#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, target_length;
    std::cin >> n >> target_length;

    // Đọc cây và dựng danh sách kề (đồ thị vô hướng).
    std::vector<std::vector<int>> graph(n + 1);
    for (int edge = 0; edge < n - 1; ++edge) {
        int first, second;
        std::cin >> first >> second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    // DFS ITERATIVE từ root 1 (dùng stack tường minh, tránh tràn stack khi n lớn):
    // tính parent[] và thứ tự preorder trong order[].
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

    // height[node] = số cạnh của đường đi xuống dài nhất từ node.
    // heavy_child[node] = con đạt được height đó; con này sẽ được "thừa kế"
    // mảng đếm khoảng cách mà không phải sao chép (long-path decomposition).
    // Duyệt bottom-up (đảo preorder): con luôn được xử lý trước cha.
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

    // Cấp phát memory pool: mỗi heavy chain chiếm một đoạn liên tiếp kích thước
    // height+1. Với cạnh heavy u->v ta đặt pointer[u] + 1 == pointer[v], nhờ đó
    // cnt[u][d] dùng lại đúng ô cnt[v][d-1] (thừa kế heavy child trong O(1)).
    std::vector<int> pointer(n + 1, 0);
    int pool_size = 0;
    for (const int node : order) {
        if (node != 1 && heavy_child[parent[node]] == node) {
            continue;
        }
        // node là đỉnh đầu một heavy chain; gán con trỏ dọc theo cả chuỗi.
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

    // Duyệt bottom-up: khi xử lý node thì mọi con đã xong, riêng heavy child đã
    // sẵn nằm chung vùng nhớ với node.
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int node = *iterator;
        const int node_pointer = pointer[node];
        distance_count[node_pointer] = 1;   // cnt[node][0] = 1 (chính node)

        // Chỉ lặp qua các light child (mọi con trừ heavy child).
        for (const int child : graph[node]) {
            if (parent[child] != node || child == heavy_child[node]) {
                continue;
            }

            const int child_pointer = pointer[child];
            // Pairing: đỉnh cách child là child_depth thì cách node là
            // child_depth+1; ghép với một đỉnh đã gộp trước cách node đúng
            // previous_depth = k-(child_depth+1). Ràng buộc >= 1 loại bỏ chính
            // node và bảo đảm hai đầu mút đều nằm thực sự dưới node.
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

            // Merge cnt[child] vào cnt[node] (dịch lên một khoảng cách).
            for (int child_depth = 0;
                 child_depth <= height[child]; ++child_depth) {
                distance_count[node_pointer + child_depth + 1] +=
                    distance_count[child_pointer + child_depth];
            }
        }

        // Trường hợp node là một đầu mút của đường đi thẳng đứng dài đúng k.
        if (target_length <= height[node]) {
            answer += distance_count[node_pointer + target_length];
        }
    }

    std::cout << answer << '\n';
    return 0;
}

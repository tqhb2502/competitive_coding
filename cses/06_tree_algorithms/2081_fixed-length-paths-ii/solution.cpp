#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

// Một cây con (branch) của centroid: danh sách depth của các đỉnh và depth lớn nhất
struct DepthBranch {
    std::vector<int> depths;
    int maximum_depth = 0;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, minimum_length, maximum_length;
    std::cin >> n >> minimum_length >> maximum_length;

    // Danh sách kề của cây
    std::vector<std::vector<int>> graph(n + 1);
    for (int edge = 0; edge < n - 1; ++edge) {
        int first, second;
        std::cin >> first >> second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    std::vector<char> removed(n + 1, false);      // đỉnh đã bị chọn làm centroid và xóa
    std::vector<int> parent(n + 1, 0);            // cha khi duyệt component để tính size
    std::vector<int> subtree_size(n + 1, 0);      // kích thước cây con trong component
    std::vector<int> frequency(maximum_length + 1, 0);  // đếm số đỉnh theo từng depth
    std::vector<int> prefix(maximum_length + 1, 0);     // tổng tiền tố của frequency
    std::vector<int> component;
    component.reserve(n);
    std::vector<int> component_stack{1};          // các component chờ xử lý
    long long answer = 0;

    while (!component_stack.empty()) {
        const int start = component_stack.back();
        component_stack.pop_back();
        if (removed[start]) {
            continue;
        }

        // --- Duyệt component (iterative) khi root tại start ---
        component.clear();
        std::vector<int> traversal{start};
        parent[start] = 0;
        while (!traversal.empty()) {
            const int node = traversal.back();
            traversal.pop_back();
            component.push_back(node);
            for (const int next : graph[node]) {
                if (removed[next] || next == parent[node]) {
                    continue;
                }
                parent[next] = node;
                traversal.push_back(next);
            }
        }

        // Tính subtree size: duyệt ngược thứ tự phát hiện để cộng từ lá lên gốc
        for (const int node : component) {
            subtree_size[node] = 1;
        }
        for (auto iterator = component.rbegin();
             iterator != component.rend(); ++iterator) {
            const int node = *iterator;
            if (parent[node] != 0) {
                subtree_size[parent[node]] += subtree_size[node];
            }
        }

        // --- Tìm centroid: đỉnh mà mọi phần tách ra đều <= total/2 ---
        const int total_size = static_cast<int>(component.size());
        int centroid = start;
        for (const int node : component) {
            int largest_part = total_size - subtree_size[node];
            for (const int next : graph[node]) {
                if (!removed[next] && parent[next] == node) {
                    largest_part = std::max(
                        largest_part, subtree_size[next]
                    );
                }
            }
            if (largest_part * 2 <= total_size) {
                centroid = node;
                break;
            }
        }

        // --- Với mỗi cây con của centroid, DFS iterative lấy mảng depth ---
        // Chỉ giữ các depth <= maximum_length vì phần dài hơn không thể tạo path hợp lệ
        std::vector<DepthBranch> branches;
        branches.reserve(graph[centroid].size());
        for (const int neighbor : graph[centroid]) {
            if (removed[neighbor]) {
                continue;
            }

            DepthBranch branch;
            std::vector<std::tuple<int, int, int>> depth_stack;
            depth_stack.emplace_back(neighbor, centroid, 1);
            while (!depth_stack.empty()) {
                const auto [node, ancestor, depth] = depth_stack.back();
                depth_stack.pop_back();
                if (depth > maximum_length) {
                    continue;
                }

                branch.depths.push_back(depth);
                branch.maximum_depth = std::max(
                    branch.maximum_depth, depth
                );
                for (const int next : graph[node]) {
                    if (!removed[next] && next != ancestor) {
                        depth_stack.emplace_back(next, node, depth + 1);
                    }
                }
            }
            if (!branch.depths.empty()) {
                branches.push_back(std::move(branch));
            }
        }

        // Mỗi cặp được đếm khi xử lý cây con thứ hai của nó. Sắp các cây con theo
        // depth lớn nhất giúp việc dựng lại mảng prefix-frequency chỉ tốn tuyến
        // tính theo tổng kích thước các cây con tại centroid này.
        std::sort(
            branches.begin(), branches.end(),
            [](const DepthBranch& first, const DepthBranch& second) {
                return first.maximum_depth < second.maximum_depth;
            }
        );

        frequency[0] = 1;  // depth 0 là chính centroid, để đếm path một đầu là centroid
        int current_maximum = 0;
        for (const DepthBranch& branch : branches) {
            // Dựng prefix-sum của frequency (các cây con đã xét trước đó)
            prefix[0] = frequency[0];
            for (int depth = 1; depth <= current_maximum; ++depth) {
                prefix[depth] = prefix[depth - 1] + frequency[depth];
            }

            // Ghép mỗi đỉnh của cây con hiện tại với các đỉnh cây con trước đó sao
            // cho tổng depth nằm trong [minimum_length, maximum_length]
            for (const int depth : branch.depths) {
                const int low = std::max(0, minimum_length - depth);
                const int high = std::min(
                    current_maximum, maximum_length - depth
                );
                if (low <= high) {
                    answer += prefix[high];
                    if (low > 0) {
                        answer -= prefix[low - 1];
                    }
                }
            }

            // Thêm depth của cây con hiện tại vào frequency cho các cây con sau
            for (const int depth : branch.depths) {
                ++frequency[depth];
            }
            current_maximum = std::max(
                current_maximum, branch.maximum_depth
            );
        }
        // Reset frequency về 0 cho centroid tiếp theo
        std::fill(
            frequency.begin(), frequency.begin() + current_maximum + 1, 0
        );

        // --- Xóa centroid, đẩy các component con vào stack để đệ quy ---
        removed[centroid] = true;
        for (const int next : graph[centroid]) {
            if (!removed[next]) {
                component_stack.push_back(next);
            }
        }
    }

    std::cout << answer << '\n';
    return 0;
}

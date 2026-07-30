#include <algorithm>
#include <iostream>
#include <vector>

// Fenwick tree (BIT): point update và prefix sum trên rank giá trị đã nén
class FenwickTree {
public:
    explicit FenwickTree(const int size) : size_(size), tree_(size + 1, 0) {}

    // Cộng value vào vị trí index
    void add(int index, const int value) {
        while (index <= size_) {
            tree_[index] += value;
            index += index & -index;
        }
    }

    // Tổng tiền tố trên đoạn [1, index]
    int prefix_sum(int index) const {
        int result = 0;
        while (index > 0) {
            result += tree_[index];
            index -= index & -index;
        }
        return result;
    }

private:
    int size_;
    std::vector<int> tree_;
};

// Một số hạng f(i, v) trong công thức bao hàm - loại trừ:
// tại chỉ số i, hỏi prefix sum đến value_rank rồi cộng với dấu sign vào truy vấn
struct Event {
    int value_rank;
    int sign;
    int query_index;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<int> values(n + 1);
    std::vector<int> sorted_values;
    sorted_values.reserve(n);
    for (int index = 1; index <= n; ++index) {
        std::cin >> values[index];
        sorted_values.push_back(values[index]);
    }

    // Nén tọa độ: sắp xếp và loại trùng để lấy tập giá trị phân biệt
    std::sort(sorted_values.begin(), sorted_values.end());
    sorted_values.erase(
        std::unique(sorted_values.begin(), sorted_values.end()),
        sorted_values.end()
    );

    // events[i]: các sự kiện cần xử lý khi quét tới chỉ số i (gom theo bucket)
    std::vector<std::vector<Event>> events(n + 1);
    std::vector<int> answers(q, 0);

    // Số giá trị phân biệt ≤ threshold, cũng là chỉ số prefix cần hỏi trên BIT
    const auto rank_at_most = [&sorted_values](const int threshold) {
        return static_cast<int>(
            std::upper_bound(
                sorted_values.begin(), sorted_values.end(), threshold
            ) - sorted_values.begin()
        );
    };

    // Tách mỗi truy vấn [a, b] × [c, d] thành tối đa 4 số hạng f theo bao hàm - loại trừ:
    //   f(b, d) - f(b, c-1) - f(a-1, d) + f(a-1, c-1)
    for (int query_index = 0; query_index < q; ++query_index) {
        int first, last, minimum_value, maximum_value;
        std::cin >> first >> last >> minimum_value >> maximum_value;

        const int upper_rank = rank_at_most(maximum_value);
        const int lower_rank = rank_at_most(minimum_value - 1);

        // Hai số hạng tại chỉ số b (last)
        if (upper_rank > 0) {
            events[last].push_back({upper_rank, 1, query_index});
        }
        if (lower_rank > 0) {
            events[last].push_back({lower_rank, -1, query_index});
        }
        // Hai số hạng tại chỉ số a-1 (bỏ qua khi a-1 = 0 vì f = 0)
        if (first > 1) {
            if (upper_rank > 0) {
                events[first - 1].push_back({upper_rank, -1, query_index});
            }
            if (lower_rank > 0) {
                events[first - 1].push_back({lower_rank, 1, query_index});
            }
        }
    }

    // Quét chỉ số i tăng dần: thêm x_i vào BIT rồi trả lời các sự kiện tại i
    FenwickTree frequencies(static_cast<int>(sorted_values.size()));
    for (int index = 1; index <= n; ++index) {
        // Rank (1-indexed) của x_i để point update +1
        const int rank = static_cast<int>(
            std::lower_bound(
                sorted_values.begin(), sorted_values.end(), values[index]
            ) - sorted_values.begin()
        ) + 1;
        frequencies.add(rank, 1);

        // BIT lúc này chứa mọi phần tử chỉ số ≤ i nên prefix sum = f(i, value_rank)
        for (const Event& event : events[index]) {
            answers[event.query_index] +=
                event.sign * frequencies.prefix_sum(event.value_rank);
        }
    }

    for (const int answer : answers) {
        std::cout << answer << '\n';
    }
    return 0;
}

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

// Lưu lại từng truy vấn để xử lí offline: type là '!' (cập nhật) hoặc '?' (đếm).
struct Query {
    char type;
    int a;
    int b;
};

// Fenwick tree (BIT) duy trì tần suất lương theo vị trí nén, hỗ trợ
// point-update và prefix-sum, mỗi thao tác O(log N).
class FenwickTree {
public:
    explicit FenwickTree(std::size_t size) : tree_(size + 1, 0) {}

    // Cộng delta vào một vị trí rồi lan lên các nút cha theo i & -i.
    void add(std::size_t position, int delta) {
        const std::size_t size = tree_.size() - 1;
        for (std::size_t i = position; i <= size; i += i & -i) {
            tree_[i] += delta;
        }
    }

    // Tổng tiền tố [1, position].
    [[nodiscard]] int prefix_sum(std::size_t position) const {
        int result = 0;
        for (std::size_t i = position; i > 0; i -= i & -i) {
            result += tree_[i];
        }
        return result;
    }

private:
    std::vector<int> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t q = 0;
    std::cin >> n >> q;

    // Đọc lương ban đầu, đồng thời gom vào tập giá trị cần nén.
    std::vector<int> salary(n);
    std::vector<int> values;
    values.reserve(n + q);
    for (int& value : salary) {
        std::cin >> value;
        values.push_back(value);
    }

    // Đọc offline mọi truy vấn; với "! k x" gom thêm lương mới x vào tập nén.
    std::vector<Query> queries;
    queries.reserve(q);
    for (std::size_t i = 0; i < q; ++i) {
        Query query{};
        std::cin >> query.type >> query.a >> query.b;
        queries.push_back(query);
        if (query.type == '!') {
            values.push_back(query.b);
        }
    }

    // Coordinate compression: sắp xếp và lấy tập giá trị duy nhất.
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    // Đổi một mức lương sang vị trí nén 1-indexed (BIT đánh chỉ số từ 1).
    const auto compressed_position = [&values](int value) {
        return static_cast<std::size_t>(
                   std::lower_bound(values.begin(), values.end(), value) - values.begin()) +
               1;
    };

    // Khởi tạo BIT: cộng 1 vào vị trí nén của lương ban đầu từng nhân viên,
    // lưu current_position[i] là vị trí nén hiện tại của nhân viên i.
    FenwickTree frequencies(values.size());
    std::vector<std::size_t> current_position(n);
    for (std::size_t i = 0; i < n; ++i) {
        current_position[i] = compressed_position(salary[i]);
        frequencies.add(current_position[i], 1);
    }

    for (const Query& query : queries) {
        if (query.type == '!') {
            // Cập nhật lương: rút 1 khỏi vị trí cũ, đặt vị trí mới rồi cộng 1.
            const std::size_t employee = static_cast<std::size_t>(query.a - 1);
            frequencies.add(current_position[employee], -1);
            current_position[employee] = compressed_position(query.b);
            frequencies.add(current_position[employee], 1);
        } else {
            // Đếm số lương trong [a, b] = prefix(số lương <= b) - prefix(số lương < a);
            // dùng bisect quy a, b về biên chỉ số hợp lệ trên tập đã nén.
            const std::size_t below_lower = static_cast<std::size_t>(
                std::lower_bound(values.begin(), values.end(), query.a) - values.begin());
            const std::size_t at_most_upper = static_cast<std::size_t>(
                std::upper_bound(values.begin(), values.end(), query.b) - values.begin());
            std::cout << frequencies.prefix_sum(at_most_upper) -
                             frequencies.prefix_sum(below_lower)
                      << '\n';
        }
    }
    return 0;
}

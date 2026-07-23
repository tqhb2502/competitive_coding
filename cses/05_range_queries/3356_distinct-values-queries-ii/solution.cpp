#include <algorithm>
#include <iostream>
#include <vector>

struct Query {
    int type;
    int first;
    int second;
};

// Fenwick/BIT lưu cờ present/absent của các cặp (value, position) đã nén.
// Ngoài add/prefix_sum còn hỗ trợ select: tìm chỉ số của phần tử active thứ k.
class FenwickTree {
public:
    explicit FenwickTree(const int size)
        : size_(size), tree_(size + 1, 0) {
        // highest_power_ = luỹ thừa 2 lớn nhất <= size_, phục vụ binary lifting.
        highest_power_ = 1;
        while (highest_power_ <= size_ / 2) {
            highest_power_ *= 2;
        }
    }

    // Bật/tắt một cặp (activate / deactivate): cộng value vào vị trí index.
    void add(int index, const int value) {
        while (index <= size_) {
            tree_[index] += value;
            index += index & -index;
        }
    }

    // Đếm số cặp active trong tiền tố [1..index].
    int prefix_sum(int index) const {
        int result = 0;
        while (index > 0) {
            result += tree_[index];
            index -= index & -index;
        }
        return result;
    }

    // Binary lifting: trả về chỉ số của phần tử active thứ "order" (1-based).
    int select(const int order) const {
        int index = 0;
        int remaining = order;
        for (int step = highest_power_; step > 0; step /= 2) {
            const int next = index + step;
            if (next <= size_ && tree_[next] < remaining) {
                index = next;
                remaining -= tree_[next];
            }
        }
        return index + 1;
    }

private:
    int size_;
    int highest_power_;
    std::vector<int> tree_;
};

// Segment tree lặp (bottom-up) cho range-max của prev[], hỗ trợ gán điểm.
// Không dùng Fenwick vì prev[i] có thể GIẢM khi cập nhật.
class MaximumSegmentTree {
public:
    explicit MaximumSegmentTree(const std::vector<int>& values) {
        // values được đánh chỉ số từ 1; count là số phần tử thực.
        const int count = static_cast<int>(values.size()) - 1;
        size_ = 1;
        while (size_ < count) {
            size_ *= 2;
        }
        tree_.assign(2 * size_, 0);
        // Nạp lá rồi dựng ngược lên gốc bằng max của hai con.
        for (int position = 1; position <= count; ++position) {
            tree_[size_ + position - 1] = values[position];
        }
        for (int index = size_ - 1; index > 0; --index) {
            tree_[index] = std::max(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    // Gán điểm: đặt prev[position] = value rồi cập nhật dọc lên gốc.
    void assign(const int position, const int value) {
        int index = size_ + position - 1;
        tree_[index] = value;
        for (index /= 2; index > 0; index /= 2) {
            tree_[index] = std::max(tree_[2 * index], tree_[2 * index + 1]);
        }
    }

    // Truy vấn max trên đoạn [left, right].
    int query(int left, const int right) const {
        int result = 0;
        left += size_ - 1;
        int end = right + size_;
        while (left < end) {
            if ((left & 1) != 0) {
                result = std::max(result, tree_[left++]);
            }
            if ((end & 1) != 0) {
                result = std::max(result, tree_[--end]);
            }
            left /= 2;
            end /= 2;
        }
        return result;
    }

private:
    int size_ = 1;
    std::vector<int> tree_;
};

// Mã hoá một cặp (value_rank, position) thành một khoá số nguyên duy nhất
// để có thể sắp xếp theo thứ tự (value, position).
long long pair_key(const int value_rank, const int position, const int stride) {
    return static_cast<long long>(value_rank) * stride + position;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    // Đọc mảng ban đầu, đồng thời thu thập mọi giá trị để nén tọa độ.
    std::vector<int> values(n + 1);
    std::vector<int> all_values;
    all_values.reserve(n + q);
    for (int position = 1; position <= n; ++position) {
        std::cin >> values[position];
        all_values.push_back(values[position]);
    }

    // Đọc các truy vấn; giá trị đích u của mỗi update cũng phải được nén.
    std::vector<Query> queries;
    queries.reserve(q);
    for (int query_index = 0; query_index < q; ++query_index) {
        Query query{};
        std::cin >> query.type >> query.first >> query.second;
        queries.push_back(query);
        if (query.type == 1) {
            all_values.push_back(query.second);
        }
    }

    // Nén giá trị: đổi mỗi giá trị thành thứ hạng (value_rank).
    std::sort(all_values.begin(), all_values.end());
    all_values.erase(
        std::unique(all_values.begin(), all_values.end()), all_values.end()
    );
    const auto value_rank = [&all_values](const int value) {
        return static_cast<int>(
            std::lower_bound(all_values.begin(), all_values.end(), value) -
            all_values.begin()
        );
    };

    // Nén MỌI cặp (value, position) có thể xuất hiện (mảng ban đầu + các update),
    // sắp theo thứ tự (value_rank, position). Hàng xóm cùng giá trị của một cặp
    // chính là hàng xóm toàn cục liền kề trong thứ tự này.
    const int stride = n + 1;
    std::vector<long long> possible_pairs;
    possible_pairs.reserve(n + q);
    for (int position = 1; position <= n; ++position) {
        possible_pairs.push_back(
            pair_key(value_rank(values[position]), position, stride)
        );
    }
    for (const Query& query : queries) {
        if (query.type == 1) {
            possible_pairs.push_back(
                pair_key(value_rank(query.second), query.first, stride)
            );
        }
    }
    std::sort(possible_pairs.begin(), possible_pairs.end());
    possible_pairs.erase(
        std::unique(possible_pairs.begin(), possible_pairs.end()),
        possible_pairs.end()
    );

    // Giải mã lại vị trí và value_rank cho từng chỉ số nén (1-based).
    const int pair_count = static_cast<int>(possible_pairs.size());
    std::vector<int> pair_position(pair_count + 1, 0);
    std::vector<int> pair_value_rank(pair_count + 1, 0);
    for (int index = 1; index <= pair_count; ++index) {
        const long long key = possible_pairs[index - 1];
        pair_position[index] = static_cast<int>(key % stride);
        pair_value_rank[index] = static_cast<int>(key / stride);
    }

    const auto compressed_pair = [&possible_pairs](const long long key) {
        return static_cast<int>(
            std::lower_bound(possible_pairs.begin(), possible_pairs.end(), key) -
            possible_pairs.begin()
        ) + 1;
    };

    // Kích hoạt cặp active hiện tại của từng vị trí trong BIT.
    std::vector<int> current_pair(n + 1, 0);
    FenwickTree active_pairs(pair_count);
    for (int position = 1; position <= n; ++position) {
        current_pair[position] = compressed_pair(
            pair_key(value_rank(values[position]), position, stride)
        );
        active_pairs.add(current_pair[position], 1);
    }

    // Tính prev[] ban đầu: last_position lưu vị trí gần nhất mang mỗi value_rank.
    std::vector<int> previous(n + 1, 0);
    std::vector<int> last_position(all_values.size(), 0);
    for (int position = 1; position <= n; ++position) {
        const int rank = value_rank(values[position]);
        previous[position] = last_position[rank];
        last_position[rank] = position;
    }
    MaximumSegmentTree maximum_previous(previous);

    // Tìm predecessor cùng giá trị của một cặp: lấy phần tử active liền trước,
    // trả về vị trí của nó nếu cùng value_rank, ngược lại 0.
    const auto same_value_predecessor = [
        &active_pairs, &pair_position, &pair_value_rank
    ](const int pair_index, const int rank) {
        const int earlier_count = active_pairs.prefix_sum(pair_index - 1);
        if (earlier_count == 0) {
            return 0;
        }
        const int predecessor = active_pairs.select(earlier_count);
        return pair_value_rank[predecessor] == rank
                   ? pair_position[predecessor]
                   : 0;
    };

    // Tìm successor cùng giá trị của một cặp: lấy phần tử active liền sau,
    // trả về vị trí của nó nếu cùng value_rank, ngược lại 0.
    const auto same_value_successor = [
        &active_pairs, &pair_position, &pair_value_rank
    ](const int pair_index, const int rank, const int active_count) {
        const int through_current = active_pairs.prefix_sum(pair_index);
        if (through_current >= active_count) {
            return 0;
        }
        const int successor = active_pairs.select(through_current + 1);
        return pair_value_rank[successor] == rank
                   ? pair_position[successor]
                   : 0;
    };

    for (const Query& query : queries) {
        // Truy vấn "2 a b": đoạn phân biệt <=> max(prev trên [a, b]) < a.
        if (query.type == 2) {
            std::cout << (
                maximum_previous.query(query.first, query.second) < query.first
                    ? "YES\n"
                    : "NO\n"
            );
            continue;
        }

        // Update "1 k u": xác định cặp cũ và cặp mới của vị trí k.
        const int position = query.first;
        const int old_pair = current_pair[position];
        const int new_pair = compressed_pair(
            pair_key(value_rank(query.second), position, stride)
        );
        if (old_pair == new_pair) {
            // Giá trị mới trùng giá trị hiện tại: không thay đổi gì.
            continue;
        }

        // Gỡ k khỏi nhóm giá trị cũ: successor cũ nay trỏ prev về predecessor cũ.
        const int old_rank = pair_value_rank[old_pair];
        active_pairs.add(old_pair, -1);
        const int old_predecessor =
            same_value_predecessor(old_pair, old_rank);
        const int old_successor =
            same_value_successor(old_pair, old_rank, n - 1);
        if (old_successor != 0) {
            maximum_previous.assign(old_successor, old_predecessor);
        }

        // Chèn k vào nhóm giá trị mới: prev[k] = predecessor mới; successor mới
        // (nếu có) đổi prev sang k.
        active_pairs.add(new_pair, 1);
        const int new_rank = pair_value_rank[new_pair];
        const int new_predecessor =
            same_value_predecessor(new_pair, new_rank);
        const int new_successor =
            same_value_successor(new_pair, new_rank, n);
        maximum_previous.assign(position, new_predecessor);
        if (new_successor != 0) {
            maximum_previous.assign(new_successor, position);
        }
        current_pair[position] = new_pair;
    }

    return 0;
}

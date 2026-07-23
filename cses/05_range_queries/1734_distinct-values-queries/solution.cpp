#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

static size_t to_index(int value) {
    return static_cast<size_t>(value);
}

// Fenwick tree (BIT) 1-indexed để cập nhật điểm và tính tổng tiền tố.
class FenwickTree {
public:
    explicit FenwickTree(int size) : tree_(to_index(size + 1), 0) {}

    // Cộng delta vào vị trí position.
    void add(int position, int delta) {
        for (int i = position; i < static_cast<int>(tree_.size()); i += i & -i) {
            tree_[to_index(i)] += delta;
        }
    }

    // Tổng tiền tố các vị trí 1..position.
    int prefix_sum(int position) const {
        int result = 0;
        for (int i = position; i > 0; i -= i & -i) {
            result += tree_[to_index(i)];
        }
        return result;
    }

private:
    vector<int> tree_;
};

struct Query {
    int left;
    int right;
    int index;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> values(to_index(n + 1));
    for (int i = 1; i <= n; ++i) {
        cin >> values[to_index(i)];
    }

    // Đọc truy vấn rồi SẮP XẾP theo đầu mút phải tăng dần (xử lý offline).
    vector<Query> queries(to_index(q));
    for (int i = 0; i < q; ++i) {
        cin >> queries[to_index(i)].left >> queries[to_index(i)].right;
        queries[to_index(i)].index = i;
    }
    sort(queries.begin(), queries.end(), [](const Query &a, const Query &b) {
        return a.right < b.right;
    });

    FenwickTree active_positions(n);
    // last_position[x] = vị trí xuất hiện gần nhất của giá trị x.
    unordered_map<int, int> last_position;
    last_position.reserve(static_cast<size_t>(2 * n));
    last_position.max_load_factor(0.7F);

    vector<int> answers(to_index(q));
    int position = 1;
    for (const Query &query : queries) {
        // Mở rộng tiền tố tới query.right, chỉ giữ dấu 1 ở lần xuất hiện cuối.
        while (position <= query.right) {
            const auto previous = last_position.find(values[to_index(position)]);
            if (previous != last_position.end()) {
                active_positions.add(previous->second, -1);  // gỡ đánh dấu cũ
            }
            active_positions.add(position, 1);               // đánh dấu vị trí mới
            last_position[values[to_index(position)]] = position;
            ++position;
        }
        // Đếm số giá trị phân biệt trong [left, right] bằng tổng đoạn BIT.
        answers[to_index(query.index)] = active_positions.prefix_sum(query.right)
                                      - active_positions.prefix_sum(query.left - 1);
    }

    for (int answer : answers) {
        cout << answer << '\n';
    }

    return 0;
}

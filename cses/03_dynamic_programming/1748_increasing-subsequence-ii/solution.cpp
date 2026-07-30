#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// Fenwick tree (BIT) lưu tổng dp theo rank giá trị, hỗ trợ prefix sum có lấy mod.
class FenwickTree {
public:
    explicit FenwickTree(int size) : tree_(size + 1, 0) {}

    // Cộng value vào vị trí index rồi lan lên các nút cha; giữ mỗi ô trong [0, MOD).
    void add(int index, int value) {
        while (index < static_cast<int>(tree_.size())) {
            tree_[index] += value;
            if (tree_[index] >= MOD) {
                tree_[index] -= MOD;
            }
            index += index & -index;
        }
    }

    // Tổng dp trên prefix [1, index] theo giá trị, lấy modulo MOD.
    int prefix_sum(int index) const {
        long long result = 0;
        while (index > 0) {
            result += tree_[index];
            index -= index & -index;
        }
        return static_cast<int>(result % MOD);
    }

private:
    static constexpr int MOD = 1'000'000'007;
    vector<int> tree_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n;
    cin >> n;
    vector<long long> values(n);
    for (long long& value : values) {
        cin >> value;
    }

    // Nén tọa độ: các giá trị phân biệt sau khi sort sẽ được ánh xạ sang rank 1..m.
    vector<long long> compressed = values;
    sort(compressed.begin(), compressed.end());
    compressed.erase(unique(compressed.begin(), compressed.end()), compressed.end());

    FenwickTree fenwick(static_cast<int>(compressed.size()));
    long long answer = 0;
    // Duyệt từ trái sang phải nên BIT chỉ chứa các j < i, bảo đảm điều kiện j < i.
    for (const long long value : values) {
        const int rank = static_cast<int>(
                             lower_bound(compressed.begin(), compressed.end(), value) - compressed.begin()) +
                         1;

        // dp[i] = 1 + tổng dp[j] với a[j] < a[i]; truy vấn tới rank - 1 để bảo đảm
        // tăng NGẶT (các giá trị bằng nhau không được nối vào nhau).
        const int ending_here = (fenwick.prefix_sum(rank - 1) + 1LL) % MOD;
        answer += ending_here;
        if (answer >= MOD) {
            answer -= MOD;
        }
        // Cộng dp vừa tính vào BIT tại vị trí rank của giá trị hiện tại.
        fenwick.add(rank, ending_here);
    }

    cout << answer << '\n';
    return 0;
}

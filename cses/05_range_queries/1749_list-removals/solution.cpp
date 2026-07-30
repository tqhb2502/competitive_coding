#include <iostream>
#include <vector>

// Fenwick tree (BIT) lưu prefix sum của mảng đánh dấu "còn sống".
class Fenwick {
public:
    explicit Fenwick(int n) : data_(static_cast<std::size_t>(n + 1), 0) {
        // Ban đầu mọi vị trí gốc đều còn sống (giá trị 1) -> build BIT trong O(n).
        for (int i = 1; i <= n; ++i) {
            data_[static_cast<std::size_t>(i)] = i & -i;
        }
    }

    // Xóa phần tử tại position: point update -1 dọc theo BIT, O(log n).
    void remove(int position) {
        const int n = static_cast<int>(data_.size()) - 1;
        for (int i = position; i <= n; i += i & -i) {
            --data_[static_cast<std::size_t>(i)];
        }
    }

    // Tìm phần tử còn sống thứ rank bằng binary lifting trên Fenwick, O(log n).
    [[nodiscard]] int kth(int rank) const {
        const int n = static_cast<int>(data_.size()) - 1;
        // step = luỹ thừa 2 lớn nhất không vượt quá n.
        int step = 1;
        while (step <= n / 2) {
            step *= 2;
        }

        // Duyệt bit từ cao xuống thấp, nhảy qua các đoạn có prefix sum còn nhỏ hơn rank.
        int position = 0;
        for (; step > 0; step /= 2) {
            const int next = position + step;
            if (next <= n && data_[static_cast<std::size_t>(next)] < rank) {
                position = next;
                rank -= data_[static_cast<std::size_t>(next)];
            }
        }
        // position + 1 là vị trí nhỏ nhất có prefix sum bằng rank.
        return position + 1;
    }

private:
    std::vector<int> data_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    // Giá trị gốc của danh sách (đánh số 1..n).
    std::vector<long long> values(static_cast<std::size_t>(n + 1), 0);
    for (int i = 1; i <= n; ++i) {
        std::cin >> values[static_cast<std::size_t>(i)];
    }

    Fenwick alive(n);
    for (int i = 0; i < n; ++i) {
        // rank = vị trí p_i trong danh sách hiện tại.
        int rank = 0;
        std::cin >> rank;

        // Tìm vị trí gốc của phần tử còn sống thứ rank, in ra rồi xóa nó.
        const int position = alive.kth(rank);
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << values[static_cast<std::size_t>(position)];
        alive.remove(position);
    }
    std::cout << '\n';
}

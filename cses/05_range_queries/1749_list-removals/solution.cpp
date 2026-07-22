#include <iostream>
#include <vector>

class Fenwick {
public:
    explicit Fenwick(int n) : data_(static_cast<std::size_t>(n + 1), 0) {
        // Every original position is alive, so this is the O(n) Fenwick build.
        for (int i = 1; i <= n; ++i) {
            data_[static_cast<std::size_t>(i)] = i & -i;
        }
    }

    void remove(int position) {
        const int n = static_cast<int>(data_.size()) - 1;
        for (int i = position; i <= n; i += i & -i) {
            --data_[static_cast<std::size_t>(i)];
        }
    }

    [[nodiscard]] int kth(int rank) const {
        const int n = static_cast<int>(data_.size()) - 1;
        int step = 1;
        while (step <= n / 2) {
            step *= 2;
        }

        int position = 0;
        for (; step > 0; step /= 2) {
            const int next = position + step;
            if (next <= n && data_[static_cast<std::size_t>(next)] < rank) {
                position = next;
                rank -= data_[static_cast<std::size_t>(next)];
            }
        }
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
    std::vector<long long> values(static_cast<std::size_t>(n + 1), 0);
    for (int i = 1; i <= n; ++i) {
        std::cin >> values[static_cast<std::size_t>(i)];
    }

    Fenwick alive(n);
    for (int i = 0; i < n; ++i) {
        int rank = 0;
        std::cin >> rank;
        const int position = alive.kth(rank);
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << values[static_cast<std::size_t>(position)];
        alive.remove(position);
    }
    std::cout << '\n';
}

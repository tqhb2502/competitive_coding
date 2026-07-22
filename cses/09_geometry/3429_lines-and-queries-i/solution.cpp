#include <algorithm>
#include <iostream>
#include <vector>

struct Line {
    long long slope = 0;
    long long intercept = 0;

    long long value(const int x) const {
        return slope * x + intercept;
    }
};

class LiChaoTree {
public:
    explicit LiChaoTree(const int maximum_x)
        : maximum_x_(maximum_x),
          lines_(static_cast<std::size_t>(4 * (maximum_x + 1))),
          occupied_(static_cast<std::size_t>(4 * (maximum_x + 1)), false) {}

    void insert(const Line line) {
        insert(line, 1, 0, maximum_x_);
    }

    long long query(const int x) const {
        return query(x, 1, 0, maximum_x_);
    }

private:
    void insert(Line incoming, const int node, const int left, const int right) {
        if (!occupied_[static_cast<std::size_t>(node)]) {
            occupied_[static_cast<std::size_t>(node)] = true;
            lines_[static_cast<std::size_t>(node)] = incoming;
            return;
        }

        const int middle = (left + right) / 2;
        const bool better_left =
            incoming.value(left) > lines_[static_cast<std::size_t>(node)].value(left);
        const bool better_middle =
            incoming.value(middle) > lines_[static_cast<std::size_t>(node)].value(middle);
        if (better_middle) {
            std::swap(incoming, lines_[static_cast<std::size_t>(node)]);
        }
        if (left == right) {
            return;
        }
        if (better_left != better_middle) {
            insert(incoming, 2 * node, left, middle);
        } else {
            insert(incoming, 2 * node + 1, middle + 1, right);
        }
    }

    long long query(const int x, const int node, const int left,
                    const int right) const {
        long long best = occupied_[static_cast<std::size_t>(node)]
            ? lines_[static_cast<std::size_t>(node)].value(x)
            : -(1LL << 62);
        if (left == right) {
            return best;
        }
        const int middle = (left + right) / 2;
        if (x <= middle) {
            best = std::max(best, query(x, 2 * node, left, middle));
        } else {
            best = std::max(best, query(x, 2 * node + 1, middle + 1, right));
        }
        return best;
    }

    int maximum_x_;
    std::vector<Line> lines_;
    std::vector<char> occupied_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int MAXIMUM_X = 100'000;
    int query_count;
    std::cin >> query_count;
    LiChaoTree tree(MAXIMUM_X);
    while (query_count-- > 0) {
        int type;
        std::cin >> type;
        if (type == 1) {
            Line line;
            std::cin >> line.slope >> line.intercept;
            tree.insert(line);
        } else {
            int x;
            std::cin >> x;
            std::cout << tree.query(x) << '\n';
        }
    }
    return 0;
}

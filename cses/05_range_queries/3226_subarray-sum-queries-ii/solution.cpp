#include <algorithm>
#include <iostream>
#include <vector>

struct SegmentSummary {
    long long sum = 0;
    long long prefix = 0;
    long long suffix = 0;
    long long best = 0;
};

SegmentSummary combine(const SegmentSummary& left,
                       const SegmentSummary& right) {
    return {
        left.sum + right.sum,
        std::max(left.prefix, left.sum + right.prefix),
        std::max(right.suffix, right.sum + left.suffix),
        std::max({left.best, right.best, left.suffix + right.prefix})
    };
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    int size = 1;
    while (size < n) {
        size *= 2;
    }
    std::vector<SegmentSummary> tree(2 * size);

    for (int index = 0; index < n; ++index) {
        long long value;
        std::cin >> value;
        const long long nonnegative = std::max(0LL, value);
        tree[size + index] = {
            value, nonnegative, nonnegative, nonnegative
        };
    }
    for (int index = size - 1; index > 0; --index) {
        tree[index] = combine(tree[2 * index], tree[2 * index + 1]);
    }

    while (q-- > 0) {
        int first, last;
        std::cin >> first >> last;
        int left = size + first - 1;
        int right = size + last;
        SegmentSummary left_result;
        SegmentSummary right_result;

        while (left < right) {
            if ((left & 1) != 0) {
                left_result = combine(left_result, tree[left++]);
            }
            if ((right & 1) != 0) {
                right_result = combine(tree[--right], right_result);
            }
            left /= 2;
            right /= 2;
        }

        std::cout << combine(left_result, right_result).best << '\n';
    }
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

// Mỗi node lưu 4 giá trị của một đoạn: tổng, max prefix, max suffix, best subarray.
// Các giá trị prefix/suffix/best đều clamp về >= 0 vì cho phép đoạn con rỗng.
struct SegmentSummary {
    long long sum = 0;
    long long prefix = 0;
    long long suffix = 0;
    long long best = 0;
};

// Phép merge KHÔNG giao hoán: gộp node trái L với node phải R theo đúng thứ tự.
// best tối ưu = nằm trong L, nằm trong R, hoặc bắc cầu (L.suffix + R.prefix).
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

    // Kích thước cây làm tròn lên lũy thừa của 2 để dùng segment tree bottom-up.
    int size = 1;
    while (size < n) {
        size *= 2;
    }
    std::vector<SegmentSummary> tree(2 * size);

    // Khởi tạo lá: với phần tử x, đặt p = max(x, 0) rồi gán prefix=suffix=best=p.
    for (int index = 0; index < n; ++index) {
        long long value;
        std::cin >> value;
        const long long nonnegative = std::max(0LL, value);
        tree[size + index] = {
            value, nonnegative, nonnegative, nonnegative
        };
    }
    // Xây các node trong bằng cách merge hai con.
    for (int index = size - 1; index > 0; --index) {
        tree[index] = combine(tree[2 * index], tree[2 * index + 1]);
    }

    while (q-- > 0) {
        int first, last;
        std::cin >> first >> last;
        int left = size + first - 1;
        int right = size + last;  // biên phải nửa mở

        // Vì merge không giao hoán, giữ hai bộ tích lũy riêng: các mảnh phía trái
        // gộp từ trái sang, các mảnh phía phải gộp từ phải sang.
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

        // Gộp hai bộ tích lũy theo thứ tự trái -> phải, lấy trường best làm đáp án.
        std::cout << combine(left_result, right_result).best << '\n';
    }
    return 0;
}

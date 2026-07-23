#ifndef CP_LIBRARY_FOUNDATIONS_TWO_POINTERS_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_TWO_POINTERS_ALGORITHM_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace cp_library {

// values phải được sắp xếp không giảm. Sum phải biểu diễn chính xác target, mỗi
// phần tử sau chuyển kiểu và mọi tổng hai phần tử; chuyển kiểu phải giữ thứ tự.
// Trả về một cặp chỉ số 0-based khác nhau, hoặc nullopt nếu không tồn tại.
template <class T, class Sum>
std::optional<std::pair<std::size_t, std::size_t>>
find_pair_with_sum_sorted(const std::vector<T>& values, const Sum& target) {
    if (values.size() < 2) {
        return std::nullopt;
    }

    std::size_t left = 0;
    std::size_t right = values.size() - 1;

    while (left < right) {
        const Sum sum =
            static_cast<Sum>(values[left]) + static_cast<Sum>(values[right]);

        if (sum == target) {
            return std::pair<std::size_t, std::size_t>{left, right};
        }
        if (sum < target) {
            ++left;
        } else {
            --right;
        }
    }

    return std::nullopt;
}

// Sum có cùng hợp đồng biểu diễn như hàm trên. Đếm các cặp chỉ số (i, j),
// i < j, có tổng không vượt quá limit. Kết quả phải vừa trong uint64_t.
template <class T, class Sum>
std::uint64_t count_pairs_with_sum_at_most_sorted(
    const std::vector<T>& values,
    const Sum& limit
) {
    if (values.size() < 2) {
        return 0;
    }

    std::size_t left = 0;
    std::size_t right = values.size() - 1;
    std::uint64_t result = 0;

    while (left < right) {
        const Sum sum =
            static_cast<Sum>(values[left]) + static_cast<Sum>(values[right]);

        if (sum <= limit) {
            result += static_cast<std::uint64_t>(right - left);
            ++left;
        } else {
            --right;
        }
    }

    return result;
}

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_TWO_POINTERS_ALGORITHM_HPP

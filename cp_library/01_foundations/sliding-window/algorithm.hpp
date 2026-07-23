#ifndef CP_LIBRARY_FOUNDATIONS_SLIDING_WINDOW_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_SLIDING_WINDOW_ALGORITHM_HPP

#include <cstddef>

namespace cp_library {

// Trạng thái do callback quản lý phải biểu diễn cửa sổ rỗng trước lời gọi.
// Điều phối mọi cửa sổ [left, right) có độ dài width.
// add/remove nhận chỉ số phần tử; visit nhận hai đầu 0-based, right exclusive.
template <class Add, class Remove, class Visit>
void for_each_fixed_window(
    std::size_t size,
    std::size_t width,
    Add&& add,
    Remove&& remove,
    Visit&& visit
) {
    if (width == 0) {
        for (std::size_t position = 0; position < size; ++position) {
            visit(position, position);
        }
        visit(size, size);
        return;
    }
    if (width > size) {
        return;
    }

    for (std::size_t right = 0; right < size; ++right) {
        if (right >= width) {
            remove(right - width);
        }
        add(right);

        if (right + 1 >= width) {
            visit(right + 1 - width, right + 1);
        }
    }
}

// Trạng thái do callback quản lý phải biểu diễn cửa sổ rỗng trước lời gọi.
// Sau mỗi lần thêm right, bỏ dần đầu trái tới suffix hợp lệ dài nhất.
// Hợp đồng: cửa sổ rỗng hợp lệ và mọi đoạn con của cửa sổ hợp lệ cũng hợp lệ.
template <class Add, class Remove, class IsValid, class Visit>
void for_each_longest_valid_suffix(
    std::size_t size,
    Add&& add,
    Remove&& remove,
    IsValid&& is_valid,
    Visit&& visit
) {
    std::size_t left = 0;

    for (std::size_t right = 0; right < size; ++right) {
        add(right);

        while (left <= right && !is_valid()) {
            remove(left);
            ++left;
        }

        visit(left, right + 1);
    }
}

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_SLIDING_WINDOW_ALGORITHM_HPP

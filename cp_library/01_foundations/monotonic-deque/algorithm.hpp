#ifndef CP_LIBRARY_FOUNDATIONS_MONOTONIC_DEQUE_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_MONOTONIC_DEQUE_ALGORITHM_HPP

#include <cstddef>
#include <deque>
#include <functional>
#include <vector>

namespace cp_library {

// better(a, b) phải là strict weak ordering và mang nghĩa a tốt hơn b.
// Khi hai giá trị tương đương, chỉ số mới hơn được giữ lại.
template <class T, class Better>
std::vector<std::size_t> sliding_window_extreme_indices(
    const std::vector<T>& values,
    std::size_t width,
    Better better
) {
    std::vector<std::size_t> result;
    if (width == 0 || width > values.size()) {
        return result;
    }

    result.reserve(values.size() - width + 1);
    std::deque<std::size_t> candidates;

    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index >= width) {
            const std::size_t first_valid = index - width + 1;
            while (
                !candidates.empty() &&
                candidates.front() < first_valid
            ) {
                candidates.pop_front();
            }
        }

        // candidates.front() là cực trị; giá trị đi từ tốt tới kém.
        // Phần tử cũ không tốt hơn phần tử mới sẽ không còn cần thiết.
        while (
            !candidates.empty() &&
            !better(values[candidates.back()], values[index])
        ) {
            candidates.pop_back();
        }
        candidates.push_back(index);

        if (index + 1 >= width) {
            result.push_back(candidates.front());
        }
    }

    return result;
}

template <class T>
std::vector<std::size_t> sliding_window_min_indices(
    const std::vector<T>& values,
    std::size_t width
) {
    return sliding_window_extreme_indices(values, width, std::less<T>{});
}

template <class T>
std::vector<std::size_t> sliding_window_max_indices(
    const std::vector<T>& values,
    std::size_t width
) {
    return sliding_window_extreme_indices(values, width, std::greater<T>{});
}

template <class T, class Better>
std::vector<T> sliding_window_extreme_values(
    const std::vector<T>& values,
    std::size_t width,
    Better better
) {
    const std::vector<std::size_t> indices =
        sliding_window_extreme_indices(values, width, better);

    std::vector<T> result;
    result.reserve(indices.size());
    for (const std::size_t index : indices) {
        result.push_back(values[index]);
    }
    return result;
}

template <class T>
std::vector<T> sliding_window_min_values(
    const std::vector<T>& values,
    std::size_t width
) {
    return sliding_window_extreme_values(values, width, std::less<T>{});
}

template <class T>
std::vector<T> sliding_window_max_values(
    const std::vector<T>& values,
    std::size_t width
) {
    return sliding_window_extreme_values(values, width, std::greater<T>{});
}

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_MONOTONIC_DEQUE_ALGORITHM_HPP

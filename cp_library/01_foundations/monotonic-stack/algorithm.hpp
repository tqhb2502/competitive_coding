#ifndef CP_LIBRARY_FOUNDATIONS_MONOTONIC_STACK_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_MONOTONIC_STACK_ALGORITHM_HPP

#include <cstddef>
#include <functional>
#include <vector>

namespace cp_library {

enum class MonotonicStrictness {
    Strict,
    NonStrict
};

namespace detail {

template <typename T, typename Compare>
bool shouldPopForLess(const T& candidate,
                      const T& current,
                      MonotonicStrictness strictness,
                      const Compare& less) {
    if (strictness == MonotonicStrictness::Strict) {
        // Candidate phải < current; phần tử bằng nhau cũng bị loại.
        return !less(candidate, current);
    }
    // Candidate được phép <= current; chỉ loại candidate > current.
    return less(current, candidate);
}

template <typename T, typename Compare>
bool shouldPopForGreater(const T& candidate,
                         const T& current,
                         MonotonicStrictness strictness,
                         const Compare& less) {
    if (strictness == MonotonicStrictness::Strict) {
        // Candidate phải > current; phần tử bằng nhau cũng bị loại.
        return !less(current, candidate);
    }
    // Candidate được phép >= current; chỉ loại candidate < current.
    return less(candidate, current);
}

template <typename T, typename ShouldPop>
std::vector<std::size_t> nearestPrevious(const std::vector<T>& values,
                                         ShouldPop should_pop) {
    const std::size_t size = values.size();
    std::vector<std::size_t> answer(size, size);
    std::vector<std::size_t> stack;
    stack.reserve(size);

    for (std::size_t index = 0; index < size; ++index) {
        while (!stack.empty() &&
               should_pop(values[stack.back()], values[index])) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            answer[index] = stack.back();
        }
        stack.push_back(index);
    }
    return answer;
}

template <typename T, typename ShouldPop>
std::vector<std::size_t> nearestNext(const std::vector<T>& values,
                                     ShouldPop should_pop) {
    const std::size_t size = values.size();
    std::vector<std::size_t> answer(size, size);
    std::vector<std::size_t> stack;
    stack.reserve(size);

    for (std::size_t index = size; index-- > 0;) {
        while (!stack.empty() &&
               should_pop(values[stack.back()], values[index])) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            answer[index] = stack.back();
        }
        stack.push_back(index);
    }
    return answer;
}

}  // namespace detail

// Mọi hàm trả về index 0-based. Sentinel values.size() nghĩa là không tồn tại.
// Compare phải tạo một strict weak ordering; mặc định là std::less<T>.

template <typename T, typename Compare = std::less<T>>
std::vector<std::size_t> nearestPreviousLess(
    const std::vector<T>& values,
    MonotonicStrictness strictness,
    Compare less = Compare{}) {
    return detail::nearestPrevious(
        values, [strictness, less](const T& candidate, const T& current) {
            return detail::shouldPopForLess(candidate, current, strictness, less);
        });
}

template <typename T, typename Compare = std::less<T>>
std::vector<std::size_t> nearestNextLess(
    const std::vector<T>& values,
    MonotonicStrictness strictness,
    Compare less = Compare{}) {
    return detail::nearestNext(
        values, [strictness, less](const T& candidate, const T& current) {
            return detail::shouldPopForLess(candidate, current, strictness, less);
        });
}

template <typename T, typename Compare = std::less<T>>
std::vector<std::size_t> nearestPreviousGreater(
    const std::vector<T>& values,
    MonotonicStrictness strictness,
    Compare less = Compare{}) {
    return detail::nearestPrevious(
        values, [strictness, less](const T& candidate, const T& current) {
            return detail::shouldPopForGreater(candidate, current, strictness,
                                               less);
        });
}

template <typename T, typename Compare = std::less<T>>
std::vector<std::size_t> nearestNextGreater(
    const std::vector<T>& values,
    MonotonicStrictness strictness,
    Compare less = Compare{}) {
    return detail::nearestNext(
        values, [strictness, less](const T& candidate, const T& current) {
            return detail::shouldPopForGreater(candidate, current, strictness,
                                               less);
        });
}

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_MONOTONIC_STACK_ALGORITHM_HPP

#ifndef CP_LIBRARY_FOUNDATIONS_BINARY_SEARCH_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_BINARY_SEARCH_ALGORITHM_HPP

#include <cassert>
#include <type_traits>

namespace cp_library {

template <typename Integer>
struct BinarySearchBoundary {
    Integer last_false;
    Integer first_true;
};

namespace detail {

// Trả về một giá trị nằm nghiêm ngặt giữa low và high khi chúng chưa kề nhau.
// Nhánh khác dấu tránh cả high - low lẫn low + high bị tràn số.
template <typename Integer>
constexpr Integer safeIntegerMidpoint(Integer low, Integer high) noexcept {
    static_assert(std::is_integral<Integer>::value,
                  "Binary search requires an integral type.");
    static_assert(!std::is_same<typename std::remove_cv<Integer>::type, bool>::value,
                  "bool is not a valid binary-search domain.");

    if constexpr (std::is_signed<Integer>::value) {
        if (low < Integer{0} && high >= Integer{0}) {
            return static_cast<Integer>(low / Integer{2} + high / Integer{2});
        }
    }
    return static_cast<Integer>(low + (high - low) / Integer{2});
}

}  // namespace detail

// Điều kiện:
// - false_sentinel < true_sentinel;
// - is_true(false_sentinel) = false, is_true(true_sentinel) = true;
// - is_true đơn điệu: một prefix false theo sau bởi một suffix true.
//
// Hai sentinel luôn thuộc miền của predicate. Hàm không tự gọi predicate tại
// sentinel, nhờ đó mỗi vòng lặp chỉ tốn đúng một lần đánh giá.
template <typename Integer, typename Predicate>
BinarySearchBoundary<Integer> binarySearchBoundary(Integer false_sentinel,
                                                    Integer true_sentinel,
                                                    Predicate is_true) {
    static_assert(std::is_integral<Integer>::value,
                  "Binary search requires an integral type.");
    static_assert(!std::is_same<typename std::remove_cv<Integer>::type, bool>::value,
                  "bool is not a valid binary-search domain.");
    assert(false_sentinel < true_sentinel);

    // false_sentinel + 1 an toàn vì false_sentinel < true_sentinel, nên
    // false_sentinel chưa thể là giá trị lớn nhất của Integer.
    while (static_cast<Integer>(false_sentinel + Integer{1}) != true_sentinel) {
        const Integer middle =
            detail::safeIntegerMidpoint(false_sentinel, true_sentinel);
        if (is_true(middle)) {
            true_sentinel = middle;
        } else {
            false_sentinel = middle;
        }
    }
    return {false_sentinel, true_sentinel};
}

template <typename Integer, typename Predicate>
Integer binarySearchFirstTrue(Integer false_sentinel,
                              Integer true_sentinel,
                              Predicate is_true) {
    return binarySearchBoundary(false_sentinel, true_sentinel, is_true).first_true;
}

template <typename Integer, typename Predicate>
Integer binarySearchLastFalse(Integer false_sentinel,
                              Integer true_sentinel,
                              Predicate is_true) {
    return binarySearchBoundary(false_sentinel, true_sentinel, is_true).last_false;
}

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_BINARY_SEARCH_ALGORITHM_HPP

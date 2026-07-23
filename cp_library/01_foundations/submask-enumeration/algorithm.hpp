#ifndef CP_LIBRARY_FOUNDATIONS_SUBMASK_ENUMERATION_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_SUBMASK_ENUMERATION_ALGORITHM_HPP

#include <cstddef>
#include <limits>
#include <type_traits>

namespace cp_library {

namespace detail {

template <typename Mask>
constexpr void requireUnsignedMask() {
    static_assert(std::is_integral<Mask>::value,
                  "A bitmask must have an integral type.");
    static_assert(std::is_unsigned<Mask>::value,
                  "Submask enumeration requires an unsigned type.");
    static_assert(!std::is_same<typename std::remove_cv<Mask>::type, bool>::value,
                  "bool is not a valid bitmask type.");
}

}  // namespace detail

// Trả về 0 nếu position nằm ngoài số bit giá trị của Mask. Nhánh dịch chỉ được
// đánh giá sau khi đã kiểm tra position, nên không có shift với độ rộng quá lớn.
template <typename Mask>
constexpr Mask singleBitOrZero(std::size_t position) noexcept {
    detail::requireUnsignedMask<Mask>();
    return position < static_cast<std::size_t>(std::numeric_limits<Mask>::digits)
               ? static_cast<Mask>(Mask{1} << position)
               : Mask{0};
}

template <typename Mask>
constexpr bool isSubmask(Mask submask, Mask mask) noexcept {
    detail::requireUnsignedMask<Mask>();
    return (submask & static_cast<Mask>(~mask)) == Mask{0};
}

// Gọi visitor lần lượt với mọi submask theo thứ tự số học giảm dần, gồm cả 0.
// Với mask == 0, visitor được gọi đúng một lần với giá trị 0.
template <typename Mask, typename Visitor>
void forEachSubmask(Mask mask, Visitor&& visitor) {
    detail::requireUnsignedMask<Mask>();

    Mask submask = mask;
    while (true) {
        visitor(submask);
        if (submask == Mask{0}) {
            break;
        }
        submask = static_cast<Mask>((submask - Mask{1}) & mask);
    }
}

// Phiên bản không thăm 0. Với mask == 0, visitor không được gọi.
template <typename Mask, typename Visitor>
void forEachNonzeroSubmask(Mask mask, Visitor&& visitor) {
    detail::requireUnsignedMask<Mask>();

    for (Mask submask = mask; submask != Mask{0};
         submask = static_cast<Mask>((submask - Mask{1}) & mask)) {
        visitor(submask);
    }
}

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_SUBMASK_ENUMERATION_ALGORITHM_HPP

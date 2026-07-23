#ifndef CP_LIBRARY_FOUNDATIONS_DIFFERENCE_ARRAY_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_DIFFERENCE_ARRAY_ALGORITHM_HPP

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace cp_library {

template <typename T = long long>
class DifferenceArray {
    static_assert(std::is_arithmetic<T>::value && std::is_signed<T>::value,
                  "DifferenceArray requires a signed arithmetic type");

    static std::size_t storageSize(std::size_t size) {
        if (size == std::numeric_limits<std::size_t>::max()) {
            throw std::length_error(
                "DifferenceArray: size is too large for the sentinel"
            );
        }
        return size + 1;
    }

public:
    DifferenceArray() : difference_(1, T{}) {}

    explicit DifferenceArray(std::size_t size)
        : difference_(storageSize(size), T{}) {}

    explicit DifferenceArray(const std::vector<T>& values) {
        assign(values);
    }

    DifferenceArray(const DifferenceArray&) = default;
    DifferenceArray& operator=(const DifferenceArray&) = default;

    // Object nguồn trở thành một DifferenceArray rỗng nhưng vẫn dùng lại được.
    DifferenceArray(DifferenceArray&& other) : difference_(1, T{}) {
        difference_.swap(other.difference_);
    }

    DifferenceArray& operator=(DifferenceArray&& other) {
        if (this != &other) {
            std::vector<T> empty_difference(1, T{});
            difference_.swap(other.difference_);
            other.difference_.swap(empty_difference);
        }
        return *this;
    }

    // Đặt lại thành một mảng gồm size giá trị 0.
    void reset(std::size_t size) {
        difference_.assign(storageSize(size), T{});
    }

    // Mã hóa cả sentinel: difference_[i] = a[i] - a[i - 1],
    // với a[-1] = a[n] = 0.
    void assign(const std::vector<T>& values) {
        difference_.assign(values.size() + 1, T{});
        if (values.empty()) {
            return;
        }

        difference_[0] = values[0];
        for (std::size_t i = 1; i < values.size(); ++i) {
            difference_[i] = values[i] - values[i - 1];
        }
        difference_[values.size()] = T{} - values.back();
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return difference_.size() - 1;
    }

    // Cộng delta vào đoạn 0-based, nửa mở [left, right).
    void add(std::size_t left, std::size_t right, T delta) {
        if (left > right || right > size()) {
            throw std::out_of_range(
                "DifferenceArray::add: invalid half-open range"
            );
        }
        if (left == right) {
            return;
        }

        difference_[left] += delta;
        difference_[right] -= delta;
    }

    // Khôi phục trạng thái hiện tại mà không làm thay đổi object.
    [[nodiscard]] std::vector<T> materialize() const {
        std::vector<T> values(size());
        T running = T{};
        for (std::size_t i = 0; i < size(); ++i) {
            running += difference_[i];
            values[i] = running;
        }
        return values;
    }

private:
    std::vector<T> difference_;
};

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_DIFFERENCE_ARRAY_ALGORITHM_HPP

#ifndef CP_LIBRARY_FOUNDATIONS_COORDINATE_COMPRESSION_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_COORDINATE_COMPRESSION_ALGORITHM_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cp_library {

// Compare phải là strict weak ordering và gọi được trên một object const.
template <typename T, typename Compare = std::less<T>>
class CoordinateCompression {
public:
    explicit CoordinateCompression(Compare compare = Compare{})
        : compare_(std::move(compare)) {}

    explicit CoordinateCompression(std::vector<T> values,
                                   Compare compare = Compare{})
        : compare_(std::move(compare)) {
        assign(std::move(values));
    }

    // Giữ đúng một đại diện cho mỗi lớp tương đương theo Compare.
    void assign(std::vector<T> values) {
        std::sort(values.begin(), values.end(), compare_);
        values.erase(
            std::unique(
                values.begin(), values.end(),
                [this](const T& left, const T& right) {
                    return equivalent(left, right);
                }
            ),
            values.end()
        );
        values_ = std::move(values);
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return values_.size();
    }

    [[nodiscard]] bool empty() const noexcept {
        return values_.empty();
    }

    // Trả về rank đầu tiên không đứng trước value theo Compare.
    [[nodiscard]] std::size_t lowerBoundIndex(const T& value) const {
        return static_cast<std::size_t>(
            std::lower_bound(
                values_.begin(), values_.end(), value, compare_
            ) - values_.begin()
        );
    }

    // Trả về rank đầu tiên đứng sau value theo Compare.
    [[nodiscard]] std::size_t upperBoundIndex(const T& value) const {
        return static_cast<std::size_t>(
            std::upper_bound(
                values_.begin(), values_.end(), value, compare_
            ) - values_.begin()
        );
    }

    [[nodiscard]] bool contains(const T& value) const {
        const std::size_t index = lowerBoundIndex(value);
        return index < size() && equivalent(values_[index], value);
    }

    // Chỉ nhận giá trị đã xuất hiện lúc assign; rank nằm trong [0, size()).
    [[nodiscard]] std::size_t indexOf(const T& value) const {
        const std::size_t index = lowerBoundIndex(value);
        if (index == size() || !equivalent(values_[index], value)) {
            throw std::invalid_argument(
                "CoordinateCompression::indexOf: value is not registered"
            );
        }
        return index;
    }

    [[nodiscard]] const T& valueAt(std::size_t index) const {
        if (index >= size()) {
            throw std::out_of_range(
                "CoordinateCompression::valueAt: index is out of range"
            );
        }
        return values_[index];
    }

    [[nodiscard]] std::vector<std::size_t> compress(
        const std::vector<T>& input
    ) const {
        std::vector<std::size_t> result;
        result.reserve(input.size());
        for (const T& value : input) {
            result.push_back(indexOf(value));
        }
        return result;
    }

    [[nodiscard]] const std::vector<T>& values() const noexcept {
        return values_;
    }

private:
    [[nodiscard]] bool equivalent(const T& left, const T& right) const {
        return !compare_(left, right) && !compare_(right, left);
    }

    Compare compare_;
    std::vector<T> values_;
};

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_COORDINATE_COMPRESSION_ALGORITHM_HPP

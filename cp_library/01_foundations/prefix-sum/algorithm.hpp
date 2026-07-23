#ifndef CP_LIBRARY_FOUNDATIONS_PREFIX_SUM_ALGORITHM_HPP
#define CP_LIBRARY_FOUNDATIONS_PREFIX_SUM_ALGORITHM_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace cp_library {

template <typename Sum = long long>
class PrefixSum {
public:
    PrefixSum() : prefix_(1, Sum{}) {}

    template <typename Value>
    explicit PrefixSum(const std::vector<Value>& values) {
        assign(values);
    }

    PrefixSum(const PrefixSum&) = default;
    PrefixSum& operator=(const PrefixSum&) = default;

    // Object nguồn trở thành một PrefixSum rỗng nhưng vẫn dùng lại được.
    PrefixSum(PrefixSum&& other) : prefix_(1, Sum{}) {
        prefix_.swap(other.prefix_);
    }

    PrefixSum& operator=(PrefixSum&& other) {
        if (this != &other) {
            std::vector<Sum> empty_prefix(1, Sum{});
            prefix_.swap(other.prefix_);
            other.prefix_.swap(empty_prefix);
        }
        return *this;
    }

    // prefix_[i] luôn là tổng của đúng i phần tử đầu tiên.
    template <typename Value>
    void assign(const std::vector<Value>& values) {
        prefix_.assign(values.size() + 1, Sum{});
        for (std::size_t i = 0; i < values.size(); ++i) {
            prefix_[i + 1] = prefix_[i] + static_cast<Sum>(values[i]);
        }
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return prefix_.size() - 1;
    }

    // Trả về tổng trên [0, end).
    [[nodiscard]] Sum prefix(std::size_t end) const {
        if (end > size()) {
            throw std::out_of_range("PrefixSum::prefix: end is out of range");
        }
        return prefix_[end];
    }

    // Trả về tổng trên đoạn 0-based, nửa mở [left, right).
    [[nodiscard]] Sum query(std::size_t left, std::size_t right) const {
        if (left > right || right > size()) {
            throw std::out_of_range("PrefixSum::query: invalid half-open range");
        }
        return prefix_[right] - prefix_[left];
    }

private:
    std::vector<Sum> prefix_;
};

}  // namespace cp_library

#endif  // CP_LIBRARY_FOUNDATIONS_PREFIX_SUM_ALGORITHM_HPP

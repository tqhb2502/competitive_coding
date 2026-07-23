#ifndef CP_LIBRARY_EXAMPLE_ENTRY_ALGORITHM_HPP
#define CP_LIBRARY_EXAMPLE_ENTRY_ALGORITHM_HPP

#include <vector>

namespace cp_library {

// Hàm mẫu giúp entry biên dịch ngay; thay bằng thuật toán thật khi tạo entry mới.
inline long long sumValues(const std::vector<long long>& values) {
    long long result = 0;
    for (const long long value : values) {
        result += value;
    }
    return result;
}

}  // namespace cp_library

#endif  // CP_LIBRARY_EXAMPLE_ENTRY_ALGORITHM_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

namespace {

// Khóa vector chứa hiệu prefix count của mỗi ký tự so với ký tự mốc.
using Key = std::array<int, 26>;

// Hàm băm cho khóa vector (trộn từng phần tử theo kiểu splitmix64).
struct KeyHash {
    std::size_t operator()(const Key& key) const noexcept {
        std::uint64_t hash = 0x9e3779b97f4a7c15ULL;
        for (const int value : key) {
            std::uint64_t x = static_cast<std::uint32_t>(value);
            x += 0x9e3779b97f4a7c15ULL;
            x = (x ^ (x >> 30U)) * 0xbf58476d1ce4e5b9ULL;
            x = (x ^ (x >> 27U)) * 0x94d049bb133111ebULL;
            x ^= x >> 31U;
            hash ^= x + 0x9e3779b97f4a7c15ULL + (hash << 6U) + (hash >> 2U);
        }
        return static_cast<std::size_t>(hash);
    }
};

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;

    // Nén các ký tự phân biệt thành chỉ số 0..alphabetSize-1.
    std::array<int, 26> index{};
    index.fill(-1);
    int alphabetSize = 0;
    for (const char ch : text) {
        const int letter = ch - 'a';
        if (index[static_cast<std::size_t>(letter)] == -1) {
            index[static_cast<std::size_t>(letter)] = alphabetSize++;
        }
    }

    // counts: số lần xuất hiện của mỗi ký tự trong prefix hiện tại.
    // frequency: đếm số prefix đã gặp có cùng khóa vector.
    Key counts{};
    Key key{};
    std::unordered_map<Key, int, KeyHash> frequency;
    frequency.reserve(text.size() * 2U + 1U);
    frequency.max_load_factor(0.7F);
    frequency[key] = 1;  // Khóa của prefix rỗng.

    long long answer = 0;
    for (const char ch : text) {
        // Cập nhật bộ đếm của ký tự vừa duyệt.
        const int letter = ch - 'a';
        const int compressed = index[static_cast<std::size_t>(letter)];
        ++counts[static_cast<std::size_t>(compressed)];

        // Dựng khóa: hiệu prefix count của từng ký tự so với ký tự mốc 0.
        key.fill(0);
        for (int c = 1; c < alphabetSize; ++c) {
            key[static_cast<std::size_t>(c - 1)] =
                counts[static_cast<std::size_t>(c)] - counts[0];
        }

        // Mỗi prefix trước đó có cùng khóa tạo một substring đặc biệt kết thúc ở đây.
        const auto iterator = frequency.find(key);
        if (iterator != frequency.end()) {
            answer += iterator->second;
            ++iterator->second;
        } else {
            frequency.emplace(key, 1);
        }
    }

    std::cout << answer << '\n';
    return 0;
}

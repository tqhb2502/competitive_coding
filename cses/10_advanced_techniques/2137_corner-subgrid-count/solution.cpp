#include <cstdint>
#include <cstdio>

namespace {

constexpr int MAX_N = 3000;
constexpr int WORD_BITS = 64;
constexpr int WORDS = (MAX_N + WORD_BITS - 1) / WORD_BITS;
constexpr int STRIDE = ((WORDS + 7) / 8) * 8;

// STRIDE is 48 words, so every row begins at a 64-byte boundary.
alignas(64) std::uint64_t rows[MAX_N][STRIDE]{};

__attribute__((target("popcnt")))
std::int64_t count_subgrids(const int n) {
    std::int64_t answer = 0;
    for (int first_row = 0; first_row < n; ++first_row) {
        const std::uint64_t* const first = rows[first_row];
        for (int second_row = first_row + 1; second_row < n; ++second_row) {
            const std::uint64_t* const second = rows[second_row];
            int common = 0;
#pragma GCC unroll 64
            for (int word = 0; word < WORDS; ++word) {
                common +=
                    __builtin_popcountll(first[word] & second[word]);
            }
            answer += static_cast<std::int64_t>(common) * (common - 1) / 2;
        }
    }
    return answer;
}

}  // namespace

int main() {
    int n;
    if (std::scanf("%d", &n) != 1) {
        return 0;
    }

    char input[MAX_N + 1];
    for (int row = 0; row < n; ++row) {
        std::scanf("%3000s", input);
        for (int column = 0; column < n; ++column) {
            rows[row][column / WORD_BITS] |=
                std::uint64_t(input[column] == '1')
                << (column % WORD_BITS);
        }
    }

    std::printf("%lld\n", static_cast<long long>(count_subgrids(n)));
    return 0;
}

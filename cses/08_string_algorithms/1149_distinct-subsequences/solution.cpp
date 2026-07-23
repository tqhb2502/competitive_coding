#include <array>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr long long MOD = 1'000'000'007LL;
    std::string s;
    std::cin >> s;

    // dp: số subsequence phân biệt tính cả chuỗi rỗng; khởi tạo chỉ có chuỗi rỗng.
    long long subsequences = 1;
    // previous_count[c] = giá trị dp ngay trước lần xuất hiện gần nhất của c (dp[j-1]).
    std::array<long long, 26> previous_count{};
    std::array<bool, 26> seen{};

    for (const char character : s) {
        const std::size_t letter = static_cast<std::size_t>(character - 'a');
        // Gấp đôi: mỗi chuỗi cũ có thể giữ nguyên hoặc gắn thêm ký tự hiện tại.
        long long next = 2 * subsequences % MOD;
        // Trừ phần bị đếm trùng do ký tự này đã từng xuất hiện trước đó.
        if (seen[letter]) {
            next = (next - previous_count[letter] + MOD) % MOD;
        }
        previous_count[letter] = subsequences;
        seen[letter] = true;
        subsequences = next;
    }

    // Trừ 1 để loại chuỗi rỗng, cộng MOD giữ kết quả không âm.
    std::cout << (subsequences - 1 + MOD) % MOD << '\n';
    return 0;
}

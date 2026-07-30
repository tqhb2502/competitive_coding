#include <iostream>
#include <cstddef>
#include <unordered_set>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // P-position cuối cùng của trò chơi này là 1222; tính g(k) vượt qua ngưỡng
    // đó một chút là đủ để tái tạo toàn bộ P-position mà không cần hard-code.
    constexpr std::size_t LIMIT = 1300;
    std::vector<int> grundy(LIMIT + 1, 0);
    // Tính giá trị Grundy theo công thức truy hồi Sprague-Grundy.
    for (std::size_t coins = 3; coins <= LIMIT; ++coins) {
        // Tập các nimber đạt được khi tách đống coins thành left và coins-left
        // (left < coins-left để bảo đảm hai đống khác kích thước).
        std::unordered_set<int> reachable;
        for (std::size_t left = 1; left < coins - left; ++left) {
            reachable.insert(grundy[left] ^ grundy[coins - left]);
        }

        // g(coins) = mex của tập reachable (số nguyên không âm nhỏ nhất vắng mặt).
        int mex = 0;
        while (reachable.count(mex) != 0U) {
            ++mex;
        }
        grundy[coins] = mex;
    }

    int tests;
    std::cin >> tests;
    while (tests-- > 0) {
        std::size_t coins;
        std::cin >> coins;
        // g(n)==0 (chỉ có thể xảy ra khi n <= LIMIT) là P-position: người đi
        // sẽ thua nên đáp án là "second"; ngược lại "first".
        const bool losing = coins <= LIMIT && grundy[coins] == 0;
        std::cout << (losing ? "second" : "first") << '\n';
    }
    return 0;
}

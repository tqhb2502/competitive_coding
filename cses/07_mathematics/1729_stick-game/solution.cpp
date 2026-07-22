#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n, k;
    std::cin >> n >> k;
    std::vector<std::size_t> moves(k);
    for (std::size_t& move : moves) {
        std::cin >> move;
    }

    const std::size_t largest_move =
        *std::max_element(moves.begin(), moves.end());
    std::vector<unsigned char> winning(n + largest_move + 1, 0);
    std::string answer(n, 'L');

    // A losing state makes every state that can move to it winning.
    for (std::size_t sticks = 0; sticks <= n; ++sticks) {
        if (winning[sticks] != 0) {
            if (sticks > 0) {
                answer[sticks - 1] = 'W';
            }
            continue;
        }
        for (const std::size_t move : moves) {
            winning[sticks + move] = 1;
        }
    }

    std::cout << answer << '\n';
    return 0;
}

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

    // Nới rộng thêm largest_move phần tử để khi đánh dấu winning[sticks + move]
    // luôn hợp lệ, khỏi cần kiểm tra biên. Mặc định 0 nghĩa là thế THUA.
    const std::size_t largest_move =
        *std::max_element(moves.begin(), moves.end());
    std::vector<unsigned char> winning(n + largest_move + 1, 0);
    std::string answer(n, 'L');

    // Lan truyền tiến: duyệt tăng dần nên khi tới sticks mọi thế thua j < sticks
    // đã đánh dấu xong các thế thắng của nó.
    for (std::size_t sticks = 0; sticks <= n; ++sticks) {
        if (winning[sticks] != 0) {
            // Thế THẮNG: đã có nước đi đưa đối thủ về một thế thua trước đó.
            if (sticks > 0) {
                answer[sticks - 1] = 'W';
            }
            continue;
        }
        // Thế THUA: mọi trạng thái bốc được để quay về sticks đều là thế THẮNG.
        for (const std::size_t move : moves) {
            winning[sticks + move] = 1;
        }
    }

    std::cout << answer << '\n';
    return 0;
}

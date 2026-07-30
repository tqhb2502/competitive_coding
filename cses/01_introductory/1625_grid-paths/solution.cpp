#include <array>
#include <iostream>
#include <string>

using namespace std;

namespace {

constexpr int WIDTH = 9;            // lưới đệm 9x9: viền ngoài luôn "bị chặn"
constexpr int TOTAL_STEPS = 48;     // chuỗi mô tả có 48 bước (đi qua 49 ô)
constexpr int START = WIDTH + 1;    // ô thực (hàng 1, cột 1) = góc trên-trái
constexpr int TARGET = 7 * WIDTH + 1;  // ô thực (hàng 7, cột 1) = góc dưới-trái

// Mã hóa 4 hướng D/U/L/R thành độ lệch chỉ số trên lưới đệm.
const array<int, 4> DELTA{WIDTH, -WIDTH, -1, 1};
const array<char, 4> DIRECTION{'D', 'U', 'L', 'R'};

string description;
array<bool, WIDTH * WIDTH> occupied{};  // đánh dấu ô đã thăm / bị chặn
int answer = 0;

// Quay lui (DFS): đi theo từng bước của chuỗi mô tả, đếm số đường Hamilton hợp lệ.
void dfs(int position, int step) {
    // Cắt theo đích đến: chỉ hợp lệ khi tới ô đích đúng ở bước cuối (step == 48);
    // tới sớm hơn là ngõ cụt vì ô đích chỉ được thăm 1 lần.
    if (position == TARGET) {
        if (step == TOTAL_STEPS) {
            ++answer;
        }
        return;
    }
    if (step == TOTAL_STEPS) {
        return;
    }

    // Trạng thái 4 ô kề: true nghĩa là "bị chặn" (tường biên hoặc đã thăm).
    const bool up = occupied[position - WIDTH];
    const bool down = occupied[position + WIDTH];
    const bool left = occupied[position - 1];
    const bool right = occupied[position + 1];

    // Cắt theo "chia đôi lưới": nếu buộc phải rẽ ngang một hành lang mà hai đầu
    // đều bị chặn thì lưới bị cắt rời thành hai phần vẫn còn ô chưa thăm -> dừng.
    if ((up && down && !left && !right) ||
        (left && right && !up && !down)) {
        return;
    }

    // Ở bước này: nếu là hướng cụ thể chỉ thử hướng đó, nếu '?' thử cả 4 hướng.
    const char requiredDirection = description[step];
    for (int direction = 0; direction < 4; ++direction) {
        if (requiredDirection != '?' &&
            requiredDirection != DIRECTION[direction]) {
            continue;
        }

        // Chỉ đi sang ô chưa thăm; đánh dấu, đệ quy rồi bỏ đánh dấu khi quay lui.
        const int next = position + DELTA[direction];
        if (occupied[next]) {
            continue;
        }

        occupied[next] = true;
        dfs(next, step + 1);
        occupied[next] = false;
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> description;

    // Đánh dấu sẵn toàn bộ viền ngoài 9x9 là "bị chặn" để khỏi kiểm tra biên.
    for (int index = 0; index < WIDTH; ++index) {
        occupied[index] = true;
        occupied[(WIDTH - 1) * WIDTH + index] = true;
        occupied[index * WIDTH] = true;
        occupied[index * WIDTH + WIDTH - 1] = true;
    }
    occupied[START] = true;  // ô xuất phát coi như đã thăm

    dfs(START, 0);
    cout << answer << '\n';
    return 0;
}

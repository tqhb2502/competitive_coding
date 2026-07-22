#include <array>
#include <iostream>
#include <string>

using namespace std;

namespace {

constexpr int WIDTH = 9;
constexpr int TOTAL_STEPS = 48;
constexpr int START = WIDTH + 1;
constexpr int TARGET = 7 * WIDTH + 1;

const array<int, 4> DELTA{WIDTH, -WIDTH, -1, 1};
const array<char, 4> DIRECTION{'D', 'U', 'L', 'R'};

string description;
array<bool, WIDTH * WIDTH> occupied{};
int answer = 0;

void dfs(int position, int step) {
    if (position == TARGET) {
        if (step == TOTAL_STEPS) {
            ++answer;
        }
        return;
    }
    if (step == TOTAL_STEPS) {
        return;
    }

    const bool up = occupied[position - WIDTH];
    const bool down = occupied[position + WIDTH];
    const bool left = occupied[position - 1];
    const bool right = occupied[position + 1];

    // Continuing sideways or vertically here would split the unvisited cells.
    if ((up && down && !left && !right) ||
        (left && right && !up && !down)) {
        return;
    }

    const char requiredDirection = description[step];
    for (int direction = 0; direction < 4; ++direction) {
        if (requiredDirection != '?' &&
            requiredDirection != DIRECTION[direction]) {
            continue;
        }

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

    for (int index = 0; index < WIDTH; ++index) {
        occupied[index] = true;
        occupied[(WIDTH - 1) * WIDTH + index] = true;
        occupied[index * WIDTH] = true;
        occupied[index * WIDTH + WIDTH - 1] = true;
    }
    occupied[START] = true;

    dfs(START, 0);
    cout << answer << '\n';
    return 0;
}

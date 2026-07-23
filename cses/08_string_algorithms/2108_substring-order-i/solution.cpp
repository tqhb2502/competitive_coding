#include <array>
#include <iostream>
#include <string>
#include <vector>

// Một trạng thái của suffix automaton (SAM).
struct State {
    int length = 0;                 // độ dài xâu dài nhất kết thúc tại trạng thái này
    int link = -1;                  // suffix link
    std::array<int, 26> next{};     // các transition theo từng ký tự

    State() {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    long long rank;
    std::cin >> text >> rank;

    // Xây dựng suffix automaton của xâu s bằng thuật toán online chuẩn.
    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();
    int last = 0;

    for (const char character : text) {
        const int letter = character - 'a';
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;

        // Đi ngược theo suffix link, thêm transition tới trạng thái mới.
        int state = last;
        while (state != -1 && automaton[state].next[letter] == -1) {
            automaton[state].next[letter] = current;
            state = automaton[state].link;
        }
        if (state == -1) {
            automaton[current].link = 0;
        } else {
            const int child = automaton[state].next[letter];
            if (automaton[state].length + 1 == automaton[child].length) {
                automaton[current].link = child;
            } else {
                // Nhân bản (clone) trạng thái child để giữ tính chất của SAM.
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[child]);
                automaton[clone].length = automaton[state].length + 1;
                while (state != -1 &&
                       automaton[state].next[letter] == child) {
                    automaton[state].next[letter] = clone;
                    state = automaton[state].link;
                }
                automaton[child].link = clone;
                automaton[current].link = clone;
            }
        }
        last = current;
    }

    // Sắp xếp các trạng thái theo len TĂNG dần bằng counting sort;
    // khi duyệt ngược sẽ được thứ tự len GIẢM dần (thứ tự topo hợp lệ cho DP).
    const int state_count = static_cast<int>(automaton.size());
    std::vector<int> bucket(text.size() + 1, 0);
    for (const State& state : automaton) {
        ++bucket[static_cast<std::size_t>(state.length)];
    }
    for (std::size_t length = 1; length < bucket.size(); ++length) {
        bucket[length] += bucket[length - 1];
    }
    std::vector<int> order(static_cast<std::size_t>(state_count));
    for (int state = state_count - 1; state >= 0; --state) {
        const int length = automaton[state].length;
        order[static_cast<std::size_t>(--bucket[static_cast<std::size_t>(length)])] =
            state;
    }

    // DP đếm số xâu con phân biệt sinh ra khi rời mỗi trạng thái:
    // paths[v] = tổng theo mọi transition (c -> u) của (1 + paths[u]).
    std::vector<long long> paths(static_cast<std::size_t>(state_count), 0);
    for (int index = state_count - 1; index >= 0; --index) {
        const int state = order[static_cast<std::size_t>(index)];
        long long total = 0;
        for (const int child : automaton[state].next) {
            if (child != -1) {
                total += 1 + paths[static_cast<std::size_t>(child)];
            }
        }
        paths[static_cast<std::size_t>(state)] = total;
    }

    // Truy vết greedy từ root: tại mỗi trạng thái xét ký tự theo thứ tự 'a'..'z'.
    std::string answer;
    int state = 0;
    while (rank > 0) {
        for (int letter = 0; letter < 26; ++letter) {
            const int child = automaton[state].next[letter];
            if (child == -1) {
                continue;
            }
            // Số xâu con trong nhánh theo ký tự này.
            const long long branch =
                1 + paths[static_cast<std::size_t>(child)];
            if (rank > branch) {
                // Bỏ qua trọn nhánh này rồi thử ký tự tiếp theo.
                rank -= branch;
                continue;
            }
            // Chọn ký tự này; giảm rank đi 1 ứng với xâu kết thúc ngay tại đây.
            answer.push_back(static_cast<char>('a' + letter));
            --rank;
            state = child;
            break;
        }
    }

    std::cout << answer << '\n';
    return 0;
}

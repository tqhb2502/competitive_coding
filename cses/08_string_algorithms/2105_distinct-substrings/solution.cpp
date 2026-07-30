#include <array>
#include <iostream>
#include <string>
#include <vector>

// Một trạng thái của suffix automaton (SAM).
struct State {
    int length = 0;                // độ dài xâu con dài nhất của lớp tương đương
    int link = -1;                 // suffix link
    std::array<int, 26> next{};    // các cạnh transitions theo từng chữ cái

    State() {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    if (!(std::cin >> text)) {
        return 0;
    }

    // Khởi tạo SAM với đúng một trạng thái gốc (initial).
    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();
    int last = 0;

    // Thêm lần lượt từng ký tự vào cuối, mở rộng SAM online.
    for (const char character : text) {
        const int letter = character - 'a';

        // Tạo trạng thái mới cho tiền tố hiện tại.
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;

        // Đi theo chuỗi suffix link, gắn cạnh tới trạng thái mới khi còn thiếu.
        int state = last;
        while (state != -1 && automaton[state].next[letter] == -1) {
            automaton[state].next[letter] = current;
            state = automaton[state].link;
        }

        if (state == -1) {
            // Không gặp cạnh nào: suffix link trỏ về gốc.
            automaton[current].link = 0;
        } else {
            const int target = automaton[state].next[letter];
            if (automaton[state].length + 1 == automaton[target].length) {
                // target liền kề, dùng trực tiếp làm suffix link.
                automaton[current].link = target;
            } else {
                // Tách lớp: tạo trạng thái clone sao chép target rồi chỉnh len.
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[target]);
                automaton[clone].length = automaton[state].length + 1;

                // Chuyển hướng các cạnh đang trỏ tới target sang clone.
                while (state != -1 && automaton[state].next[letter] == target) {
                    automaton[state].next[letter] = clone;
                    state = automaton[state].link;
                }
                automaton[target].link = clone;
                automaton[current].link = clone;
            }
        }
        last = current;
    }

    // Mỗi trạng thái v != gốc đóng góp len[v] - len[link[v]] xâu con mới.
    long long distinct_substrings = 0;
    for (std::size_t state = 1; state < automaton.size(); ++state) {
        distinct_substrings += automaton[state].length - automaton[automaton[state].link].length;
    }
    std::cout << distinct_substrings << '\n';

    return 0;
}

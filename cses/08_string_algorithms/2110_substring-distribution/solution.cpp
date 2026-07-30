#include <array>
#include <iostream>
#include <string>
#include <vector>

// Một state của suffix automaton (SAM):
// - length: độ dài xâu con dài nhất mà state biểu diễn
// - link: suffix link
// - next: cạnh chuyển theo từng ký tự a-z
struct State {
    int length = 0;
    int link = -1;
    std::array<int, 26> next{};

    State() {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;

    // Xây dựng SAM online: bắt đầu với state khởi tạo 0
    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();
    int last = 0;

    for (const char character : text) {
        const int letter = character - 'a';

        // Tạo state mới ứng với tiền tố dài thêm 1 ký tự (state dài nhất mới)
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;

        // Đi theo suffix link, thêm cạnh chuyển tới state mới
        int state = last;
        while (state != -1 && automaton[state].next[letter] == -1) {
            automaton[state].next[letter] = current;
            state = automaton[state].link;
        }
        if (state == -1) {
            // Không có cạnh trùng: suffix link trỏ về state khởi tạo
            automaton[current].link = 0;
        } else {
            const int child = automaton[state].next[letter];
            if (automaton[state].length + 1 == automaton[child].length) {
                // Cạnh liên tục: dùng thẳng child làm suffix link
                automaton[current].link = child;
            } else {
                // Tách state (clone) để giữ tính tối thiểu của automaton
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[child]);
                automaton[clone].length = automaton[state].length + 1;
                // Chuyển các cạnh trỏ tới child sang clone
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

    // Mảng hiệu: mỗi state v cộng 1 cho khoảng độ dài [len[link]+1, len[v]]
    std::vector<int> difference(text.size() + 2, 0);
    for (std::size_t state = 1; state < automaton.size(); ++state) {
        const int left =
            automaton[static_cast<std::size_t>(automaton[state].link)].length + 1;
        const int right = automaton[state].length;
        ++difference[static_cast<std::size_t>(left)];
        --difference[static_cast<std::size_t>(right + 1)];
    }

    // Prefix sum: giá trị tại L là số xâu con phân biệt có độ dài L
    int count = 0;
    for (std::size_t length = 1; length <= text.size(); ++length) {
        count += difference[length];
        if (length > 1) {
            std::cout << ' ';
        }
        std::cout << count;
    }
    std::cout << '\n';
    return 0;
}

#include <array>
#include <iostream>
#include <string>
#include <vector>

// Một trạng thái của Suffix Automaton (SAM).
struct State {
    int length = 0;          // độ dài xâu dài nhất thuộc lớp tương đương này
    int link = -1;           // suffix link
    int first_position = 0;  // vị trí kết thúc (0-indexed) sớm nhất -> firstpos
    std::array<int, 26> next{};  // transition theo 26 ký tự

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

    // Xây SAM của xâu s theo từng ký tự (thuật toán online chuẩn CP-algorithms).
    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();  // trạng thái khởi đầu
    int last = 0;

    for (int position = 0; position < static_cast<int>(text.size()); ++position) {
        const int letter = text[position] - 'a';

        // Tạo trạng thái mới cur đại diện cho toàn bộ tiền tố hiện tại.
        // firstpos[cur] = position: vị trí kết thúc lần xuất hiện đầu tiên.
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;
        automaton[current].first_position = position;

        // Nối transition từ chuỗi suffix link của last tới cur.
        int state = last;
        while (state != -1 && automaton[state].next[letter] == -1) {
            automaton[state].next[letter] = current;
            state = automaton[state].link;
        }

        if (state == -1) {
            automaton[current].link = 0;
        } else {
            const int target = automaton[state].next[letter];
            if (automaton[state].length + 1 == automaton[target].length) {
                // target liền kề, gán suffix link trực tiếp.
                automaton[current].link = target;
            } else {
                // Tách (clone) target; clone kế thừa firstpos[target] vì endpos
                // chỉ mở rộng thêm vị trí mới (lớn hơn) nên min không đổi.
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[target]);
                automaton[clone].length = automaton[state].length + 1;

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

    // Trả lời truy vấn: đi từng ký tự của pattern trong SAM.
    int pattern_count = 0;
    std::cin >> pattern_count;
    while (pattern_count-- > 0) {
        std::string pattern;
        std::cin >> pattern;
        int state = 0;
        for (const char character : pattern) {
            state = automaton[state].next[character - 'a'];
            if (state == -1) {  // thiếu transition -> pattern không phải xâu con
                break;
            }
        }
        if (state == -1) {
            std::cout << -1 << '\n';
        } else {
            // Đổi vị trí kết thúc sớm nhất sang vị trí bắt đầu (1-indexed).
            std::cout << automaton[state].first_position - static_cast<int>(pattern.size()) + 2 << '\n';
        }
    }

    return 0;
}

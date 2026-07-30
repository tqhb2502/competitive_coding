#include <array>
#include <iostream>
#include <string>
#include <vector>

// Một state của suffix automaton (SAM): độ dài chuỗi dài nhất trong lớp endpos,
// suffix link, bảng chuyển theo 26 ký tự, và số lần xuất hiện (|endpos|).
struct State {
    int length = 0;
    int link = -1;
    std::array<int, 26> next{};
    long long occurrences = 0;

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

    // Xây dựng suffix automaton của chuỗi s bằng cách thêm từng ký tự.
    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();  // state gốc (root = 0)
    int last = 0;

    for (const char character : text) {
        const int letter = character - 'a';
        // Tạo state gốc "cur" cho prefix mới, gán occurrences = 1 vì nó kết thúc
        // tại một vị trí thật sự trong s.
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;
        automaton[current].occurrences = 1;

        // Đi ngược theo suffix link, nối chuyển tới "cur" cho các state chưa có
        // chuyển theo ký tự này.
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
                // Đã liền mạch: dùng thẳng target làm suffix link của cur.
                automaton[current].link = target;
            } else {
                // Tách state: tạo "clone" của target với len = len(state)+1.
                // clone không tương ứng vị trí kết thúc mới nên occurrences = 0.
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[target]);
                automaton[clone].length = automaton[state].length + 1;
                automaton[clone].occurrences = 0;

                // Chuyển hướng các chuyển đang trỏ tới target sang clone.
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

    // Counting sort các state theo len; mảng order thu được tăng dần theo len.
    std::vector<int> length_count(text.size() + 1, 0);
    for (const State& state : automaton) {
        ++length_count[state.length];
    }
    for (std::size_t length = 1; length < length_count.size(); ++length) {
        length_count[length] += length_count[length - 1];
    }

    std::vector<int> order(automaton.size());
    for (int state = static_cast<int>(automaton.size()) - 1; state >= 0; --state) {
        order[--length_count[automaton[state].length]] = state;
    }
    // Cộng dồn occurrences theo suffix link từ len lớn về nhỏ:
    // occurrences[v] trở thành |endpos(v)| = số lần xuất hiện.
    for (int index = static_cast<int>(order.size()) - 1; index > 0; --index) {
        const int state = order[index];
        automaton[automaton[state].link].occurrences += automaton[state].occurrences;
    }

    // Trả lời truy vấn: đi theo từng ký tự của pattern qua các chuyển của SAM.
    int pattern_count = 0;
    std::cin >> pattern_count;
    while (pattern_count-- > 0) {
        std::string pattern;
        std::cin >> pattern;
        int state = 0;
        for (const char character : pattern) {
            state = automaton[state].next[character - 'a'];
            if (state == -1) {
                break;  // không có chuyển => pattern không phải substring của s
            }
        }
        // Đi hết pattern tới state u thì đáp án = occurrences[u]; tắc thì 0.
        std::cout << (state == -1 ? 0 : automaton[state].occurrences) << '\n';
    }

    return 0;
}

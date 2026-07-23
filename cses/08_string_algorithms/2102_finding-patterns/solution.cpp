#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {

// Mỗi trạng thái của Suffix Automaton: độ dài chuỗi dài nhất, suffix link và
// bảng transition cho 26 chữ cái a..z (-1 nghĩa là không có cạnh).
struct State {
    int length = 0;
    int link = -1;
    std::array<int, 26> next;

    State() {
        next.fill(-1);
    }
};

class SuffixAutomaton {
public:
    // Xây SAM online: thêm lần lượt từng ký tự của text.
    explicit SuffixAutomaton(const std::string& text) {
        states_.reserve(2 * text.size());
        states_.emplace_back();  // state 0 là trạng thái khởi đầu
        int last = 0;
        for (const char character : text) {
            last = extend(last, character - 'a');
        }
    }

    // Chạy pattern trên automaton từ state 0: đọc hết được -> là substring.
    [[nodiscard]] bool contains(const std::string& pattern) const {
        int state = 0;
        for (const char character : pattern) {
            state = states_[state].next[character - 'a'];
            if (state == -1) {  // tắc transition -> không phải substring
                return false;
            }
        }
        return true;
    }

private:
    // Thêm một ký tự mới, trả về trạng thái tương ứng chuỗi hiện tại.
    int extend(const int last, const int letter) {
        const int current = static_cast<int>(states_.size());
        states_.emplace_back();
        states_[current].length = states_[last].length + 1;

        // Đi ngược theo suffix link, nối cạnh về cur ở những nơi còn thiếu.
        int state = last;
        while (state != -1 && states_[state].next[letter] == -1) {
            states_[state].next[letter] = current;
            state = states_[state].link;
        }

        if (state == -1) {  // đi hết chuỗi suffix link -> link về state 0
            states_[current].link = 0;
            return current;
        }

        // Đã có cạnh sẵn tới next_state; nếu liền kề (len khớp) thì nối trực tiếp.
        const int next_state = states_[state].next[letter];
        if (states_[state].length + 1 == states_[next_state].length) {
            states_[current].link = next_state;
            return current;
        }

        // Ngược lại phải clone next_state để tách lớp endpos.
        const int clone = static_cast<int>(states_.size());
        states_.push_back(states_[next_state]);
        states_[clone].length = states_[state].length + 1;

        // Chuyển các cạnh đang trỏ tới next_state sang clone.
        while (state != -1 && states_[state].next[letter] == next_state) {
            states_[state].next[letter] = clone;
            state = states_[state].link;
        }
        states_[next_state].link = clone;
        states_[current].link = clone;
        return current;
    }

    std::vector<State> states_;
};

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    int pattern_count = 0;
    std::cin >> text >> pattern_count;

    const SuffixAutomaton automaton(text);
    // Với mỗi pattern, kiểm tra sự tồn tại rồi in "YES"/"NO".
    for (int index = 0; index < pattern_count; ++index) {
        std::string pattern;
        std::cin >> pattern;
        std::cout << (automaton.contains(pattern) ? "YES" : "NO") << '\n';
    }
    return 0;
}

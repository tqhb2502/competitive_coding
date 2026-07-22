#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {

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
    explicit SuffixAutomaton(const std::string& text) {
        states_.reserve(2 * text.size());
        states_.emplace_back();
        int last = 0;
        for (const char character : text) {
            last = extend(last, character - 'a');
        }
    }

    [[nodiscard]] bool contains(const std::string& pattern) const {
        int state = 0;
        for (const char character : pattern) {
            state = states_[state].next[character - 'a'];
            if (state == -1) {
                return false;
            }
        }
        return true;
    }

private:
    int extend(const int last, const int letter) {
        const int current = static_cast<int>(states_.size());
        states_.emplace_back();
        states_[current].length = states_[last].length + 1;

        int state = last;
        while (state != -1 && states_[state].next[letter] == -1) {
            states_[state].next[letter] = current;
            state = states_[state].link;
        }

        if (state == -1) {
            states_[current].link = 0;
            return current;
        }

        const int next_state = states_[state].next[letter];
        if (states_[state].length + 1 == states_[next_state].length) {
            states_[current].link = next_state;
            return current;
        }

        const int clone = static_cast<int>(states_.size());
        states_.push_back(states_[next_state]);
        states_[clone].length = states_[state].length + 1;

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
    for (int index = 0; index < pattern_count; ++index) {
        std::string pattern;
        std::cin >> pattern;
        std::cout << (automaton.contains(pattern) ? "YES" : "NO") << '\n';
    }
    return 0;
}

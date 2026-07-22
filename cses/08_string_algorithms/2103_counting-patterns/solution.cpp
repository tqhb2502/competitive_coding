#include <array>
#include <iostream>
#include <string>
#include <vector>

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

    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();
    int last = 0;

    for (const char character : text) {
        const int letter = character - 'a';
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;
        automaton[current].occurrences = 1;

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
                automaton[current].link = target;
            } else {
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[target]);
                automaton[clone].length = automaton[state].length + 1;
                automaton[clone].occurrences = 0;

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
    for (int index = static_cast<int>(order.size()) - 1; index > 0; --index) {
        const int state = order[index];
        automaton[automaton[state].link].occurrences += automaton[state].occurrences;
    }

    int pattern_count = 0;
    std::cin >> pattern_count;
    while (pattern_count-- > 0) {
        std::string pattern;
        std::cin >> pattern;
        int state = 0;
        for (const char character : pattern) {
            state = automaton[state].next[character - 'a'];
            if (state == -1) {
                break;
            }
        }
        std::cout << (state == -1 ? 0 : automaton[state].occurrences) << '\n';
    }

    return 0;
}

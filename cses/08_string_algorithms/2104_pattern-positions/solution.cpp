#include <array>
#include <iostream>
#include <string>
#include <vector>

struct State {
    int length = 0;
    int link = -1;
    int first_position = 0;
    std::array<int, 26> next{};

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

    for (int position = 0; position < static_cast<int>(text.size()); ++position) {
        const int letter = text[position] - 'a';
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;
        automaton[current].first_position = position;

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
        if (state == -1) {
            std::cout << -1 << '\n';
        } else {
            std::cout << automaton[state].first_position - static_cast<int>(pattern.size()) + 2 << '\n';
        }
    }

    return 0;
}

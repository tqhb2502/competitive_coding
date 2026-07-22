#include <array>
#include <iostream>
#include <string>
#include <vector>

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

    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();
    int last = 0;

    for (const char character : text) {
        const int letter = character - 'a';
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;

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

    std::vector<int> difference(text.size() + 2, 0);
    for (std::size_t state = 1; state < automaton.size(); ++state) {
        const int left =
            automaton[static_cast<std::size_t>(automaton[state].link)].length + 1;
        const int right = automaton[state].length;
        ++difference[static_cast<std::size_t>(left)];
        --difference[static_cast<std::size_t>(right + 1)];
    }

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

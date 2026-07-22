#include <array>
#include <iostream>
#include <string>
#include <vector>

struct State {
    int length = 0;
    int link = -1;
    long long occurrences = 0;
    std::array<int, 26> next{};

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
            const int child = automaton[state].next[letter];
            if (automaton[state].length + 1 == automaton[child].length) {
                automaton[current].link = child;
            } else {
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[child]);
                automaton[clone].length = automaton[state].length + 1;
                automaton[clone].occurrences = 0;
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

    for (int index = state_count - 1; index > 0; --index) {
        const int state = order[static_cast<std::size_t>(index)];
        const int parent = automaton[state].link;
        automaton[parent].occurrences += automaton[state].occurrences;
    }

    std::vector<long long> continuations(
        static_cast<std::size_t>(state_count), 0
    );
    for (int index = state_count - 1; index >= 0; --index) {
        const int state = order[static_cast<std::size_t>(index)];
        long long total = 0;
        for (const int child : automaton[state].next) {
            if (child != -1) {
                total += automaton[child].occurrences +
                         continuations[static_cast<std::size_t>(child)];
            }
        }
        continuations[static_cast<std::size_t>(state)] = total;
    }

    std::string answer;
    int state = 0;
    while (true) {
        bool descended = false;
        for (int letter = 0; letter < 26; ++letter) {
            const int child = automaton[state].next[letter];
            if (child == -1) {
                continue;
            }
            const long long occurrences = automaton[child].occurrences;
            const long long branch = occurrences +
                continuations[static_cast<std::size_t>(child)];
            if (rank > branch) {
                rank -= branch;
                continue;
            }

            answer.push_back(static_cast<char>('a' + letter));
            if (rank <= occurrences) {
                std::cout << answer << '\n';
                return 0;
            }
            rank -= occurrences;
            state = child;
            descended = true;
            break;
        }
        if (!descended) {
            break;
        }
    }

    return 0;
}

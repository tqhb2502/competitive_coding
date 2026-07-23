#include <array>
#include <iostream>
#include <string>
#include <vector>

// Một state của Suffix Automaton (SAM).
struct State {
    int length = 0;              // độ dài chuỗi dài nhất thuộc state này
    int link = -1;               // suffix link
    long long occurrences = 0;   // cnt: số lần xuất hiện (endpos size)
    std::array<int, 26> next{};  // transition theo từng ký tự a..z

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

    // ---- Xây Suffix Automaton bằng cách thêm từng ký tự ----
    std::vector<State> automaton;
    automaton.reserve(2 * text.size());
    automaton.emplace_back();  // state khởi đầu (rỗng)
    int last = 0;

    for (const char character : text) {
        const int letter = character - 'a';
        const int current = static_cast<int>(automaton.size());
        automaton.emplace_back();
        automaton[current].length = automaton[last].length + 1;
        automaton[current].occurrences = 1;  // state "gốc" -> cnt = 1

        // Đi ngược theo suffix link, thêm transition tới state mới.
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
                // Tách clone khi transition không "liền" về length.
                const int clone = static_cast<int>(automaton.size());
                automaton.push_back(automaton[child]);
                automaton[clone].length = automaton[state].length + 1;
                automaton[clone].occurrences = 0;  // clone -> cnt = 0
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

    // ---- Sắp xếp state theo length tăng dần bằng counting sort ----
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

    // ---- Tính cnt (endpos size): cộng ngược theo suffix link, length giảm dần ----
    for (int index = state_count - 1; index > 0; --index) {
        const int state = order[static_cast<std::size_t>(index)];
        const int parent = automaton[state].link;
        automaton[parent].occurrences += automaton[state].occurrences;
    }

    // ---- Tính g[v] = tổng số phần tử (có lặp) đi tiếp được từ v ----
    // Transition luôn tới length lớn hơn nên duyệt theo length giảm dần.
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

    // ---- Đi tìm substring thứ k theo thứ tự từ điển ----
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
            // Tổng số phần tử đi qua cạnh này = cnt[child] + g[child].
            const long long branch = occurrences +
                continuations[static_cast<std::size_t>(child)];
            if (rank > branch) {
                rank -= branch;  // bỏ qua toàn bộ nhánh, thử ký tự tiếp theo
                continue;
            }

            answer.push_back(static_cast<char>('a' + letter));
            // Chuỗi "hiện tại + c" đứng trước, lặp đúng occurrences lần.
            if (rank <= occurrences) {
                std::cout << answer << '\n';
                return 0;
            }
            rank -= occurrences;  // đi xuống child để mở rộng thêm
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

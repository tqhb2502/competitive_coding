#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int size;
    std::cin >> size;
    std::vector<int> suffix_array(static_cast<std::size_t>(size));
    std::vector<int> rank(static_cast<std::size_t>(size + 2), 0);
    for (int index = 0; index < size; ++index) {
        std::cin >> suffix_array[static_cast<std::size_t>(index)];
        rank[static_cast<std::size_t>(
            suffix_array[static_cast<std::size_t>(index)]
        )] = index + 1;
    }

    std::vector<int> letter(static_cast<std::size_t>(size + 1), 0);
    int current_letter = 0;
    letter[static_cast<std::size_t>(suffix_array[0])] = 0;
    for (int index = 1; index < size; ++index) {
        const int previous = suffix_array[static_cast<std::size_t>(index - 1)];
        const int current = suffix_array[static_cast<std::size_t>(index)];
        if (rank[static_cast<std::size_t>(previous + 1)] >
            rank[static_cast<std::size_t>(current + 1)]) {
            ++current_letter;
        }
        letter[static_cast<std::size_t>(current)] = current_letter;
    }

    if (current_letter >= 26) {
        std::cout << -1 << '\n';
        return 0;
    }

    std::string answer(static_cast<std::size_t>(size), 'a');
    for (int position = 1; position <= size; ++position) {
        answer[static_cast<std::size_t>(position - 1)] = static_cast<char>(
            'a' + letter[static_cast<std::size_t>(position)]
        );
    }
    std::cout << answer << '\n';
    return 0;
}

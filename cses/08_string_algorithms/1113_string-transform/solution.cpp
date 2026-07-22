#include <array>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string last_column;
    std::cin >> last_column;
    const int length = static_cast<int>(last_column.size());

    std::array<int, 256> frequency{};
    for (const unsigned char character : last_column) {
        ++frequency[character];
    }

    std::array<int, 256> first_position{};
    int total = 0;
    for (int character = 0; character < 256; ++character) {
        first_position[static_cast<std::size_t>(character)] = total;
        total += frequency[static_cast<std::size_t>(character)];
    }

    std::array<int, 256> next_position = first_position;
    std::vector<int> lf(static_cast<std::size_t>(length));
    for (int row = 0; row < length; ++row) {
        const unsigned char character =
            static_cast<unsigned char>(last_column[static_cast<std::size_t>(row)]);
        lf[static_cast<std::size_t>(row)] = next_position[character]++;
    }

    std::string restored(static_cast<std::size_t>(length), ' ');
    int row = 0;  // The first sorted rotation starts with the unique '#'.
    for (int output_index = length - 1; output_index >= 0; --output_index) {
        restored[static_cast<std::size_t>(output_index)] =
            last_column[static_cast<std::size_t>(row)];
        row = lf[static_cast<std::size_t>(row)];
    }

    // restored is "#" followed by the original string.
    std::cout << restored.substr(1) << '\n';
    return 0;
}

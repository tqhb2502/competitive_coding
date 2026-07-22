#include <array>
#include <iostream>
#include <string>
#include <vector>

struct Node {
    int length;
    int link;
    std::array<int, 26> next{};

    Node(const int node_length, const int suffix_link)
        : length(node_length), link(suffix_link) {
        next.fill(-1);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::cin >> text;
    const int size = static_cast<int>(text.size());
    std::vector<int> characters(static_cast<std::size_t>(size + 1), -1);
    for (int index = 1; index <= size; ++index) {
        characters[static_cast<std::size_t>(index)] =
            text[static_cast<std::size_t>(index - 1)] - 'a';
    }

    std::vector<Node> tree;
    tree.reserve(text.size() + 2);
    tree.emplace_back(-1, 0);
    tree.emplace_back(0, 0);
    int last = 1;

    for (int index = 1; index <= size; ++index) {
        const int character = characters[static_cast<std::size_t>(index)];
        int current = last;
        while (characters[static_cast<std::size_t>(
                   index - tree[current].length - 1
               )] != character) {
            current = tree[current].link;
        }

        int child = tree[current].next[character];
        if (child == -1) {
            const int new_length = tree[current].length + 2;
            int suffix_link = 1;
            if (new_length > 1) {
                int candidate = tree[current].link;
                while (characters[static_cast<std::size_t>(
                           index - tree[candidate].length - 1
                       )] != character) {
                    candidate = tree[candidate].link;
                }
                suffix_link = tree[candidate].next[character];
            }

            child = static_cast<int>(tree.size());
            tree.emplace_back(new_length, suffix_link);
            tree[current].next[character] = child;
        }
        last = child;

        if (index > 1) {
            std::cout << ' ';
        }
        std::cout << tree[last].length;
    }
    std::cout << '\n';
    return 0;
}

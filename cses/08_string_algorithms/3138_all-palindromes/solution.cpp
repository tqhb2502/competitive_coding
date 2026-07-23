#include <array>
#include <iostream>
#include <string>
#include <vector>

// Mỗi nút của Eertree ứng với một palindrome phân biệt.
struct Node {
    int length;                  // độ dài palindrome
    int link;                    // suffix link: palindrome-suffix thật sự dài nhất
    std::array<int, 26> next{};  // transition: nút ứng với palindrome c + P + c

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

    // Xâu 1-indexed với sentinel characters[0] = -1 (khác mọi mã kí tự 0..25)
    // để bảo đảm chỉ số index - length - 1 luôn hợp lệ, imaginary root luôn khớp.
    std::vector<int> characters(static_cast<std::size_t>(size + 1), -1);
    for (int index = 1; index <= size; ++index) {
        characters[static_cast<std::size_t>(index)] =
            text[static_cast<std::size_t>(index - 1)] - 'a';
    }

    // Hai gốc: nút 0 = imaginary root (len = -1), nút 1 = empty root (len = 0).
    std::vector<Node> tree;
    tree.reserve(text.size() + 2);
    tree.emplace_back(-1, 0);
    tree.emplace_back(0, 0);
    int last = 1;  // palindrome-suffix dài nhất của tiền tố hiện tại

    // Thêm từng kí tự từ trái sang phải.
    for (int index = 1; index <= size; ++index) {
        const int character = characters[static_cast<std::size_t>(index)];

        // Đi theo suffix link tìm current: suffix-palindrome dài nhất bọc thêm được kí tự.
        int current = last;
        while (characters[static_cast<std::size_t>(
                   index - tree[current].length - 1
               )] != character) {
            current = tree[current].link;
        }

        int child = tree[current].next[character];
        if (child == -1) {
            // Palindrome c + P(current) + c chưa tồn tại: tạo nút mới.
            const int new_length = tree[current].length + 2;
            int suffix_link = 1;
            if (new_length > 1) {
                // Tìm suffix link cho nút mới bằng cách tiếp tục đi từ link[current].
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

        // length[last] = độ dài palindrome-suffix dài nhất kết thúc tại index.
        if (index > 1) {
            std::cout << ' ';
        }
        std::cout << tree[last].length;
    }
    std::cout << '\n';
    return 0;
}

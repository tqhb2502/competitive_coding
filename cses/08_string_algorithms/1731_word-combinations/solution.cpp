#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr int MOD = 1'000'000'007;

// Mỗi nút Trie: next[c] là chỉ số nút con theo ký tự c (0 nghĩa là chưa có),
// is_word đánh dấu nút này là điểm kết thúc của một từ trong từ điển.
struct TrieNode {
    std::array<int, 26> next{};
    bool is_word = false;
};

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string target;
    int word_count = 0;
    std::cin >> target >> word_count;

    const int target_length = static_cast<int>(target.size());
    std::vector<TrieNode> trie(1);  // nút gốc mang chỉ số 0
    trie.reserve(1'000'001);

    // Xây Trie từ toàn bộ từ điển; bỏ qua từ dài hơn s vì không thể khớp.
    for (int index = 0; index < word_count; ++index) {
        std::string word;
        std::cin >> word;
        if (word.size() > target.size()) {
            continue;
        }

        int node = 0;
        for (const char character : word) {
            const int letter = character - 'a';
            if (trie[node].next[letter] == 0) {  // tạo nút con nếu chưa tồn tại
                trie[node].next[letter] = static_cast<int>(trie.size());
                trie.emplace_back();
            }
            node = trie[node].next[letter];
        }
        trie[node].is_word = true;  // đánh dấu kết thúc từ
    }

    // ways[i] = số cách ghép được tiền tố s[0..i-1]; ways[0] = 1 (chuỗi rỗng).
    std::vector<int> ways(target_length + 1, 0);
    ways[0] = 1;

    for (int start = 0; start < target_length; ++start) {
        if (ways[start] == 0) {
            continue;
        }

        // Đi trên Trie theo các ký tự s[start..], mỗi khi gặp nút kết thúc từ
        // (tức s[start..end] là một từ) thì cộng dồn ways[start] vào ways[end+1].
        int node = 0;
        for (int end = start; end < target_length; ++end) {
            const int letter = target[end] - 'a';
            node = trie[node].next[letter];
            if (node == 0) {  // không còn nhánh khớp -> dừng
                break;
            }
            if (trie[node].is_word) {
                ways[end + 1] += ways[start];
                if (ways[end + 1] >= MOD) {
                    ways[end + 1] -= MOD;
                }
            }
        }
    }

    std::cout << ways[target_length] << '\n';
    return 0;
}

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int size;
    std::cin >> size;

    // Đọc suffix array; đồng thời tính rank[p] = thứ hạng của suffix bắt đầu tại p.
    // Vì SA[i] là vị trí của suffix nhỏ thứ i nên rank[SA[i]] = i + 1.
    // rank có kích thước size + 2 để giữ rank[n+1] = 0 (suffix rỗng, nhỏ nhất).
    std::vector<int> suffix_array(static_cast<std::size_t>(size));
    std::vector<int> rank(static_cast<std::size_t>(size + 2), 0);
    for (int index = 0; index < size; ++index) {
        std::cin >> suffix_array[static_cast<std::size_t>(index)];
        rank[static_cast<std::size_t>(suffix_array[static_cast<std::size_t>(index)])] =
            index + 1;
    }

    // Greedy dựng chỉ số ký tự c[p] (0 = 'a'): suffix nhỏ nhất SA[0] nhận ký tự 'a',
    // rồi duyệt các suffix liên tiếp theo thứ tự SA.
    std::vector<int> letter(static_cast<std::size_t>(size + 1), 0);
    int current_letter = 0;
    letter[static_cast<std::size_t>(suffix_array[0])] = 0;
    for (int index = 1; index < size; ++index) {
        const int previous = suffix_array[static_cast<std::size_t>(index - 1)];
        const int current = suffix_array[static_cast<std::size_t>(index)];
        // Nếu rank[p+1] > rank[q+1] thì phần đuôi ngược thứ tự, buộc phải tăng ký tự;
        // ngược lại giữ nguyên ký tự để dùng ít ký tự phân biệt nhất.
        if (rank[static_cast<std::size_t>(previous + 1)] >
            rank[static_cast<std::size_t>(current + 1)]) {
            ++current_letter;
        }
        letter[static_cast<std::size_t>(current)] = current_letter;
    }

    // Cần quá 26 ký tự phân biệt thì không biểu diễn được bằng a-z.
    if (current_letter >= 26) {
        std::cout << -1 << '\n';
        return 0;
    }

    // Ghép các ký tự theo vị trí 1..n thành xâu kết quả.
    std::string answer(static_cast<std::size_t>(size), 'a');
    for (int position = 1; position <= size; ++position) {
        answer[static_cast<std::size_t>(position - 1)] =
            static_cast<char>('a' + letter[static_cast<std::size_t>(position)]);
    }
    std::cout << answer << '\n';
    return 0;
}

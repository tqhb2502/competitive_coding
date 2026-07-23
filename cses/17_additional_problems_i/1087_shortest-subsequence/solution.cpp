#include <array>
#include <cstddef>
#include <iostream>
#include <string>

using namespace std;

// Ánh xạ mỗi ký tự DNA thành chỉ số 0..3
size_t letterIndex(char letter) {
    if (letter == 'A') {
        return 0;
    }
    if (letter == 'C') {
        return 1;
    }
    if (letter == 'G') {
        return 2;
    }
    return 3;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string dna;
    cin >> dna;

    array<bool, 4> seen{};   // các ký tự đã gặp trong khối hiện tại
    int distinct = 0;        // số ký tự khác nhau trong khối hiện tại
    string answer;
    answer.reserve(dna.size() / 4 + 1);

    // Tham lam: cắt xâu thành các khối đủ cả bốn ký tự, mỗi khối đóng góp
    // một ký tự vào đáp án (chính ký tự làm khối trở nên đầy đủ)
    for (char letter : dna) {
        size_t index = letterIndex(letter);
        if (!seen[index]) {
            seen[index] = true;
            ++distinct;
        }
        if (distinct == 4) {
            answer.push_back(letter);
            seen.fill(false);   // bắt đầu đếm lại cho khối kế tiếp
            distinct = 0;
        }
    }

    // Đoạn dư cuối không đủ bốn ký tự: chọn một ký tự còn thiếu ghép vào đuôi
    constexpr array<char, 4> LETTERS{'A', 'C', 'G', 'T'};
    for (size_t index = 0; index < 4; ++index) {
        if (!seen[index]) {
            answer.push_back(LETTERS[index]);
            break;
        }
    }

    cout << answer << '\n';
}

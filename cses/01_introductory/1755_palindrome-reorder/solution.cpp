#include <array>
#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    cin >> input;

    // Đếm số lần xuất hiện của từng ký tự in hoa A-Z.
    array<int, 26> count{};
    for (char character : input) {
        ++count[character - 'A'];
    }

    // Tìm ký tự có tần suất lẻ (nếu có) và đếm xem có bao nhiêu ký tự như vậy.
    int oddLetter = -1;
    int oddCount = 0;
    for (int letter = 0; letter < 26; ++letter) {
        if (count[letter] % 2 == 1) {
            oddLetter = letter;
            ++oddCount;
        }
    }

    // Có nhiều hơn 1 ký tự tần suất lẻ thì không thể ghép đôi -> vô nghiệm.
    if (oddCount > 1) {
        cout << "NO SOLUTION\n";
        return 0;
    }

    // Dựng nửa trái: mỗi ký tự lấy (count / 2) bản, theo thứ tự A->Z.
    string leftHalf;
    leftHalf.reserve(input.size() / 2);
    for (int letter = 0; letter < 26; ++letter) {
        leftHalf.append(count[letter] / 2,
                        static_cast<char>('A' + letter));
    }

    // Kết quả = nửa trái + ký tự giữa (nếu có) + nửa trái đảo ngược.
    string rightHalf(leftHalf.rbegin(), leftHalf.rend());
    cout << leftHalf;
    if (oddLetter != -1) {
        cout << static_cast<char>('A' + oddLetter);
    }
    cout << rightHalf << '\n';
    return 0;
}

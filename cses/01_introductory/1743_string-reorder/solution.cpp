#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input;
    cin >> input;
    const int length = static_cast<int>(input.size());

    // Đếm số lần xuất hiện của từng chữ cái A-Z.
    array<int, 26> count{};
    for (char character : input) {
        ++count[character - 'A'];
    }

    // Điều kiện tồn tại: sắp xếp được khi và chỉ khi 2*M <= n+1.
    int currentMaximum = *max_element(count.begin(), count.end());
    if (2 * currentMaximum > length + 1) {
        cout << "-1\n";
        return 0;
    }

    // frequency[k] = số chữ cái đang còn đúng k lần; giúp cập nhật curmax O(1).
    vector<int> frequency(currentMaximum + 2, 0);
    for (int value : count) {
        ++frequency[value];
    }

    // active = danh sách (đã sắp xếp tăng dần) các chữ cái còn count > 0.
    vector<int> active;
    for (int letter = 0; letter < 26; ++letter) {
        if (count[letter] > 0) {
            active.push_back(letter);
        }
    }

    // maxletter = một chữ cái đạt count == curmax (kiểm chứng lại khi cần).
    int maximumLetter = 0;
    for (int letter = 0; letter < 26; ++letter) {
        if (count[letter] == currentMaximum) {
            maximumLetter = letter;
            break;
        }
    }

    string result(length, ' ');
    int previous = -1;      // chữ cái vừa đặt (-1 = chưa có)
    int remaining = length; // số ký tự còn phải đặt

    // Tham lam theo từng vị trí: đặt chữ cái nhỏ nhất mà phần còn lại vẫn khả thi.
    for (int position = 0; position < length; ++position) {
        int chosen;
        if (2 * currentMaximum > remaining) {
            // Trạng thái CĂNG (2*curmax == remaining+1): chữ trội là duy nhất
            // và BUỘC phải đặt ngay, nếu không sẽ không xếp vừa nữa.
            if (count[maximumLetter] != currentMaximum) {
                // maxletter đã "cũ" -> quét lại tìm chữ đang đạt curmax.
                for (int letter : active) {
                    if (count[letter] == currentMaximum) {
                        maximumLetter = letter;
                        break;
                    }
                }
            }
            chosen = maximumLetter;
        } else {
            // Trạng thái LỎNG: chọn chữ nhỏ nhất khác chữ liền trước (previous).
            chosen = active.front();
            if (chosen == previous) {
                chosen = active[1];
            }
        }

        result[position] = static_cast<char>('A' + chosen);

        // Giảm 1 lần dùng chữ đã chọn và cập nhật các cấu trúc phụ trong O(1).
        const int oldCount = count[chosen];
        --count[chosen];
        --frequency[oldCount];
        ++frequency[oldCount - 1];
        if (oldCount == currentMaximum && frequency[currentMaximum] == 0) {
            --currentMaximum; // không còn chữ nào đạt curmax -> giảm 1.
        }
        if (oldCount == 1) {
            active.erase(find(active.begin(), active.end(), chosen)); // hết chữ này.
        }

        previous = chosen;
        --remaining;
    }

    cout << result << '\n';
    return 0;
}

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// Đếm số bước Euclid trừ theo lô để đưa cặp (first, second) về (1, 1).
// Mỗi lô trừ count lần số nhỏ khỏi số lớn; nếu không về được 1 thì cặp không
// nguyên tố cùng nhau, trả về vô cực (không hợp lệ).
int subtractionSteps(int first, int second) {
    int steps = 0;
    while (first != second) {
        if (first > second) {
            const int count = (first - 1) / second;
            first -= count * second;
            steps += count;
        } else {
            const int count = (second - 1) / first;
            second -= count * first;
            steps += count;
        }
    }
    return first == 1 ? steps : numeric_limits<int>::max();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int subsequences;
    cin >> subsequences;

    // Điều kiện đúng n dãy con: chọn cặp (first, second) với first + second = n + 2.
    const int sum = subsequences + 2;

    // Duyệt mọi first, tìm cặp có độ dài xâu (số bước) nhỏ nhất.
    int bestFirst = 1;
    int bestSteps = numeric_limits<int>::max();
    for (int first = 1; first <= sum / 2; ++first) {
        const int steps = subtractionSteps(first, sum - first);
        if (steps < bestSteps) {
            bestSteps = steps;
            bestFirst = first;
        }
    }

    // Dựng lại xâu bằng phép trừ từng bước: first > second ghi bit 0, ngược lại ghi bit 1.
    int first = bestFirst;
    int second = sum - first;
    string answer;
    answer.reserve(static_cast<size_t>(bestSteps));
    while (first != second) {
        if (first > second) {
            first -= second;
            answer.push_back('0');
        } else {
            second -= first;
            answer.push_back('1');
        }
    }
    // Chuỗi bit dựng theo thứ tự ngược, đảo lại để có đáp án.
    reverse(answer.begin(), answer.end());
    cout << answer << '\n';
}

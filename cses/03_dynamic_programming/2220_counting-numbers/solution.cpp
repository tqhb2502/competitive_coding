#include <cstring>
#include <iostream>
#include <string>

using namespace std;

// upperBoundDigits: biểu diễn thập phân của cận trên x cho lần count hiện tại.
string upperBoundDigits;
// memo[pos][prev][started][tight]: bảng ghi nhớ của Digit DP. prev = 10 nghĩa là
// chưa đặt chữ số nào (đang trong vùng leading zero), nên cần 11 giá trị.
long long memo[20][11][2][2];

// Digit DP: đếm các số 0..x không có hai chữ số liền kề bằng nhau, duyệt các chữ
// số từ trái sang phải theo trạng thái (pos, prev, started, tight).
long long digitDp(int position, int previousDigit, bool started, bool tight) {
    // Cơ sở: đã đặt hết chữ số, số vừa tạo hợp lệ nên đếm là 1.
    if (position == static_cast<int>(upperBoundDigits.size())) {
        return 1;
    }

    long long& result = memo[position][previousDigit][started][tight];
    if (result != -1) {
        return result;
    }

    result = 0;
    const int upperDigit = upperBoundDigits[position] - '0';
    // Khi tight thì chữ số bị chặn trên bởi chữ số tương ứng của x.
    const int limit = tight ? upperDigit : 9;
    for (int digit = 0; digit <= limit; ++digit) {
        const bool nextTight = tight && digit == upperDigit;
        if (!started && digit == 0) {
            // Vẫn là leading zero: chưa thực sự bắt đầu số, giữ prev = 10.
            result += digitDp(position + 1, 10, false, nextTight);
        } else {
            // Đã bắt đầu số: cấm chữ số trùng chữ số liền trước.
            if (started && digit == previousDigit) {
                continue;
            }
            result += digitDp(position + 1, digit, true, nextTight);
        }
    }
    return result;
}

// countValid(x) = số các số nguyên trong [0, x] thỏa mãn điều kiện.
long long countValid(long long bound) {
    if (bound < 0) {
        return 0;
    }
    upperBoundDigits = to_string(bound);
    memset(memo, -1, sizeof(memo));
    return digitDp(0, 10, false, true);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long lowerBound, upperBound;
    cin >> lowerBound >> upperBound;
    // Đáp án cho đoạn [a, b] tính bằng hiệu hai giá trị tiền tố.
    cout << countValid(upperBound) - countValid(lowerBound - 1) << '\n';
    return 0;
}

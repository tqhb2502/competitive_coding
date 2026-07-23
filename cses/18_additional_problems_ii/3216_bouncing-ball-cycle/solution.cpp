#include <iostream>
#include <numeric>

using namespace std;

using Integer = long long;

// Kích thước giao của hai quan hệ đồng dư trên lớp tự đối (lớp 0 và lớp g).
// Nếu một cạnh có thương chẵn thì chỉ có một lớp, ngược lại: reflected -> 2
// lớp (0 và g), còn không thì toàn bộ divisor + 1 lớp trùng nhau.
Integer intersectionSize(bool firstOdd, bool secondOdd, Integer divisor,
                         bool reflected) {
    if (!firstOdd || !secondOdd) {
        return 1;
    }
    return reflected ? 2 : divisor + 1;
}

// Đếm số ô khác nhau mà bóng đi qua, dựa trên số cặp lớp thặng dư modulo 2g.
Integer countVisited(Integer firstSide, Integer secondSide) {
    const Integer divisor = gcd(firstSide, secondSide);   // g = gcd(a, b)
    const Integer firstQuotient = firstSide / divisor;    // A = a / g
    const Integer secondQuotient = secondSide / divisor;  // B = b / g
    // Số phần tử đầy đủ trong mỗi lớp modulo 2g trên từng cạnh là floor(A/2).
    const Integer firstBase = firstQuotient / 2;
    const Integer secondBase = secondQuotient / 2;
    // Thương lẻ hay chẵn quyết định phần "dư ra" ở các lớp biên.
    const bool firstOdd = firstQuotient % 2 != 0;
    const bool secondOdd = secondQuotient % 2 != 0;
    const Integer firstExtra = firstOdd ? divisor + 1 : 1;
    const Integer secondExtra = secondOdd ? divisor + 1 : 1;

    // Phần chung của cả hai quan hệ đồng dư (không tính lớp giao đặc biệt).
    const Integer basePart = 2 * divisor * firstBase * secondBase
                           + firstBase * secondExtra
                           + secondBase * firstExtra;
    // Số cặp có phần dư bằng nhau và số cặp có phần dư đối nhau.
    const Integer sameResidue = basePart
        + intersectionSize(firstOdd, secondOdd, divisor, false);
    const Integer oppositeResidue = basePart
        + intersectionSize(firstOdd, secondOdd, divisor, true);

    // Các lớp tự đối (lớp 0 và lớp g) bị đếm ở cả hai quan hệ nên phải trừ ra.
    const Integer firstAtZero = firstBase + 1;
    const Integer secondAtZero = secondBase + 1;
    const Integer firstAtHalf = firstBase + (firstOdd ? 1 : 0);
    const Integer secondAtHalf = secondBase + (secondOdd ? 1 : 0);
    return sameResidue + oppositeResidue
         - firstAtZero * secondAtZero - firstAtHalf * secondAtHalf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests-- > 0) {
        Integer rows;
        Integer columns;
        cin >> rows >> columns;
        const Integer firstSide = rows - 1;     // a = n - 1
        const Integer secondSide = columns - 1; // b = m - 1
        // Thời điểm quay lại góc xuất phát: T = 2 * lcm(a, b).
        const Integer common = firstSide / gcd(firstSide, secondSide) * secondSide;
        cout << 2 * common << ' ' << countVisited(firstSide, secondSide) << '\n';
    }
}

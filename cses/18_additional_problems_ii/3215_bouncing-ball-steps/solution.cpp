#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>

using namespace std;

using Integer = long long;

// Tọa độ theo một chiều dài "length" sau "steps" bước: trải phẳng các lần phản
// xạ, tọa độ chạy tuần hoàn với chu kỳ 2*(length-1) rồi lấy công thức tam giác.
Integer coordinate(Integer length, Integer steps) {
    const Integer side = length - 1;
    const Integer remainder = steps % (2 * side);
    return 1 + min(remainder, 2 * side - remainder);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests-- > 0) {
        Integer rows;
        Integer columns;
        Integer steps;
        cin >> rows >> columns >> steps;

        // Bóng đổi hướng khi chạm biên dọc (bội của rows-1) hoặc biên ngang
        // (bội của columns-1); dùng bù trừ để không đếm lặp lần chạm đồng thời.
        const Integer verticalPeriod = rows - 1;
        const Integer horizontalPeriod = columns - 1;
        const Integer commonPeriod =
            verticalPeriod / gcd(verticalPeriod, horizontalPeriod) * horizontalPeriod;
        const Integer changes = steps / verticalPeriod + steps / horizontalPeriod
                              - steps / commonPeriod;

        // In ra hàng, cột sau k bước và tổng số lần đổi hướng.
        cout << coordinate(rows, steps) << ' '
             << coordinate(columns, steps) << ' '
             << changes << '\n';
    }
}

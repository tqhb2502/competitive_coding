#include <iostream>
#include <string>

using namespace std;

// Tìm chữ số nằm ở vị trí thứ k (đếm từ 1) trong chuỗi 123456789101112...
char digitAt(long long k) {
    // digits = số chữ số của nhóm hiện tại (d)
    // count  = số lượng số có digits chữ số = 9 * 10^(digits-1)
    // start  = số nhỏ nhất có digits chữ số = 10^(digits-1)
    long long digits = 1;
    long long count = 9;
    long long start = 1;

    // Trừ dần tổng số chữ số của từng nhóm cho đến khi k rơi vào nhóm hiện tại
    while (k > digits * count) {
        k -= digits * count;
        ++digits;
        count *= 10;
        start *= 10;
    }

    // k là vị trí 1-indexed trong khối các số có digits chữ số:
    // xác định số chứa chữ số và chỉ số chữ số bên trong số đó
    const long long number = start + (k - 1) / digits;
    const int digitIndex = static_cast<int>((k - 1) % digits);
    return to_string(number)[digitIndex];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    while (q--) {
        long long k;
        cin >> k;
        cout << digitAt(k) << '\n';
    }

    return 0;
}

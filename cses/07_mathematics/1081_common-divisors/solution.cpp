#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc mảng và ghi nhận giá trị lớn nhất M.
    int n;
    cin >> n;
    vector<int> values(static_cast<size_t>(n));
    int maximum = 0;
    for (int& value : values) {
        cin >> value;
        maximum = max(maximum, value);
    }

    // frequency[v] = số phần tử của mảng có giá trị đúng bằng v.
    vector<int> frequency(static_cast<size_t>(maximum) + 1, 0);
    for (int value : values) {
        ++frequency[static_cast<size_t>(value)];
    }

    // Duyệt ước d từ lớn xuống nhỏ (harmonic sieve). Với mỗi d, đếm số phần tử
    // chia hết cho d bằng cách cộng frequency tại các bội d, 2d, 3d, ...
    // d đầu tiên có >= 2 bội chính là gcd lớn nhất của một cặp -> đáp án.
    for (int divisor = maximum; divisor >= 1; --divisor) {
        int divisible_count = 0;
        for (int multiple = divisor; multiple <= maximum; multiple += divisor) {
            divisible_count += frequency[static_cast<size_t>(multiple)];
            if (divisible_count >= 2) {
                cout << divisor << '\n';
                return 0;
            }
        }
    }
}

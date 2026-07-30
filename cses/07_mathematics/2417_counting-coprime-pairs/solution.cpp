#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc n giá trị, đồng thời lưu giá trị lớn nhất để giới hạn kích thước sàng.
    int n;
    std::cin >> n;
    std::vector<int> values(n);
    int maximum_value = 0;
    for (int& value : values) {
        std::cin >> value;
        maximum_value = std::max(maximum_value, value);
    }

    // freq[v] = số lần giá trị v xuất hiện.
    std::vector<int> frequency(maximum_value + 1, 0);
    for (const int value : values) {
        ++frequency[value];
    }

    // Sàng tuyến tính (linear sieve) tính hàm Mobius mu[] đến maximum_value.
    // mu[m] = (-1)^(số ước nguyên tố phân biệt) nếu m squarefree, ngược lại = 0.
    std::vector<int> mobius(maximum_value + 1, 0);
    std::vector<int> primes;
    std::vector<char> composite(maximum_value + 1, false);
    if (maximum_value >= 1) {
        mobius[1] = 1;
    }
    for (int value = 2; value <= maximum_value; ++value) {
        if (!composite[value]) {
            // value là số nguyên tố: có đúng một ước nguyên tố nên mu = -1.
            primes.push_back(value);
            mobius[value] = -1;
        }
        for (const int prime : primes) {
            if (prime > maximum_value / value) {
                break;
            }
            const int product = prime * value;
            composite[product] = true;
            if (value % prime == 0) {
                // product chia hết cho prime^2 -> không squarefree -> mu = 0.
                mobius[product] = 0;
                break;
            }
            // Thêm một ước nguyên tố phân biệt -> đổi dấu mu.
            mobius[product] = -mobius[value];
        }
    }

    // ans = sum_{d} mu[d] * C(cnt[d], 2), với cnt[d] = số phần tử chia hết cho d.
    // Chỉ những d có mu[d] != 0 mới đóng góp.
    long long answer = 0;
    for (int divisor = 1; divisor <= maximum_value; ++divisor) {
        if (mobius[divisor] == 0) {
            continue;
        }
        // Đếm số phần tử chia hết cho divisor bằng cách cộng freq của các bội.
        long long divisible_count = 0;
        for (int multiple = divisor; multiple <= maximum_value;
             multiple += divisor) {
            divisible_count += frequency[multiple];
        }
        // C(cnt, 2) = cnt*(cnt-1)/2 là số cặp cùng chia hết cho divisor.
        answer += static_cast<long long>(mobius[divisor]) *
                  divisible_count * (divisible_count - 1) / 2;
    }

    std::cout << answer << '\n';
    return 0;
}

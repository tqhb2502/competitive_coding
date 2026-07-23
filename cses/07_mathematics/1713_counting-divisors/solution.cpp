#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> values(static_cast<size_t>(n));
    for (int& value : values) {
        cin >> value;
    }

    // Xây smallest prime factor sieve tới giá trị lớn nhất: spf[i] là ước
    // nguyên tố nhỏ nhất của i. Khởi tạo spf[i] = i, rồi với mỗi nguyên tố
    // prime, đánh dấu các bội chưa được chạm.
    const int maximum = *max_element(values.begin(), values.end());
    vector<int> smallest_prime(static_cast<size_t>(maximum) + 1);
    iota(smallest_prime.begin(), smallest_prime.end(), 0);
    for (int prime = 2; prime * prime <= maximum; ++prime) {
        // spf[prime] != prime nghĩa là prime đã có ước nhỏ hơn, không phải
        // nguyên tố, bỏ qua.
        if (smallest_prime[static_cast<size_t>(prime)] != prime) {
            continue;
        }
        for (int multiple = prime * prime; multiple <= maximum; multiple += prime) {
            // Chỉ gán khi bội chưa được nguyên tố nhỏ hơn đánh dấu.
            if (smallest_prime[static_cast<size_t>(multiple)] == multiple) {
                smallest_prime[static_cast<size_t>(multiple)] = prime;
            }
        }
    }

    // Với mỗi x: phân tích thừa số bằng cách liên tục chia cho spf[x], đếm số
    // mũ e của từng nguyên tố, rồi nhân số ước với (e + 1).
    for (int value : values) {
        int divisors = 1;
        while (value > 1) {
            const int prime = smallest_prime[static_cast<size_t>(value)];
            int exponent = 0;
            do {
                value /= prime;
                ++exponent;
            } while (value > 1 && smallest_prime[static_cast<size_t>(value)] == prime);
            divisors *= exponent + 1;
        }
        cout << divisors << '\n';
    }
}

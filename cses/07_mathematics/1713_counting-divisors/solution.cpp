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

    const int maximum = *max_element(values.begin(), values.end());
    vector<int> smallest_prime(static_cast<size_t>(maximum) + 1);
    iota(smallest_prime.begin(), smallest_prime.end(), 0);
    for (int prime = 2; prime * prime <= maximum; ++prime) {
        if (smallest_prime[static_cast<size_t>(prime)] != prime) {
            continue;
        }
        for (int multiple = prime * prime; multiple <= maximum; multiple += prime) {
            if (smallest_prime[static_cast<size_t>(multiple)] == multiple) {
                smallest_prime[static_cast<size_t>(multiple)] = prime;
            }
        }
    }

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

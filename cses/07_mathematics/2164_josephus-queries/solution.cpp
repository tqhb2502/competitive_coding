#include <iostream>

long long kth_removed(long long n, long long k) {
    // The final original label is maintained as offset + scale * sublabel.
    long long offset = 0;
    long long scale = 1;

    while (true) {
        const long long first_round = n / 2;
        if (k <= first_round) {
            return offset + scale * (2 * k);
        }

        const long long remaining_k = k - first_round;
        if ((n & 1LL) == 0) {
            // Odd survivors 1,3,... map a sublabel x to 2*x-1.
            offset -= scale;
            scale *= 2;
            n = first_round;
            k = remaining_k;
        } else {
            // After the even labels, label 1 is removed next.
            if (remaining_k == 1) {
                return offset + scale;
            }
            // Remaining labels 3,5,... map a sublabel x to 2*x+1.
            offset += scale;
            scale *= 2;
            n = first_round;
            k = remaining_k - 1;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int queries;
    std::cin >> queries;
    while (queries-- > 0) {
        long long n, k;
        std::cin >> n >> k;
        std::cout << kth_removed(n, k) << '\n';
    }
    return 0;
}

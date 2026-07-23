#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        cin >> n;

        // Grundy của mỗi đống theo subtraction game {1,2,3} là heap % 4,
        // XOR toàn bộ để lấy giá trị Grundy của cả ván.
        long long nim_sum = 0;
        while (n--) {
            long long heap;
            cin >> heap;
            nim_sum ^= heap % 4;
        }

        // XOR khác 0 -> N-position (first thắng), bằng 0 -> P-position (second thắng).
        cout << (nim_sum == 0 ? "second" : "first") << '\n';
    }
}

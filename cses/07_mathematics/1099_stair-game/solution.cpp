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
        // Staircase Nim: chỉ XOR số bóng trên các bậc 1-indexed CHẴN
        // (tức các bậc ở khoảng cách lẻ tới sink là bậc 1).
        long long nim_sum = 0;
        for (int stair = 1; stair <= n; ++stair) {
            long long balls;
            cin >> balls;
            if (stair % 2 == 0) {
                nim_sum ^= balls;
            }
        }
        // XOR khác 0 -> người đi trước thắng, ngược lại người đi sau thắng.
        cout << (nim_sum == 0 ? "second" : "first") << '\n';
    }
}

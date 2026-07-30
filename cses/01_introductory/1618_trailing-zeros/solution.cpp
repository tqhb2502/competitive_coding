#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    // Công thức Legendre: đếm số mũ của thừa số nguyên tố 5 trong n!,
    // cũng chính là số chữ số 0 ở cuối của n!.
    // Mỗi vòng cộng dồn số bội của 5, 25, 125, ... trong [1..n].
    long long result = 0;
    for (long long power_of_five = 5; power_of_five <= n; power_of_five *= 5) {
        result += n / power_of_five;
    }

    cout << result << '\n';
    return 0;
}

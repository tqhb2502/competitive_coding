#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    long long result = 0;
    for (long long power_of_five = 5; power_of_five <= n; power_of_five *= 5) {
        result += n / power_of_five;
    }

    cout << result << '\n';
    return 0;
}

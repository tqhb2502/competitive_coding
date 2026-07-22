#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int children, maximum_candies;
    cin >> children >> maximum_candies;

    long double answer = 0.0L;
    long double previous_cdf = 0.0L;
    for (int candies = 1; candies <= maximum_candies; ++candies) {
        const long double ratio = static_cast<long double>(candies) / maximum_candies;
        const long double current_cdf = pow(ratio, children);
        answer += candies * (current_cdf - previous_cdf);
        previous_cdf = current_cdf;
    }

    cout << fixed << setprecision(6) << answer << '\n';
    return 0;
}

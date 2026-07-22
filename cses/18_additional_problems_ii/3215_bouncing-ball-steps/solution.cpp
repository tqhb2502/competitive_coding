#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>

using namespace std;

using Integer = long long;

Integer coordinate(Integer length, Integer steps) {
    const Integer side = length - 1;
    const Integer remainder = steps % (2 * side);
    return 1 + min(remainder, 2 * side - remainder);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests-- > 0) {
        Integer rows;
        Integer columns;
        Integer steps;
        cin >> rows >> columns >> steps;

        const Integer verticalPeriod = rows - 1;
        const Integer horizontalPeriod = columns - 1;
        const Integer commonPeriod =
            verticalPeriod / gcd(verticalPeriod, horizontalPeriod) * horizontalPeriod;
        const Integer changes = steps / verticalPeriod + steps / horizontalPeriod
                              - steps / commonPeriod;

        cout << coordinate(rows, steps) << ' '
             << coordinate(columns, steps) << ' '
             << changes << '\n';
    }
}

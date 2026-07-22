#include <cstring>
#include <iostream>
#include <string>

using namespace std;

string upperBoundDigits;
long long memo[20][11][2][2];

long long digitDp(int position, int previousDigit, bool started, bool tight) {
    if (position == static_cast<int>(upperBoundDigits.size())) {
        return 1;
    }

    long long& result = memo[position][previousDigit][started][tight];
    if (result != -1) {
        return result;
    }

    result = 0;
    const int upperDigit = upperBoundDigits[position] - '0';
    const int limit = tight ? upperDigit : 9;
    for (int digit = 0; digit <= limit; ++digit) {
        const bool nextTight = tight && digit == upperDigit;
        if (!started && digit == 0) {
            result += digitDp(position + 1, 10, false, nextTight);
        } else {
            if (started && digit == previousDigit) {
                continue;
            }
            result += digitDp(position + 1, digit, true, nextTight);
        }
    }
    return result;
}

long long countValid(long long bound) {
    if (bound < 0) {
        return 0;
    }
    upperBoundDigits = to_string(bound);
    memset(memo, -1, sizeof(memo));
    return digitDp(0, 10, false, true);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long lowerBound, upperBound;
    cin >> lowerBound >> upperBound;
    cout << countValid(upperBound) - countValid(lowerBound - 1) << '\n';
    return 0;
}

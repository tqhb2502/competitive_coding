#include <iostream>
#include <numeric>

using namespace std;

using Integer = long long;

Integer intersectionSize(bool firstOdd, bool secondOdd, Integer divisor,
                         bool reflected) {
    if (!firstOdd || !secondOdd) {
        return 1;
    }
    return reflected ? 2 : divisor + 1;
}

Integer countVisited(Integer firstSide, Integer secondSide) {
    const Integer divisor = gcd(firstSide, secondSide);
    const Integer firstQuotient = firstSide / divisor;
    const Integer secondQuotient = secondSide / divisor;
    const Integer firstBase = firstQuotient / 2;
    const Integer secondBase = secondQuotient / 2;
    const bool firstOdd = firstQuotient % 2 != 0;
    const bool secondOdd = secondQuotient % 2 != 0;
    const Integer firstExtra = firstOdd ? divisor + 1 : 1;
    const Integer secondExtra = secondOdd ? divisor + 1 : 1;

    const Integer basePart = 2 * divisor * firstBase * secondBase
                           + firstBase * secondExtra
                           + secondBase * firstExtra;
    const Integer sameResidue = basePart
        + intersectionSize(firstOdd, secondOdd, divisor, false);
    const Integer oppositeResidue = basePart
        + intersectionSize(firstOdd, secondOdd, divisor, true);

    const Integer firstAtZero = firstBase + 1;
    const Integer secondAtZero = secondBase + 1;
    const Integer firstAtHalf = firstBase + (firstOdd ? 1 : 0);
    const Integer secondAtHalf = secondBase + (secondOdd ? 1 : 0);
    return sameResidue + oppositeResidue
         - firstAtZero * secondAtZero - firstAtHalf * secondAtHalf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;
    while (tests-- > 0) {
        Integer rows;
        Integer columns;
        cin >> rows >> columns;
        const Integer firstSide = rows - 1;
        const Integer secondSide = columns - 1;
        const Integer common = firstSide / gcd(firstSide, secondSide) * secondSide;
        cout << 2 * common << ' ' << countVisited(firstSide, secondSide) << '\n';
    }
}

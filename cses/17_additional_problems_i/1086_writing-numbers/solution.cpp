#include <iostream>

using namespace std;

using UnsignedInteger = unsigned long long;

UnsignedInteger countDigit(UnsignedInteger lastNumber, int digit) {
    UnsignedInteger result = 0;
    for (UnsignedInteger factor = 1; factor <= lastNumber; factor *= 10) {
        const UnsignedInteger higher = lastNumber / (factor * 10);
        const int current = static_cast<int>((lastNumber / factor) % 10);
        const UnsignedInteger lower = lastNumber % factor;

        if (digit != 0) {
            result += higher * factor;
            if (current > digit) {
                result += factor;
            } else if (current == digit) {
                result += lower + 1;
            }
        } else if (higher != 0) {
            result += (higher - 1) * factor;
            if (current == 0) {
                result += lower + 1;
            } else {
                result += factor;
            }
        }

        if (factor > lastNumber / 10) {
            break;
        }
    }
    return result;
}

bool canWrite(UnsignedInteger lastNumber, UnsignedInteger limit) {
    for (int digit = 0; digit <= 9; ++digit) {
        if (countDigit(lastNumber, digit) > limit) {
            return false;
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long inputLimit;
    cin >> inputLimit;
    const UnsignedInteger limit = inputLimit;

    UnsignedInteger low = 0;
    UnsignedInteger high = 1'000'000'000'000'000'000ULL;
    while (low + 1 < high) {
        const UnsignedInteger middle = (low + high) / 2;
        if (canWrite(middle, limit)) {
            low = middle;
        } else {
            high = middle;
        }
    }

    cout << low << '\n';
}

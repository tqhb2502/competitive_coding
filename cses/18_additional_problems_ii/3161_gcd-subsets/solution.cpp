#include <iostream>
#include <vector>

using namespace std;

constexpr long long MODULO = 1'000'000'007LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size = 0;
    cin >> size;
    vector<int> frequency(size + 1, 0);
    for (int index = 0; index < size; ++index) {
        int value = 0;
        cin >> value;
        ++frequency[value];
    }

    vector<long long> powersOfTwo(size + 1, 1);
    for (int count = 1; count <= size; ++count) {
        powersOfTwo[count] = powersOfTwo[count - 1] * 2LL % MODULO;
    }

    vector<long long> exact(size + 1, 0);
    for (int divisor = size; divisor >= 1; --divisor) {
        int divisibleCount = 0;
        for (int multiple = divisor; multiple <= size; multiple += divisor) {
            divisibleCount += frequency[multiple];
        }

        long long ways = powersOfTwo[divisibleCount] - 1;
        for (int multiple = divisor + divisor; multiple <= size;
             multiple += divisor) {
            ways -= exact[multiple];
            if (ways < 0) {
                ways += MODULO;
            }
        }
        exact[divisor] = ways;
    }

    for (int divisor = 1; divisor <= size; ++divisor) {
        cout << exact[divisor] << (divisor == size ? '\n' : ' ');
    }
}

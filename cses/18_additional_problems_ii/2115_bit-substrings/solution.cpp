#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

long long modularPower(long long base, long long exponent, int modulus) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1;
    }
    return result;
}

template <int Modulus, int PrimitiveRoot>
void numberTheoreticTransform(vector<int>& values, bool invert) {
    const int size = static_cast<int>(values.size());
    for (int index = 1, reversed = 0; index < size; ++index) {
        int bit = size >> 1;
        while ((reversed & bit) != 0) {
            reversed ^= bit;
            bit >>= 1;
        }
        reversed ^= bit;
        if (index < reversed) {
            swap(values[index], values[reversed]);
        }
    }

    for (int length = 2; length <= size; length <<= 1) {
        int root = static_cast<int>(
            modularPower(PrimitiveRoot, (Modulus - 1) / length, Modulus));
        if (invert) {
            root = static_cast<int>(modularPower(root, Modulus - 2, Modulus));
        }
        for (int start = 0; start < size; start += length) {
            long long multiplier = 1;
            for (int offset = 0; offset < length / 2; ++offset) {
                const int first = values[start + offset];
                const int second = static_cast<int>(
                    multiplier * values[start + offset + length / 2] %
                    Modulus);
                int sum = first + second;
                if (sum >= Modulus) {
                    sum -= Modulus;
                }
                int difference = first - second;
                if (difference < 0) {
                    difference += Modulus;
                }
                values[start + offset] = sum;
                values[start + offset + length / 2] = difference;
                multiplier = multiplier * root % Modulus;
            }
        }
    }
    if (invert) {
        const long long inverseSize =
            modularPower(size, Modulus - 2, Modulus);
        for (int& value : values) {
            value = static_cast<int>(value * inverseSize % Modulus);
        }
    }
}

template <int Modulus, int PrimitiveRoot>
vector<int> convolution(const vector<long long>& first,
                        const vector<long long>& second) {
    int size = 1;
    while (size < static_cast<int>(first.size() + second.size() - 1)) {
        size <<= 1;
    }
    vector<int> left(size, 0);
    vector<int> right(size, 0);
    for (int index = 0; index < static_cast<int>(first.size()); ++index) {
        left[index] = static_cast<int>(first[index] % Modulus);
    }
    for (int index = 0; index < static_cast<int>(second.size()); ++index) {
        right[index] = static_cast<int>(second[index] % Modulus);
    }
    numberTheoreticTransform<Modulus, PrimitiveRoot>(left, false);
    numberTheoreticTransform<Modulus, PrimitiveRoot>(right, false);
    for (int index = 0; index < size; ++index) {
        left[index] = static_cast<int>(
            static_cast<long long>(left[index]) * right[index] % Modulus);
    }
    numberTheoreticTransform<Modulus, PrimitiveRoot>(left, true);
    left.resize(first.size() + second.size() - 1);
    return left;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string bits;
    cin >> bits;
    int ones = 0;
    vector<long long> frequency(bits.size() + 1, 0);
    ++frequency[0];
    for (const char bit : bits) {
        if (bit == '1') {
            ++ones;
        }
        ++frequency[ones];
    }
    frequency.resize(ones + 1);

    vector<long long> reversedFrequency = frequency;
    reverse(reversedFrequency.begin(), reversedFrequency.end());
    constexpr int firstModulus = 998244353;
    constexpr int secondModulus = 1004535809;
    const vector<int> firstProduct =
        convolution<firstModulus, 3>(frequency, reversedFrequency);
    const vector<int> secondProduct =
        convolution<secondModulus, 3>(frequency, reversedFrequency);
    const long long inverseFirstModulus = modularPower(
        firstModulus % secondModulus, secondModulus - 2, secondModulus);

    long long zeroAnswer = 0;
    for (const long long count : frequency) {
        zeroAnswer += count * (count - 1) / 2;
    }
    cout << zeroAnswer;
    const int valueCount = ones + 1;
    for (int requiredOnes = 1;
         requiredOnes <= static_cast<int>(bits.size()); ++requiredOnes) {
        long long answer = 0;
        if (requiredOnes <= ones) {
            const int index = valueCount - 1 - requiredOnes;
            const long long firstRemainder = firstProduct[index];
            long long difference =
                secondProduct[index] - firstRemainder % secondModulus;
            if (difference < 0) {
                difference += secondModulus;
            }
            const long long multiplier =
                difference * inverseFirstModulus % secondModulus;
            answer = firstRemainder + firstModulus * multiplier;
        }
        cout << ' ' << answer;
    }
    cout << '\n';
}

#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class FenwickTree {
public:
    FenwickTree(const int size, const long long modulus)
        : tree_(static_cast<std::size_t>(size + 1), 0), modulus_(modulus) {}

    void add(int index, long long delta) {
        delta %= modulus_;
        if (delta < 0) {
            delta += modulus_;
        }
        while (index < static_cast<int>(tree_.size())) {
            tree_[static_cast<std::size_t>(index)] += delta;
            if (tree_[static_cast<std::size_t>(index)] >= modulus_) {
                tree_[static_cast<std::size_t>(index)] -= modulus_;
            }
            index += index & -index;
        }
    }

    long long range_sum(const int left, const int right) const {
        long long result = prefix_sum(right) - prefix_sum(left - 1);
        if (result < 0) {
            result += modulus_;
        }
        return result;
    }

private:
    long long prefix_sum(int index) const {
        long long result = 0;
        while (index > 0) {
            result += tree_[static_cast<std::size_t>(index)];
            if (result >= modulus_) {
                result -= modulus_;
            }
            index -= index & -index;
        }
        return result;
    }

    std::vector<long long> tree_;
    long long modulus_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int length, operation_count;
    std::string text;
    std::cin >> length >> operation_count >> text;

    constexpr long long MOD1 = 1'000'000'007LL;
    constexpr long long MOD2 = 998'244'353LL;
    const std::uint64_t seed = static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    ) ^ (static_cast<std::uint64_t>(std::random_device{}()) << 32U);
    std::mt19937_64 generator(seed);
    const long long base1 = std::uniform_int_distribution<long long>(
        256, MOD1 - 2
    )(generator);
    const long long base2 = std::uniform_int_distribution<long long>(
        256, MOD2 - 2
    )(generator);

    std::vector<long long> power1(static_cast<std::size_t>(length + 1), 1);
    std::vector<long long> power2(static_cast<std::size_t>(length + 1), 1);
    for (int exponent = 1; exponent <= length; ++exponent) {
        power1[static_cast<std::size_t>(exponent)] =
            power1[static_cast<std::size_t>(exponent - 1)] * base1 % MOD1;
        power2[static_cast<std::size_t>(exponent)] =
            power2[static_cast<std::size_t>(exponent - 1)] * base2 % MOD2;
    }

    FenwickTree forward1(length, MOD1);
    FenwickTree backward1(length, MOD1);
    FenwickTree forward2(length, MOD2);
    FenwickTree backward2(length, MOD2);
    std::vector<int> value(static_cast<std::size_t>(length + 1), 0);

    for (int position = 1; position <= length; ++position) {
        value[static_cast<std::size_t>(position)] =
            text[static_cast<std::size_t>(position - 1)] - 'a' + 1;
        const long long character = value[static_cast<std::size_t>(position)];
        forward1.add(
            position,
            character * power1[static_cast<std::size_t>(position - 1)] % MOD1
        );
        backward1.add(
            position,
            character * power1[static_cast<std::size_t>(length - position)] % MOD1
        );
        forward2.add(
            position,
            character * power2[static_cast<std::size_t>(position - 1)] % MOD2
        );
        backward2.add(
            position,
            character * power2[static_cast<std::size_t>(length - position)] % MOD2
        );
    }

    while (operation_count-- > 0) {
        int type;
        std::cin >> type;
        if (type == 1) {
            int position;
            char character;
            std::cin >> position >> character;
            const int new_value = character - 'a' + 1;
            const int difference =
                new_value - value[static_cast<std::size_t>(position)];
            if (difference == 0) {
                continue;
            }
            value[static_cast<std::size_t>(position)] = new_value;
            forward1.add(
                position,
                static_cast<long long>(difference) *
                    power1[static_cast<std::size_t>(position - 1)]
            );
            backward1.add(
                position,
                static_cast<long long>(difference) *
                    power1[static_cast<std::size_t>(length - position)]
            );
            forward2.add(
                position,
                static_cast<long long>(difference) *
                    power2[static_cast<std::size_t>(position - 1)]
            );
            backward2.add(
                position,
                static_cast<long long>(difference) *
                    power2[static_cast<std::size_t>(length - position)]
            );
        } else {
            int left, right;
            std::cin >> left >> right;
            const long long forward_hash1 =
                forward1.range_sum(left, right) *
                power1[static_cast<std::size_t>(length - right)] % MOD1;
            const long long backward_hash1 =
                backward1.range_sum(left, right) *
                power1[static_cast<std::size_t>(left - 1)] % MOD1;
            const long long forward_hash2 =
                forward2.range_sum(left, right) *
                power2[static_cast<std::size_t>(length - right)] % MOD2;
            const long long backward_hash2 =
                backward2.range_sum(left, right) *
                power2[static_cast<std::size_t>(left - 1)] % MOD2;
            std::cout << (
                forward_hash1 == backward_hash1 &&
                forward_hash2 == backward_hash2 ? "YES" : "NO"
            ) << '\n';
        }
    }
    return 0;
}

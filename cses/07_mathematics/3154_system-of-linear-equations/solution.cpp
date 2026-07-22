#include <algorithm>
#include <iostream>
#include <vector>

constexpr long long MOD = 1'000'000'007LL;

long long modular_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int equation_count, variable_count;
    std::cin >> equation_count >> variable_count;
    std::vector<std::vector<long long>> matrix(
        equation_count, std::vector<long long>(variable_count + 1)
    );
    for (auto& row : matrix) {
        for (long long& value : row) {
            std::cin >> value;
        }
    }

    std::vector<int> pivot_row(variable_count, -1);
    int rank = 0;
    for (int column = 0;
         column < variable_count && rank < equation_count; ++column) {
        int selected = rank;
        while (selected < equation_count && matrix[selected][column] == 0) {
            ++selected;
        }
        if (selected == equation_count) {
            continue;
        }
        std::swap(matrix[selected], matrix[rank]);

        const long long inverse = modular_power(matrix[rank][column], MOD - 2);
        for (int next_column = column;
             next_column <= variable_count; ++next_column) {
            matrix[rank][next_column] =
                matrix[rank][next_column] * inverse % MOD;
        }

        for (int row = rank + 1; row < equation_count; ++row) {
            const long long factor = matrix[row][column];
            if (factor == 0) {
                continue;
            }
            matrix[row][column] = 0;
            for (int next_column = column + 1;
                 next_column <= variable_count; ++next_column) {
                matrix[row][next_column] = (
                    matrix[row][next_column] -
                    factor * matrix[rank][next_column]
                ) % MOD;
                if (matrix[row][next_column] < 0) {
                    matrix[row][next_column] += MOD;
                }
            }
        }

        pivot_row[column] = rank;
        ++rank;
    }

    for (int row = rank; row < equation_count; ++row) {
        bool all_zero = true;
        for (int column = 0; column < variable_count; ++column) {
            if (matrix[row][column] != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero && matrix[row][variable_count] != 0) {
            std::cout << -1 << '\n';
            return 0;
        }
    }

    std::vector<long long> solution(variable_count, 0);
    for (int column = variable_count - 1; column >= 0; --column) {
        if (pivot_row[column] == -1) {
            continue;
        }
        const int row = pivot_row[column];
        long long value = matrix[row][variable_count];
        for (int next_column = column + 1;
             next_column < variable_count; ++next_column) {
            value = (
                value - matrix[row][next_column] * solution[next_column]
            ) % MOD;
            if (value < 0) {
                value += MOD;
            }
        }
        solution[column] = value;
    }

    for (int variable = 0; variable < variable_count; ++variable) {
        if (variable > 0) {
            std::cout << ' ';
        }
        std::cout << solution[variable];
    }
    std::cout << '\n';
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>

constexpr long long MOD = 1'000'000'007LL;

// Nghịch đảo modular qua định lý Fermat: inv(a) = a^(MOD-2) mod MOD.
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

    // Đọc hệ n phương trình, m ẩn; mỗi hàng gồm m hệ số + vế phải (cột thứ m).
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

    // Khử tiến (forward elimination) đưa hệ về dạng bậc thang theo hàng.
    // pivot_row[column] = hàng làm pivot cho cột đó, -1 nếu là biến tự do.
    std::vector<int> pivot_row(variable_count, -1);
    int rank = 0;
    for (int column = 0;
         column < variable_count && rank < equation_count; ++column) {
        // Tìm một hàng chưa dùng có hệ số tại cột này khác 0 để làm pivot.
        int selected = rank;
        while (selected < equation_count && matrix[selected][column] == 0) {
            ++selected;
        }
        if (selected == equation_count) {
            continue;  // Không có pivot -> cột này là biến tự do, bỏ qua.
        }
        std::swap(matrix[selected], matrix[rank]);

        // Chuẩn hóa hàng pivot: nhân với nghịch đảo để hệ số pivot thành 1.
        const long long inverse = modular_power(matrix[rank][column], MOD - 2);
        for (int next_column = column;
             next_column <= variable_count; ++next_column) {
            matrix[rank][next_column] =
                matrix[rank][next_column] * inverse % MOD;
        }

        // Khử cột này khỏi mọi hàng phía dưới: row_i <- row_i - factor*pivot.
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

    // Hàng chỉ còn vế phải khác 0 (0 = b != 0) -> hệ vô nghiệm.
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

    // Thế ngược (back-substitution) từ pivot cuối lên đầu; biến tự do = 0.
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
        solution[column] = value;  // Hệ số pivot đã là 1 nên không cần chia.
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

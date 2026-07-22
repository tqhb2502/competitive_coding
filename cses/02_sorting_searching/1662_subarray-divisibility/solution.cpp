#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> residue_count(n, 0);
    residue_count[0] = 1;

    long long prefix = 0;
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        long long value;
        std::cin >> value;
        prefix += value;

        long long residue = prefix % n;
        if (residue < 0) {
            residue += n;
        }
        answer += residue_count[residue];
        ++residue_count[residue];
    }

    std::cout << answer << '\n';
    return 0;
}

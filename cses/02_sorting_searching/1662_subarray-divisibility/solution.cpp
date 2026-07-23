#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    // cnt[r] = số prefix đã gặp có số dư r khi chia cho n.
    // Khởi tạo cnt[0] = 1 tương ứng prefix rỗng P[0] = 0.
    std::vector<long long> residue_count(n, 0);
    residue_count[0] = 1;

    long long prefix = 0;
    long long answer = 0;  // đáp án có thể tới ~2e10 nên phải dùng long long
    for (int i = 0; i < n; ++i) {
        long long value;
        std::cin >> value;
        prefix += value;  // cộng dồn prefix sum

        // Lấy số dư và đưa về khoảng [0, n-1] vì phần tử có thể âm.
        long long residue = prefix % n;
        if (residue < 0) {
            residue += n;
        }

        // Mỗi prefix trước đó cùng số dư tạo thành một subarray hợp lệ.
        answer += residue_count[residue];
        ++residue_count[residue];
    }

    std::cout << answer << '\n';
    return 0;
}

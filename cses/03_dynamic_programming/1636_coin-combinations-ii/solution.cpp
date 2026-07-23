#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int MOD = 1'000'000'007;

    int n;
    int target;
    std::cin >> n >> target;

    std::vector<int> coins(n);
    for (int& coin : coins) {
        std::cin >> coin;
    }

    // ways[s] = số cách (không kể thứ tự) tạo ra tổng s.
    // Base case: ways[0] = 1 (một cách tạo tổng 0: không dùng đồng xu nào).
    std::vector<int> ways(target + 1, 0);
    ways[0] = 1;

    // Xét lần lượt từng loại đồng xu ở vòng NGOÀI để mỗi tổ hợp (multiset) chỉ
    // được sinh đúng một lần -> đếm không kể thứ tự.
    for (const int coin : coins) {
        if (coin > target) {
            continue;  // Đồng xu lớn hơn tổng đích thì không đóng góp gì.
        }
        // Cho tổng s tăng dần để loại xu hiện tại được dùng lại tùy ý (unbounded).
        for (int sum = coin; sum <= target; ++sum) {
            ways[sum] += ways[sum - coin];
            if (ways[sum] >= MOD) {
                ways[sum] -= MOD;  // Trừ có điều kiện thay cho phép lấy dư.
            }
        }
    }

    std::cout << ways[target] << '\n';
    return 0;
}

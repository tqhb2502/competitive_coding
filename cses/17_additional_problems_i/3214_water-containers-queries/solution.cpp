#include <iostream>
#include <numeric>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc số lượng truy vấn
    int tests = 0;
    std::cin >> tests;
    while (tests-- > 0) {
        // Mỗi truy vấn: hai bình dung tích a, b và lượng đích x cần có ở bình A
        long long capacityA = 0;
        long long capacityB = 0;
        long long target = 0;
        std::cin >> capacityA >> capacityB >> target;

        // Định lý hai bình: đạt được x khi và chỉ khi x không vượt dung tích bình
        // A và x chia hết cho gcd(a, b)
        const long long divisor = std::gcd(capacityA, capacityB);
        const bool possible = target <= capacityA && target % divisor == 0;
        std::cout << (possible ? "YES\n" : "NO\n");
    }
    return 0;
}

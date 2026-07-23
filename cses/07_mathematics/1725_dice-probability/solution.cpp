#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n, lower, upper;
    cin >> n >> lower >> upper;

    // probability[s] = xác suất để tổng hiện tại bằng s.
    // Khởi tạo: chưa tung con nào nên tổng chắc chắn bằng 0.
    vector<double> probability(6 * n + 1);
    probability[0] = 1.0;
    size_t maximum_sum = 0;

    // Thêm lần lượt từng con xúc xắc: phân phối mới là tích chập của
    // phân phối cũ với phân phối đều {1..6}.
    for (size_t dice = 0; dice < n; ++dice) {
        vector<double> next(6 * n + 1);
        for (size_t sum = 0; sum <= maximum_sum; ++sum) {
            // Mỗi mặt face = 1..6 xuất hiện với xác suất 1/6.
            for (size_t face = 1; face <= 6; ++face) {
                next[sum + face] += probability[sum] / 6.0;
            }
        }
        probability = std::move(next);
        maximum_sum += 6;
    }

    // Đáp số: tổng xác suất trên đoạn [lower, upper].
    double answer = 0.0;
    for (size_t sum = lower; sum <= upper; ++sum) {
        answer += probability[sum];
    }

    // In với 6 chữ số thập phân.
    cout << fixed << setprecision(6) << answer << '\n';
    return 0;
}

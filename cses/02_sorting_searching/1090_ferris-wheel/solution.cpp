#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long maximum_weight;
    std::cin >> n >> maximum_weight;

    std::vector<long long> weights(n);
    for (long long& weight : weights) {
        std::cin >> weight;
    }

    // Sắp xếp cân nặng tăng dần để dùng two-pointer.
    std::sort(weights.begin(), weights.end());

    // left: đứa nhẹ nhất còn lại; right: đứa nặng nhất còn lại.
    int left = 0;
    int right = n - 1;
    int gondolas = 0;

    // Mỗi bước xử lý đứa nặng nhất còn lại, tốn đúng 1 gondola.
    while (left <= right) {
        // Nếu ghép được đứa nhẹ nhất vào chung thì cho nó lên cùng.
        if (weights[left] + weights[right] <= maximum_weight) {
            ++left;
        }
        --right;
        ++gondolas;
    }

    std::cout << gondolas << '\n';
    return 0;
}

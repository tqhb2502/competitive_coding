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
    std::sort(weights.begin(), weights.end());

    int left = 0;
    int right = n - 1;
    int gondolas = 0;

    while (left <= right) {
        if (weights[left] + weights[right] <= maximum_weight) {
            ++left;
        }
        --right;
        ++gondolas;
    }

    std::cout << gondolas << '\n';
    return 0;
}

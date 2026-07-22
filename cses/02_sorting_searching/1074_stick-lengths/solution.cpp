#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> lengths(n);
    for (long long& length : lengths) {
        std::cin >> length;
    }

    std::sort(lengths.begin(), lengths.end());
    const long long median = lengths[n / 2];

    long long answer = 0;
    for (const long long length : lengths) {
        answer += (length >= median ? length - median : median - length);
    }

    std::cout << answer << '\n';
    return 0;
}

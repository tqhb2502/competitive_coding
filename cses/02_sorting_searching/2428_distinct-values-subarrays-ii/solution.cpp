#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, maximumDistinct;
    cin >> n >> maximumDistinct;

    vector<int> values(n);
    unordered_map<int, int> compressedIndex;
    compressedIndex.reserve(2 * n);
    compressedIndex.max_load_factor(0.7F);
    int distinctValues = 0;
    for (int& value : values) {
        int original;
        cin >> original;
        const auto [iterator, inserted] =
            compressedIndex.emplace(original, distinctValues);
        if (inserted) {
            ++distinctValues;
        }
        value = iterator->second;
    }

    vector<int> frequency(distinctValues, 0);
    int windowDistinct = 0;
    int left = 0;
    long long answer = 0;

    for (int right = 0; right < n; ++right) {
        if (frequency[values[right]]++ == 0) {
            ++windowDistinct;
        }
        while (windowDistinct > maximumDistinct) {
            if (--frequency[values[left]] == 0) {
                --windowDistinct;
            }
            ++left;
        }
        answer += right - left + 1;
    }

    cout << answer << '\n';
    return 0;
}

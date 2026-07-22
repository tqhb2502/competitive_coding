#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long limit;
    cin >> n >> limit;
    vector<long long> values(2 * n);
    for (int i = 0; i < n; ++i) {
        cin >> values[i];
        values[i + n] = values[i];
    }

    const int doubledSize = 2 * n;
    vector<int> nextPosition(doubledSize + 1, doubledSize);
    long long sum = 0;
    int right = 0;
    for (int left = 0; left < doubledSize; ++left) {
        while (right < doubledSize && sum + values[right] <= limit) {
            sum += values[right];
            ++right;
        }
        nextPosition[left] = right;
        sum -= values[left];
    }

    int levels = 1;
    while ((1LL << levels) <= n) {
        ++levels;
    }
    vector<vector<int>> jump(levels, vector<int>(doubledSize + 1, doubledSize));
    jump[0] = nextPosition;
    for (int level = 1; level < levels; ++level) {
        for (int position = 0; position <= doubledSize; ++position) {
            jump[level][position] = jump[level - 1][jump[level - 1][position]];
        }
    }

    int answer = numeric_limits<int>::max();
    for (int start = 0; start < n; ++start) {
        const int target = start + n;
        int position = start;
        int segments = 0;
        for (int level = levels - 1; level >= 0; --level) {
            if (jump[level][position] < target) {
                position = jump[level][position];
                segments += 1 << level;
            }
        }
        answer = min(answer, segments + 1);
    }

    cout << answer << '\n';
}

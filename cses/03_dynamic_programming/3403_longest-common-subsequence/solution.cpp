#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> first(n);
    vector<int> second(m);
    for (int& value : first) {
        cin >> value;
    }
    for (int& value : second) {
        cin >> value;
    }

    const int width = m + 1;
    vector<int> dp((n + 1) * width, 0);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            const int current = i * width + j;
            if (first[i - 1] == second[j - 1]) {
                dp[current] = dp[(i - 1) * width + j - 1] + 1;
            } else {
                dp[current] = max(dp[(i - 1) * width + j],
                                  dp[i * width + j - 1]);
            }
        }
    }

    vector<int> subsequence;
    int i = n;
    int j = m;
    while (i > 0 && j > 0) {
        if (first[i - 1] == second[j - 1]) {
            subsequence.push_back(first[i - 1]);
            --i;
            --j;
        } else if (dp[(i - 1) * width + j] >= dp[i * width + j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    reverse(subsequence.begin(), subsequence.end());

    cout << subsequence.size() << '\n';
    for (size_t index = 0; index < subsequence.size(); ++index) {
        if (index > 0) {
            cout << ' ';
        }
        cout << subsequence[index];
    }
    cout << '\n';
    return 0;
}

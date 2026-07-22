#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> prefixA(n + 1, 0);
    vector<long long> prefixB(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long value;
        cin >> value;
        prefixA[i] = prefixA[i - 1] + value;
    }
    for (int i = 1; i <= n; ++i) {
        long long value;
        cin >> value;
        prefixB[i] = prefixB[i - 1] + value;
    }

    long double low = 0.0L;
    long double high = 1'000'000'000.0L;
    for (int iteration = 0; iteration < 70; ++iteration) {
        const long double middle = (low + high) / 2.0L;
        long double bestA = numeric_limits<long double>::lowest();
        long double bestB = numeric_limits<long double>::lowest();

        for (int i = 1; i <= n; ++i) {
            bestA = max(bestA, static_cast<long double>(prefixA[i]) - middle * i);
            bestB = max(bestB, static_cast<long double>(prefixB[i]) - middle * i);
        }

        if (bestA + bestB >= 0.0L) {
            low = middle;
        } else {
            high = middle;
        }
    }

    int answerA = 1;
    int answerB = 1;
    long double bestA = numeric_limits<long double>::lowest();
    long double bestB = numeric_limits<long double>::lowest();
    for (int i = 1; i <= n; ++i) {
        const long double currentA = static_cast<long double>(prefixA[i]) - low * i;
        if (currentA > bestA) {
            bestA = currentA;
            answerA = i;
        }
        const long double currentB = static_cast<long double>(prefixB[i]) - low * i;
        if (currentB > bestB) {
            bestB = currentB;
            answerB = i;
        }
    }

    cout << answerA << ' ' << answerB << '\n';
}

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

    // prefixA[i], prefixB[i]: tổng i phần tử đầu của mỗi mảng (prefix sum)
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

    // Nhị phân trên giá trị trung bình x trong [0, 10^9].
    // x khả thi khi tồn tại cặp (i, j) có trung bình ít nhất x, tương đương
    // max_i(prefixA[i] - x*i) + max_j(prefixB[j] - x*j) >= 0 (hai chỉ số độc lập).
    long double low = 0.0L;
    long double high = 1'000'000'000.0L;
    for (int iteration = 0; iteration < 70; ++iteration) {
        const long double middle = (low + high) / 2.0L;
        long double bestA = numeric_limits<long double>::lowest();
        long double bestB = numeric_limits<long double>::lowest();

        // Tìm riêng giá trị lớn nhất của mỗi mảng trong O(n)
        for (int i = 1; i <= n; ++i) {
            bestA = max(bestA, static_cast<long double>(prefixA[i]) - middle * i);
            bestB = max(bestB, static_cast<long double>(prefixB[i]) - middle * i);
        }

        // Nếu tổng hai giá trị lớn nhất >= 0 thì x khả thi, nâng cận dưới
        if (bestA + bestB >= 0.0L) {
            low = middle;
        } else {
            high = middle;
        }
    }

    // Tại cận dưới cuối cùng, lấy chính hai chỉ số đạt giá trị lớn nhất làm đáp án
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

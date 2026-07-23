#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc hai mảng a (dài n) và b (dài m).
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

    // Bảng dp kích thước (n+1) x (m+1), làm phẳng thành mảng 1 chiều.
    // dp[i][j] = độ dài LCS giữa tiền tố a[0..i-1] và b[0..j-1]; hàng/cột 0 = 0.
    const int width = m + 1;
    vector<int> dp((n + 1) * width, 0);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            const int current = i * width + j;
            if (first[i - 1] == second[j - 1]) {
                // Trùng phần tử cuối: nối vào LCS của hai tiền tố ngắn hơn.
                dp[current] = dp[(i - 1) * width + j - 1] + 1;
            } else {
                // Khác: bỏ phần tử cuối của a hoặc của b, lấy phương án tốt hơn.
                dp[current] = max(dp[(i - 1) * width + j],
                                  dp[i * width + j - 1]);
            }
        }
    }

    // Truy vết ngược từ ô (n, m) để dựng lại một dãy con chung.
    vector<int> subsequence;
    int i = n;
    int j = m;
    while (i > 0 && j > 0) {
        if (first[i - 1] == second[j - 1]) {
            // Phần tử này thuộc LCS: ghi lại rồi lùi cả hai chỉ số.
            subsequence.push_back(first[i - 1]);
            --i;
            --j;
        } else if (dp[(i - 1) * width + j] >= dp[i * width + j - 1]) {
            // Đi về ô có dp lớn hơn.
            --i;
        } else {
            --j;
        }
    }
    // Đảo ngược để có thứ tự đúng từ trái sang phải.
    reverse(subsequence.begin(), subsequence.end());

    // In độ dài LCS rồi in dãy con.
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

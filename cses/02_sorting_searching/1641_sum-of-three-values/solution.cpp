#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long target;
    cin >> n >> target;

    // Ghép mỗi giá trị với chỉ số gốc (đánh số từ 1) để còn in ra vị trí ban đầu.
    vector<pair<long long, int>> values(n);
    for (int i = 0; i < n; ++i) {
        cin >> values[i].first;
        values[i].second = i + 1;
    }

    // Sort các cặp theo giá trị tăng dần để chạy two-pointer.
    sort(values.begin(), values.end());

    if (n >= 3) {
        // Tổng cặp lớn nhất có thể có trong bất kỳ hậu tố nào (hai phần tử lớn nhất ở cuối).
        const long long largest_pair = values[n - 1].first + values[n - 2].first;

        // Cố định phần tử đầu tiên values[i], quy về bài toán 2SUM trên đoạn [i+1, n-1].
        for (int i = 0; i + 2 < n; ++i) {
            const long long needed_pair = target - values[i].first;
            // Cắt tỉa: không cặp nào trong hậu tố đạt tới needed_pair -> bỏ qua i này.
            if (needed_pair > largest_pair) {
                continue;
            }
            // Cắt tỉa: cặp nhỏ nhất trong hậu tố đã vượt needed_pair; các i sau cũng vô vọng.
            if (needed_pair < values[i + 1].first + values[i + 2].first) {
                break;
            }

            // Two-pointer trên đoạn đã sort để tìm cặp có tổng bằng needed_pair.
            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                const long long pair_sum = values[left].first + values[right].first;
                if (pair_sum == needed_pair) {
                    // Tìm được bộ ba, in ra ba chỉ số gốc.
                    cout << values[i].second << ' ' << values[left].second << ' '
                         << values[right].second << '\n';
                    return 0;
                }
                if (pair_sum < needed_pair) {
                    ++left;   // Cần tổng lớn hơn.
                } else {
                    --right;  // Cần tổng nhỏ hơn.
                }
            }
        }
    }

    // Duyệt hết mà không có bộ ba nào thỏa mãn.
    cout << "IMPOSSIBLE\n";
    return 0;
}

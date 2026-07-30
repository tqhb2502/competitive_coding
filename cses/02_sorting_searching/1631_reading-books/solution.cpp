#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Duyệt qua từng quyển sách để tính đồng thời:
    //   total = S   = tổng thời gian tất cả các sách
    //   longest = M = thời gian của quyển dài nhất
    long long total = 0;
    long long longest = 0;
    for (int i = 0; i < n; ++i) {
        long long time;
        cin >> time;
        total += time;
        longest = max(longest, time);
    }

    // Đáp án = max(S, 2*M): nếu quyển dài nhất quá lớn (2*M > S) thì bị nghẽn
    // bởi 2*M, ngược lại đủ chỗ xếp lịch song song nên bằng S.
    cout << max(total, 2 * longest) << '\n';
    return 0;
}

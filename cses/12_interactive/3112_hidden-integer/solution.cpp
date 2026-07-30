#include <bits/stdc++.h>
using namespace std;

int main() {
    // Binary search theo giá trị: duy trì đoạn [lo, hi] chứa x, bất biến lo <= x <= hi
    long long lo = 1, hi = 1000000000LL;
    while (lo < hi) {
        // Vì lo < hi nên 1 <= mid <= hi-1, query luôn hợp lệ
        long long mid = (lo + hi) / 2;
        // In query và flush stdout bằng endl (bắt buộc cho interactive)
        cout << "? " << mid << endl;
        string resp;
        if (!(cin >> resp)) return 0; // judge đóng stream -> thoát
        if (resp == "YES") {
            // "YES" nghĩa là mid < x, suy ra x >= mid + 1
            lo = mid + 1;
        } else {
            // "NO" nghĩa là x <= mid
            hi = mid;
        }
    }
    // Đoạn co lại còn một ứng viên duy nhất: lo == hi == x
    cout << "! " << lo << endl;
    return 0;
}

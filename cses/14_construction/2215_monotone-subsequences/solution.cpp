#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;

        // Có nghiệm khi và chỉ khi n <= k*k (ràng buộc đã bảo đảm k <= n).
        // Vượt quá thì không thể dựng hoán vị nào -> IMPOSSIBLE.
        if ((long long)k * k < n) {
            cout << "IMPOSSIBLE\n";
            continue;
        }

        // Chia n phần tử vào đúng k block, kích thước đều nhất có thể.
        int base = n / k;   // >= 1 vì k <= n
        int rem = n % k;    // rem block đầu có kích thước base+1

        // Duyệt các block theo thứ tự xuất hiện, block đầu nhận dải giá trị lớn
        // nhất. Giữa các block giá trị giảm dần -> LDS = số block = k; bên trong
        // mỗi block giá trị tăng dần -> LIS = kích thước block lớn nhất <= k.
        string out;
        int high = n;       // giá trị lớn nhất còn lại để gán
        for (int i = 0; i < k; i++) {
            int sz = base + (i < rem ? 1 : 0);
            int low = high - sz + 1;
            for (int v = low; v <= high; v++) {   // in tăng dần trong block
                out += to_string(v);
                out.push_back(' ');
            }
            high -= sz;
        }
        out.back() = '\n';  // out không rỗng vì n >= 1
        cout << out;
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // Số bit B nhỏ nhất sao cho 2^B > n -> đủ để biểu diễn mọi giá trị trong [1..n].
    int B = 0;
    while ((1 << B) <= n) B++;   // với n <= 1000 thì B <= 10

    vector<int> a(n + 1, 0);     // hoán vị ẩn, đánh chỉ số từ 1

    // Mỗi truy vấn j xác định bit thứ j của tất cả a_i cùng một lúc.
    for (int j = 0; j < B; j++) {
        // Dựng truy vấn: b_k = bit thứ j của chỉ số k, với k = 1..n.
        string query;
        query.reserve(n);
        for (int k = 1; k <= n; k++)
            query.push_back(((k >> j) & 1) ? '1' : '0');

        cout << "? " << query << endl;   // endl flush stdout, bắt buộc với bài tương tác

        // Vị trí i của xâu trả lời chính là bit thứ j của a_i.
        string resp;
        cin >> resp;
        for (int i = 1; i <= n; i++)
            if (resp[i - 1] == '1') a[i] |= (1 << j);
    }

    // Ghép đủ các bit lại thành từng a_i rồi in đáp án cuối.
    cout << "!";
    for (int i = 1; i <= n; i++) cout << ' ' << a[i];
    cout << endl;                        // flush đáp án cuối
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> g(n);
    for (int i = 0; i < n; i++) cin >> g[i];

    const long long total = (long long)n * n;

    // CSR: với mỗi (hàng r, chữ cái c) lưu danh sách cột tăng dần để khỏi phải quét
    // lại cả lưới cho từng chữ cái. seg = r*k + c; off[seg]..off[seg+1] là đoạn cột
    // của (r,c) trong colBuf.
    // Lượt 1: đếm số cột của mỗi (hàng, chữ cái) (dịch phải 1 để tiện làm prefix).
    vector<int> off((long long)n * k + 1, 0);
    for (int r = 0; r < n; r++) {
        const string &row = g[r];
        int base = r * k;
        for (int col = 0; col < n; col++) {
            int c = row[col] - 'A';
            off[base + c + 1]++;
        }
    }
    // Prefix sum: off[i] trở thành vị trí bắt đầu của đoạn thứ i.
    for (int i = 1; i <= n * k; i++) off[i] += off[i - 1];

    // Lượt 2: điền các cột vào colBuf (cột < n <= 3000 nên vừa uint16_t).
    vector<uint16_t> colBuf(total);
    {
        vector<int> pos(off.begin(), off.end()); // con trỏ ghi cho từng đoạn
        for (int r = 0; r < n; r++) {
            const string &row = g[r];
            int base = r * k;
            for (int col = 0; col < n; col++) {
                int c = row[col] - 'A';
                colBuf[pos[base + c]++] = (uint16_t)col; // cột tăng dần trong đoạn
            }
        }
    }

    // mark[c1*n + c2] = ID chữ cái đánh dấu gần nhất cặp cột (c1<c2). Không reset
    // giữa các chữ cái: mỗi chữ cái có ID riêng nên so sánh == c chỉ đúng khi chính
    // chữ cái c đã đánh dấu cặp cột đó.
    vector<signed char> mark(total, -1);

    string out;
    out.reserve(k * 4);

    // Xử lý tuần tự từng chữ cái.
    for (int c = 0; c < k; c++) {
        bool found = false;
        for (int r = 0; r < n && !found; r++) {
            int seg = r * k + c;
            int s = off[seg], e = off[seg + 1];
            // Duyệt mỗi cặp cột (c1,c2) của chữ cái c trên hàng r.
            for (int i = s; i < e && !found; i++) {
                int c1 = colBuf[i];
                long long b = (long long)c1 * n;
                for (int j = i + 1; j < e; j++) {
                    long long p = b + colBuf[j];
                    // Cặp cột đã được chính chữ cái c đánh dấu ở hàng trước -> tìm
                    // được rectangle 4 góc -> YES.
                    if (mark[p] == (signed char)c) { found = true; break; }
                    mark[p] = (signed char)c;
                }
            }
        }
        out += (found ? "YES\n" : "NO\n");
    }

    cout << out;
    return 0;
}

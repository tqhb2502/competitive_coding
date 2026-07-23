#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    long long sum = 0;
    // Max-heap các cặp (bậc còn lại, chỉ số người chơi)
    priority_queue<pair<int, int>> pq;
    for (int i = 1; i <= n; ++i) {
        int x;
        cin >> x;
        sum += x;
        if (x > 0) pq.push({x, i});
    }

    bool possible = (sum % 2 == 0);  // Tổng bậc phải chẵn
    vector<pair<int, int>> edges;
    edges.reserve((size_t)(sum / 2) + 1);

    // Buffer tái sử dụng cho các hàng xóm lấy ra
    vector<pair<int, int>> taken;

    // Havel–Hakimi: mỗi bước lấy đỉnh chính có bậc lớn nhất
    while (possible && !pq.empty()) {
        pair<int, int> cur = pq.top();
        pq.pop();
        int d = cur.first;      // Bậc còn lại của đỉnh chính
        int u = cur.second;
        if (d == 0) break;      // Không còn nhu cầu

        if ((int)pq.size() < d) {  // Không đủ đối thủ để nối => IMPOSSIBLE
            possible = false;
            break;
        }

        // Nối u với đúng d người có bậc còn lại lớn nhất tiếp theo
        taken.clear();
        for (int j = 0; j < d; ++j) {
            taken.push_back(pq.top());
            pq.pop();
        }
        for (const auto &pr : taken) {
            int dv = pr.first;   // Bậc còn lại của hàng xóm (>= 1)
            int v = pr.second;
            edges.push_back({u, v});
            --dv;                // Giảm bậc của hàng xóm đi 1
            if (dv > 0) pq.push({dv, v});  // Đẩy lại nếu vẫn còn nhu cầu
        }
        // u đã có bậc 0 nên không đẩy lại (đã thỏa mãn hoàn toàn)
    }

    if (!possible) {
        cout << "IMPOSSIBLE\n";
    } else {
        cout << edges.size() << '\n';
        string out;
        out.reserve(edges.size() * 12);
        for (const auto &e : edges) {
            out += to_string(e.first);
            out += ' ';
            out += to_string(e.second);
            out += '\n';
        }
        cout << out;
    }
    return 0;
}

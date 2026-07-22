// Chess Tournament - CSES 1697
// https://cses.fi/problemset/task/1697
//
// Realize a degree sequence as a simple graph via Havel-Hakimi with a max-heap.
// Moi nguoi i can choi dung x_i van, moi cap toi da 1 van, khong tu choi.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    long long sum = 0;
    // max-heap cac cap (bac con lai, chi so nguoi choi)
    priority_queue<pair<int, int>> pq;
    for (int i = 1; i <= n; ++i) {
        int x;
        cin >> x;
        sum += x;
        if (x > 0) pq.push({x, i});
    }

    bool possible = (sum % 2 == 0);  // tong bac phai chan
    vector<pair<int, int>> edges;
    edges.reserve((size_t)(sum / 2) + 1);

    // buffer tai su dung cho cac hang xom lay ra
    vector<pair<int, int>> taken;

    while (possible && !pq.empty()) {
        pair<int, int> cur = pq.top();
        pq.pop();
        int d = cur.first;      // bac con lai cua dinh chinh
        int u = cur.second;
        if (d == 0) break;      // khong con nhu cau

        if ((int)pq.size() < d) {  // khong du doi thu de noi
            possible = false;
            break;
        }

        taken.clear();
        for (int j = 0; j < d; ++j) {
            taken.push_back(pq.top());
            pq.pop();
        }
        for (const auto &pr : taken) {
            int dv = pr.first;   // bac con lai cua hang xom (>= 1)
            int v = pr.second;
            edges.push_back({u, v});
            --dv;
            if (dv > 0) pq.push({dv, v});
        }
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

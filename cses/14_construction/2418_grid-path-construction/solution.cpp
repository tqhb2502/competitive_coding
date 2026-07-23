#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;

// Màu bàn cờ của ô (r,c): (r+c) mod 2.
inline int par(int r, int c) {
    return (r + c) & 1;
}

// ------------------------------------------------------------------
// Kiểm tra tính khả thi của miền chữ nhật [r0..r1] x [c0..c1] với hai đầu mút s,t.
// ------------------------------------------------------------------
bool feasibleRegion(int r0, int r1, int c0, int c1, int sr, int sc, int tr, int tc) {
    int H = r1 - r0 + 1, W = c1 - c0 + 1;
    long long area = (long long)H * W;
    int cornerpar = (r0 + c0) & 1;
    // Điều kiện màu: diện tích chẵn thì s,t phải KHÁC màu; diện tích lẻ thì cả
    // hai đều phải bằng màu góc (màu đa số).
    if (area % 2 == 0) {
        if (par(sr, sc) == par(tr, tc))
            return false;
    } else {
        if (par(sr, sc) != cornerpar || par(tr, tc) != cornerpar)
            return false;
    }
    // Ngoại lệ dải mỏng (F2): dải 2 hàng và s,t cùng một cột nằm trong (không
    // phải cột biên) thì miền bị cắt đôi; đối xứng cho dải 2 cột.
    if (H == 2 && sc == tc && sc > c0 && sc < c1)
        return false;
    if (W == 2 && sr == tr && sr > r0 && sr < r1)
        return false;
    // Lát cắt dọc (giữa cột j và j+1): điều kiện đếm-màu theo mỗi lát cắt.
    for (int j = c0; j < c1; j++) {
        int c0cnt = 0, c1cnt = 0;
        for (int col = c0; col <= j; col++)
            for (int r = r0; r <= r1; r++) {
                if (par(r, col) == 0)
                    c0cnt++;
                else
                    c1cnt++;
            }
        int D = c0cnt - c1cnt;  // hiệu (số ô màu 0 - số ô màu 1) của phần trái
        // Đếm đầu mút thật (s,t) nằm bên trái lát cắt, theo từng màu.
        int eL0 = 0, eL1 = 0, eL = 0;
        if (sc <= j) {
            eL++;
            if (par(sr, sc) == 0)
                eL0++;
            else
                eL1++;
        }
        if (tc <= j) {
            eL++;
            if (par(tr, tc) == 0)
                eL0++;
            else
                eL1++;
        }
        // a,b: số ô màu 0/1 trên cột j (các ô có thể "băng qua" lát cắt).
        int a = 0, b = 0;
        for (int r = r0; r <= r1; r++) {
            if (par(r, j) == 0)
                a++;
            else
                b++;
        }
        // Chọn số ô băng qua k và số ô băng qua màu 0/1 sao cho hiệu D bị ép khớp.
        int kmin = (eL == 0) ? 2 : 1;
        bool ok = false;
        for (int k = kmin; k <= H; k++) {
            if (((k - eL) & 1))  // k phải cùng chẵn/lẻ với số đầu mút thật bên trái
                continue;
            int num = 2 * D - (eL0 - eL1) + k;
            if (num & 1)
                continue;
            int cr0 = num / 2, cr1 = k - cr0;
            if (cr0 < 0 || cr1 < 0)
                continue;
            if (cr0 <= a && cr1 <= b) {
                ok = true;
                break;
            }
        }
        if (!ok)
            return false;
    }
    // Lát cắt ngang (giữa hàng i và i+1): đối xứng với lát cắt dọc.
    for (int i = r0; i < r1; i++) {
        int c0cnt = 0, c1cnt = 0;
        for (int rr = r0; rr <= i; rr++)
            for (int c = c0; c <= c1; c++) {
                if (par(rr, c) == 0)
                    c0cnt++;
                else
                    c1cnt++;
            }
        int D = c0cnt - c1cnt;
        int eL0 = 0, eL1 = 0, eL = 0;
        if (sr <= i) {
            eL++;
            if (par(sr, sc) == 0)
                eL0++;
            else
                eL1++;
        }
        if (tr <= i) {
            eL++;
            if (par(tr, tc) == 0)
                eL0++;
            else
                eL1++;
        }
        int a = 0, b = 0;
        for (int c = c0; c <= c1; c++) {
            if (par(i, c) == 0)
                a++;
            else
                b++;
        }
        int kmin = (eL == 0) ? 2 : 1;
        bool ok = false;
        for (int k = kmin; k <= W; k++) {
            if (((k - eL) & 1))
                continue;
            int num = 2 * D - (eL0 - eL1) + k;
            if (num & 1)
                continue;
            int cr0 = num / 2, cr1 = k - cr0;
            if (cr0 < 0 || cr1 < 0)
                continue;
            if (cr0 <= a && cr1 <= b) {
                ok = true;
                break;
            }
        }
        if (!ok)
            return false;
    }
    return true;
}

// ------------------------------------------------------------------
// Brute force (có cắt tỉa liên thông) cho lõi nhỏ còn lại.
// ------------------------------------------------------------------
static int DR[] = {-1, 1, 0, 0}, DC[] = {0, 0, -1, 1};
int BR0, BR1, BC0, BC1, BTR, BTC, BAREA;
char bvis[55][55];
vector<pii> bpath;
// Sau khi đặt một ô, kiểm tra phần chưa thăm vẫn liên thông và còn chứa đích t.
bool breach(int r, int c) {
    static char seen[55][55];
    for (int a = BR0; a <= BR1; a++)
        for (int b = BC0; b <= BC1; b++)
            seen[a][b] = 0;
    vector<pii> st = {{r, c}};
    seen[r][c] = 1;
    int cnt = 0;
    bool tgt = false;
    while (!st.empty()) {
        auto p = st.back();
        st.pop_back();
        cnt++;
        if (p.first == BTR && p.second == BTC)
            tgt = true;
        for (int d = 0; d < 4; d++) {
            int nr = p.first + DR[d], nc = p.second + DC[d];
            if (nr < BR0 || nr > BR1 || nc < BC0 || nc > BC1 || bvis[nr][nc] || seen[nr][nc])
                continue;
            seen[nr][nc] = 1;
            st.push_back({nr, nc});
        }
    }
    int rem = 0;
    for (int a = BR0; a <= BR1; a++)
        for (int b = BC0; b <= BC1; b++)
            if (!bvis[a][b])
                rem++;
    return cnt == rem && tgt;
}
// DFS thử mọi hướng, cắt tỉa bằng breach.
bool bdfs(int r, int c, int cnt) {
    bvis[r][c] = 1;
    bpath.push_back({r, c});
    if (cnt == BAREA) {
        if (r == BTR && c == BTC)
            return true;
        bvis[r][c] = 0;
        bpath.pop_back();
        return false;
    }
    for (int d = 0; d < 4; d++) {
        int nr = r + DR[d], nc = c + DC[d];
        if (nr < BR0 || nr > BR1 || nc < BC0 || nc > BC1 || bvis[nr][nc])
            continue;
        if (!breach(nr, nc))
            continue;
        if (bdfs(nr, nc, cnt + 1))
            return true;
    }
    bvis[r][c] = 0;
    bpath.pop_back();
    return false;
}
vector<pii> bruteRegion(int r0, int r1, int c0, int c1, int sr, int sc, int tr, int tc) {
    BR0 = r0;
    BR1 = r1;
    BC0 = c0;
    BC1 = c1;
    BTR = tr;
    BTC = tc;
    BAREA = (r1 - r0 + 1) * (c1 - c0 + 1);
    for (int a = r0; a <= r1; a++)
        for (int b = c0; b <= c1; b++)
            bvis[a][b] = 0;
    bpath.clear();
    if (bdfs(sr, sc, 1))
        return bpath;
    return {};
}

const int THRESH = 20;

// Dải 2 hàng [topRow,botRow]: đường "rắn bò" phủ trọn dải, đi từ (botRow,ca) tới
// (botRow,cb) với |ca-cb|=1.
vector<pii> stripRows(int topRow, int botRow, int c0, int c1, int ca, int cb) {
    vector<pii> sp;
    int lc = min(ca, cb);
    if (ca == lc) {
        for (int c = ca; c >= c0; c--)
            sp.push_back({botRow, c});
        for (int c = c0; c <= c1; c++)
            sp.push_back({topRow, c});
        for (int c = c1; c >= cb; c--)
            sp.push_back({botRow, c});
    } else {
        for (int c = ca; c <= c1; c++)
            sp.push_back({botRow, c});
        for (int c = c1; c >= c0; c--)
            sp.push_back({topRow, c});
        for (int c = c0; c <= cb; c++)
            sp.push_back({botRow, c});
    }
    return sp;
}
// Dải 2 cột [leftCol,rightCol]: đường "rắn bò" phủ trọn dải, đi từ (ra,rightCol)
// tới (rb,rightCol) với |ra-rb|=1.
vector<pii> stripCols(int leftCol, int rightCol, int r0, int r1, int ra, int rb) {
    vector<pii> sp;
    int tr_ = min(ra, rb);
    if (ra == tr_) {
        for (int r = ra; r >= r0; r--)
            sp.push_back({r, rightCol});
        for (int r = r0; r <= r1; r++)
            sp.push_back({r, leftCol});
        for (int r = r1; r >= rb; r--)
            sp.push_back({r, rightCol});
    } else {
        for (int r = ra; r <= r1; r++)
            sp.push_back({r, rightCol});
        for (int r = r1; r >= r0; r--)
            sp.push_back({r, leftCol});
        for (int r = r0; r <= rb; r++)
            sp.push_back({r, rightCol});
    }
    return sp;
}

// Xây dựng đường đi cho miền chữ nhật, đệ quy thu nhỏ về phía hai đầu mút.
vector<pii> build(int r0, int r1, int c0, int c1, int sr, int sc, int tr, int tc) {
    int H = r1 - r0 + 1, W = c1 - c0 + 1;
    // Miền 1 hàng: đi thẳng.
    if (H == 1) {
        vector<pii> res;
        int st = (tc >= sc) ? 1 : -1;
        for (int c = sc;; c += st) {
            res.push_back({r0, c});
            if (c == tc)
                break;
        }
        return res;
    }
    // Miền 1 cột: đi thẳng.
    if (W == 1) {
        vector<pii> res;
        int st = (tr >= sr) ? 1 : -1;
        for (int r = sr;; r += st) {
            res.push_back({r, c0});
            if (r == tr)
                break;
        }
        return res;
    }
    // Miền đủ nhỏ: brute force.
    if (H * W <= THRESH)
        return bruteRegion(r0, r1, c0, c1, sr, sc, tr, tc);
    int mnR = min(sr, tr), mxR = max(sr, tr), mnC = min(sc, tc), mxC = max(sc, tc);

    // --- Bóc dải biên dày 2 KHÔNG chứa đầu mút (giữ nguyên tính khả thi) ---
    // Giải đệ quy miền trong, rồi chèn dải "rắn bò" vào giữa một cạnh ngang/dọc
    // của đường đi miền trong nằm sát biên vừa bóc.
    if (H >= 4 && W >= 3 && mnR >= r0 + 2 && feasibleRegion(r0 + 2, r1, c0, c1, sr, sc, tr, tc)) {
        auto inner = build(r0 + 2, r1, c0, c1, sr, sc, tr, tc);
        for (int i = 0; i + 1 < (int)inner.size(); i++)
            if (inner[i].first == r0 + 2 && inner[i + 1].first == r0 + 2 &&
                abs(inner[i].second - inner[i + 1].second) == 1) {
                auto sp = stripRows(r0, r0 + 1, c0, c1, inner[i].second, inner[i + 1].second);
                vector<pii> res(inner.begin(), inner.begin() + i + 1);
                for (auto& p : sp)
                    res.push_back(p);
                for (int k = i + 1; k < (int)inner.size(); k++)
                    res.push_back(inner[k]);
                return res;
            }
    }
    if (H >= 4 && W >= 3 && mxR <= r1 - 2 && feasibleRegion(r0, r1 - 2, c0, c1, sr, sc, tr, tc)) {
        auto inner = build(r0, r1 - 2, c0, c1, sr, sc, tr, tc);
        for (int i = 0; i + 1 < (int)inner.size(); i++)
            if (inner[i].first == r1 - 2 && inner[i + 1].first == r1 - 2 &&
                abs(inner[i].second - inner[i + 1].second) == 1) {
                auto sp = stripRows(r1, r1 - 1, c0, c1, inner[i].second, inner[i + 1].second);
                vector<pii> res(inner.begin(), inner.begin() + i + 1);
                for (auto& p : sp)
                    res.push_back(p);
                for (int k = i + 1; k < (int)inner.size(); k++)
                    res.push_back(inner[k]);
                return res;
            }
    }
    if (W >= 4 && H >= 3 && mnC >= c0 + 2 && feasibleRegion(r0, r1, c0 + 2, c1, sr, sc, tr, tc)) {
        auto inner = build(r0, r1, c0 + 2, c1, sr, sc, tr, tc);
        for (int i = 0; i + 1 < (int)inner.size(); i++)
            if (inner[i].second == c0 + 2 && inner[i + 1].second == c0 + 2 &&
                abs(inner[i].first - inner[i + 1].first) == 1) {
                auto sp = stripCols(c0, c0 + 1, r0, r1, inner[i].first, inner[i + 1].first);
                vector<pii> res(inner.begin(), inner.begin() + i + 1);
                for (auto& p : sp)
                    res.push_back(p);
                for (int k = i + 1; k < (int)inner.size(); k++)
                    res.push_back(inner[k]);
                return res;
            }
    }
    if (W >= 4 && H >= 3 && mxC <= c1 - 2 && feasibleRegion(r0, r1, c0, c1 - 2, sr, sc, tr, tc)) {
        auto inner = build(r0, r1, c0, c1 - 2, sr, sc, tr, tc);
        for (int i = 0; i + 1 < (int)inner.size(); i++)
            if (inner[i].second == c1 - 2 && inner[i + 1].second == c1 - 2 &&
                abs(inner[i].first - inner[i + 1].first) == 1) {
                auto sp = stripCols(c1, c1 - 1, r0, r1, inner[i].first, inner[i + 1].first);
                vector<pii> res(inner.begin(), inner.begin() + i + 1);
                for (auto& p : sp)
                    res.push_back(p);
                for (int k = i + 1; k < (int)inner.size(); k++)
                    res.push_back(inner[k]);
                return res;
            }
    }

    // --- Không bóc được: tách một lát cắt đơn giữa hai đầu mút ---
    // s một phía, t phía kia, nối qua đúng một cạnh băng cắt; chọn vị trí cắt và
    // hàng/cột băng qua sao cho hai miền con đều khả thi, rồi đệ quy hai phía.
    if (sc != tc) {
        int lc = sc, rc = tc, lr = sr, rr = tr;
        if (sc > tc) {
            lc = tc;
            rc = sc;
            lr = tr;
            rr = sr;
        }
        for (int cut = lc; cut < rc; cut++)
            for (int a = r0; a <= r1; a++) {
                int lArea = (r1 - r0 + 1) * (cut - c0 + 1);
                if (lr == a && lc == cut && lArea != 1)
                    continue;
                int rArea = (r1 - r0 + 1) * (c1 - cut);
                if (rr == a && rc == cut + 1 && rArea != 1)
                    continue;
                if (!feasibleRegion(r0, r1, c0, cut, lr, lc, a, cut))
                    continue;
                if (!feasibleRegion(r0, r1, cut + 1, c1, a, cut + 1, rr, rc))
                    continue;
                auto L = build(r0, r1, c0, cut, lr, lc, a, cut);
                auto Rp = build(r0, r1, cut + 1, c1, a, cut + 1, rr, rc);
                if (L.empty() || Rp.empty())
                    continue;
                vector<pii> full = L;
                for (auto& p : Rp)
                    full.push_back(p);
                // Đảo chiều nếu s không nằm ở phía trái (để đường đi bắt đầu ở s).
                if (!(sr == lr && sc == lc))
                    reverse(full.begin(), full.end());
                return full;
            }
    }
    if (sr != tr) {
        int tr0 = sr, br0 = tr, tc0 = sc, bc0 = tc;
        if (sr > tr) {
            tr0 = tr;
            br0 = sr;
            tc0 = tc;
            bc0 = sc;
        }
        for (int cut = tr0; cut < br0; cut++)
            for (int a = c0; a <= c1; a++) {
                int tArea = (cut - r0 + 1) * (c1 - c0 + 1);
                if (tr0 == cut && tc0 == a && tArea != 1)
                    continue;
                int bArea = (r1 - cut) * (c1 - c0 + 1);
                if (br0 == cut + 1 && bc0 == a && bArea != 1)
                    continue;
                if (!feasibleRegion(r0, cut, c0, c1, tr0, tc0, cut, a))
                    continue;
                if (!feasibleRegion(cut + 1, r1, c0, c1, cut + 1, a, br0, bc0))
                    continue;
                auto T = build(r0, cut, c0, c1, tr0, tc0, cut, a);
                auto B = build(cut + 1, r1, c0, c1, cut + 1, a, br0, bc0);
                if (T.empty() || B.empty())
                    continue;
                vector<pii> full = T;
                for (auto& p : B)
                    full.push_back(p);
                if (!(sr == tr0 && sc == tc0))
                    reverse(full.begin(), full.end());
                return full;
            }
    }
    return {};  // không thể xảy ra với đầu vào khả thi
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t))
        return 0;
    string out;
    while (t--) {
        int n, m, y1, x1, y2, x2;
        cin >> n >> m >> y1 >> x1 >> y2 >> x2;
        // Đổi về chỉ số 0.
        int sr = y1 - 1, sc = x1 - 1, tr = y2 - 1, tc = x2 - 1;
        // Không khả thi -> in NO.
        if (!feasibleRegion(0, n - 1, 0, m - 1, sr, sc, tr, tc)) {
            out += "NO\n";
            continue;
        }
        auto path = build(0, n - 1, 0, m - 1, sr, sc, tr, tc);
        out += "YES\n";
        // Chuyển dãy ô liên tiếp thành hướng: hàng giảm=U, tăng=D, cột giảm=L, tăng=R.
        string mv;
        mv.reserve(path.size());
        for (size_t i = 1; i < path.size(); i++) {
            int dr = path[i].first - path[i - 1].first, dc = path[i].second - path[i - 1].second;
            if (dr == -1)
                mv += 'U';
            else if (dr == 1)
                mv += 'D';
            else if (dc == -1)
                mv += 'L';
            else
                mv += 'R';
        }
        out += mv;
        out += '\n';
    }
    cout << out;
    return 0;
}

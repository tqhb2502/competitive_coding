#include <bits/stdc++.h>
using namespace std;

// Cách lát cố định (hardcode) của hình chữ nhật 5x9 bằng L-tromino:
// id 1..15, mỗi id ứng với đúng một mảnh L. Đây là trường hợp cơ sở bất khả quy.
static const int P59[5][9] = {
    {1, 1, 2, 3, 3, 4, 4, 5, 5},
    {1, 2, 2, 3, 6, 4, 7, 7, 5},
    {8, 8, 9, 9, 6, 6, 7, 10, 10},
    {8, 11, 9, 12, 13, 13, 14, 10, 15},
    {11, 11, 12, 12, 13, 14, 14, 15, 15}};

int NID; // bộ đếm id tromino (mỗi mảnh đặt xuống mang một id duy nhất)

// Gán cùng một id cho 3 ô tạo thành một L-tromino.
inline void put3(vector<vector<int>> &A, int id,
                 int r0, int c0, int r1, int c1, int r2, int c2) {
    A[r0][c0] = id;
    A[r1][c1] = id;
    A[r2][c2] = id;
}

// Khối 2x3 tại hàng r, r+1 và cột c..c+2 -> tách thành 2 L-tromino.
void place2x3(vector<vector<int>> &A, int r, int c) {
    put3(A, ++NID, r, c, r, c + 1, r + 1, c);
    put3(A, ++NID, r, c + 2, r + 1, c + 1, r + 1, c + 2);
}

// Khối 3x2 tại hàng r..r+2 và cột c, c+1 -> tách thành 2 L-tromino.
void place3x2(vector<vector<int>> &A, int r, int c) {
    put3(A, ++NID, r, c, r, c + 1, r + 1, c);
    put3(A, ++NID, r + 1, c + 1, r + 2, c, r + 2, c + 1);
}

// Khối 5x6: dải 2x6 (hai khối 2x3) ở trên + dải 3x6 (ba khối 3x2) ở dưới.
void place5x6(vector<vector<int>> &A, int r, int c) {
    place2x3(A, r, c);
    place2x3(A, r, c + 3);
    place3x2(A, r + 2, c);
    place3x2(A, r + 2, c + 2);
    place3x2(A, r + 2, c + 4);
}

// Khối cơ sở 5x9 dùng cách lát cố định P59 (cấp id mới cho từng mảnh).
void place5x9(vector<vector<int>> &A, int r, int c) {
    int mp[16];
    for (int k = 1; k <= 15; k++) mp[k] = ++NID;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 9; j++)
            A[r + i][c + j] = mp[P59[i][j]];
}

// Xây dựng cách lát lưới n x m với điều kiện 3 | m (đã bảo đảm có nghiệm, n >= 2).
void build(vector<vector<int>> &A, int n, int m) {
    if (n % 2 == 0) {
        // Số hàng chẵn: phủ các khối 2x3 theo từng dải 2 hàng.
        for (int r = 0; r < n; r += 2)
            for (int c = 0; c < m; c += 3)
                place2x3(A, r, c);
    } else if (m % 2 == 0) {
        // Hàng lẻ, cột chẵn: băng 3 hàng đầu lát bằng khối 3x2, dưới là các dải 2 hàng.
        for (int c = 0; c < m; c += 2)
            place3x2(A, 0, c);
        for (int r = 3; r < n; r += 2)
            for (int c = 0; c < m; c += 3)
                place2x3(A, r, c);
    } else {
        // Hàng lẻ và cột lẻ (m là bội lẻ của 3, n >= 5).
        for (int r = 5; r < n; r += 2) // các dải 2 hàng phía dưới băng 5 hàng đầu
            for (int c = 0; c < m; c += 3)
                place2x3(A, r, c);
        for (int c = 0; c < m - 9; c += 6) // các khối 5x6 ở bên trái băng trên cùng
            place5x6(A, 0, c);
        place5x9(A, 0, m - 9); // khối cơ sở bất khả quy 5x9 ở bên phải
    }
}

// Kiểm tra điều kiện có nghiệm.
bool feasible(int R, int C) {
    if ((long long)R * C % 3 != 0) return false;
    int mn = min(R, C), mx = max(R, C);
    if (mn < 2) return false;
    if (mn == 3 && (mx % 2 == 1)) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    string out;
    out.reserve(1 << 20);

    while (t--) {
        int R, C;
        cin >> R >> C;
        if (!feasible(R, C)) {
            out += "NO\n";
            continue;
        }
        out += "YES\n";

        // Xoay lưới sao cho chiều rộng làm việc chia hết cho 3; ưu tiên chiều cao chẵn.
        bool canDirect = (C % 3 == 0);
        bool canTrans = (R % 3 == 0);
        bool transpose;
        if (canDirect && !canTrans)
            transpose = false;
        else if (canTrans && !canDirect)
            transpose = true;
        else { // cả hai chiều đều chia hết cho 3: cố gắng để chiều cao làm việc chẵn
            if (R % 2 == 0) transpose = false;
            else if (C % 2 == 0) transpose = true;
            else transpose = false;
        }

        int n = transpose ? C : R; // số hàng làm việc
        int m = transpose ? R : C; // số cột làm việc (3 | m)

        NID = 0;
        vector<vector<int>> A(n, vector<int>(m, 0));
        build(A, n, m);

        // Ánh xạ ngược id tromino về hướng xuất ra R x C.
        vector<vector<int>> G(R, vector<int>(C, 0));
        for (int i = 0; i < R; i++)
            for (int j = 0; j < C; j++)
                G[i][j] = transpose ? A[j][i] : A[i][j];

        // Thu thập các ô theo từng id tromino.
        vector<vector<pair<int, int>>> cells(NID + 1);
        for (int i = 0; i < R; i++)
            for (int j = 0; j < C; j++)
                cells[G[i][j]].push_back({i, j});

        // Tô màu tham lam hợp lệ trên đồ thị kề của các tromino.
        vector<int> col(NID + 1, 0); // 0 = chưa tô, ngược lại là chỉ số chữ cái 1..26
        static const int dx[4] = {-1, 1, 0, 0};
        static const int dy[4] = {0, 0, -1, 1};
        for (int id = 1; id <= NID; id++) {
            bool used[27] = {false};
            for (auto &p : cells[id]) {
                for (int d = 0; d < 4; d++) {
                    int ni = p.first + dx[d], nj = p.second + dy[d];
                    if (ni < 0 || ni >= R || nj < 0 || nj >= C) continue;
                    int oid = G[ni][nj];
                    if (oid != id && col[oid] != 0) used[col[oid]] = true;
                }
            }
            // Chọn chữ cái nhỏ nhất chưa bị các tromino kề cạnh dùng.
            int c = 1;
            while (used[c]) c++;
            col[id] = c;
        }

        // In lưới chữ cái kết quả.
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++)
                out += char('A' + col[G[i][j]] - 1);
            out += '\n';
        }
    }

    cout << out;
    return 0;
}

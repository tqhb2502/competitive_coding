// Filling Trominos - CSES 2423
// https://cses.fi/problemset/task/2423
//
// Tile an n x m grid with L-trominoes (three squares in an L shape) and print a
// letter grid where two edge-adjacent squares share a letter iff they belong to
// the same tromino. Print YES/NO.
//
// Feasibility (verified by brute force for all small boards, matches the known
// theorem): a solution exists iff
//   (n*m) % 3 == 0  AND  min(n,m) >= 2  AND  NOT ( min(n,m) == 3 && max(n,m) odd )
//
// Construction: since 3 | n*m we have 3 | n or 3 | m. Orient the board so the
// number of columns m is a multiple of 3, then tile with 2x3 / 3x2 blocks; the
// only irreducible "both dimensions odd" base case is a hardcoded 5x9 tiling.
// Finally color the trominoes greedily (adjacency graph has max degree <= 8, so
// <= 9 letters suffice).

#include <bits/stdc++.h>
using namespace std;

// Hardcoded L-tromino tiling of a 5x9 rectangle (ids 1..15, each id = one L).
static const int P59[5][9] = {
    {1, 1, 2, 3, 3, 4, 4, 5, 5},
    {1, 2, 2, 3, 6, 4, 7, 7, 5},
    {8, 8, 9, 9, 6, 6, 7, 10, 10},
    {8, 11, 9, 12, 13, 13, 14, 10, 15},
    {11, 11, 12, 12, 13, 14, 14, 15, 15}};

int NID; // running tromino id counter (unique per placed tromino)

inline void put3(vector<vector<int>> &A, int id,
                 int r0, int c0, int r1, int c1, int r2, int c2) {
    A[r0][c0] = id;
    A[r1][c1] = id;
    A[r2][c2] = id;
}

// 2x3 block at rows r,r+1 and cols c..c+2 -> two L-trominoes.
void place2x3(vector<vector<int>> &A, int r, int c) {
    put3(A, ++NID, r, c, r, c + 1, r + 1, c);
    put3(A, ++NID, r, c + 2, r + 1, c + 1, r + 1, c + 2);
}

// 3x2 block at rows r..r+2 and cols c,c+1 -> two L-trominoes.
void place3x2(vector<vector<int>> &A, int r, int c) {
    put3(A, ++NID, r, c, r, c + 1, r + 1, c);
    put3(A, ++NID, r + 1, c + 1, r + 2, c, r + 2, c + 1);
}

// 5x6 block: 2x6 strip (two 2x3) on top + 3x6 strip (three 3x2) below.
void place5x6(vector<vector<int>> &A, int r, int c) {
    place2x3(A, r, c);
    place2x3(A, r, c + 3);
    place3x2(A, r + 2, c);
    place3x2(A, r + 2, c + 2);
    place3x2(A, r + 2, c + 4);
}

// 5x9 base case using the hardcoded pattern.
void place5x9(vector<vector<int>> &A, int r, int c) {
    int mp[16];
    for (int k = 1; k <= 15; k++) mp[k] = ++NID;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 9; j++)
            A[r + i][c + j] = mp[P59[i][j]];
}

// Build a tiling of an n x m board with 3 | m (feasible, n >= 2).
void build(vector<vector<int>> &A, int n, int m) {
    if (n % 2 == 0) {
        // Even height: pack 2x3 blocks over 2-row strips.
        for (int r = 0; r < n; r += 2)
            for (int c = 0; c < m; c += 3)
                place2x3(A, r, c);
    } else if (m % 2 == 0) {
        // Odd height, even width: 3-row band of 3x2 blocks + 2-row strips below.
        for (int c = 0; c < m; c += 2)
            place3x2(A, 0, c);
        for (int r = 3; r < n; r += 2)
            for (int c = 0; c < m; c += 3)
                place2x3(A, r, c);
    } else {
        // Odd height and odd width (m is an odd multiple of 3, n >= 5).
        for (int r = 5; r < n; r += 2) // 2-row strips below the top 5 rows
            for (int c = 0; c < m; c += 3)
                place2x3(A, r, c);
        for (int c = 0; c < m - 9; c += 6) // 5x6 blocks on the left of the top band
            place5x6(A, 0, c);
        place5x9(A, 0, m - 9); // irreducible 5x9 base on the right
    }
}

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

        // Orient so working width is a multiple of 3; prefer even working height.
        bool canDirect = (C % 3 == 0);
        bool canTrans = (R % 3 == 0);
        bool transpose;
        if (canDirect && !canTrans)
            transpose = false;
        else if (canTrans && !canDirect)
            transpose = true;
        else { // both dims divisible by 3: try to make working height even
            if (R % 2 == 0) transpose = false;
            else if (C % 2 == 0) transpose = true;
            else transpose = false;
        }

        int n = transpose ? C : R; // working rows
        int m = transpose ? R : C; // working cols (3 | m)

        NID = 0;
        vector<vector<int>> A(n, vector<int>(m, 0));
        build(A, n, m);

        // Map back to output orientation (R x C) of tromino ids.
        vector<vector<int>> G(R, vector<int>(C, 0));
        for (int i = 0; i < R; i++)
            for (int j = 0; j < C; j++)
                G[i][j] = transpose ? A[j][i] : A[i][j];

        // Collect cells per tromino id.
        vector<vector<pair<int, int>>> cells(NID + 1);
        for (int i = 0; i < R; i++)
            for (int j = 0; j < C; j++)
                cells[G[i][j]].push_back({i, j});

        // Greedy proper coloring of the tromino adjacency graph.
        vector<int> col(NID + 1, 0); // 0 = uncolored, else letter index 1..26
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
            int c = 1;
            while (used[c]) c++;
            col[id] = c;
        }

        // Emit the letter grid.
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++)
                out += char('A' + col[G[i][j]] - 1);
            out += '\n';
        }
    }

    cout << out;
    return 0;
}

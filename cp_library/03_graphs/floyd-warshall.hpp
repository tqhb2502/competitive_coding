#pragma once
#include <bits/stdc++.h>
using namespace std;
// Floyd-Warshall — đường đi ngắn nhất MỌI CẶP, n nhỏ (<= ~500), O(n^3); phát hiện chu trình âm.
// Khi dùng: cần khoảng cách mọi cặp đỉnh, n nhỏ; hoặc đóng gói quan hệ đạt tới.
// ĐPT: O(n^3); bộ nhớ O(n^2). Khởi tạo d[i][j]=FW_INF (i!=j, chưa có cạnh), d[i][i]=0.
// Dùng: floyd(d); // cập nhật d tại chỗ; sau đó d[i][i] < 0 <=> có chu trình âm đi qua i
// Bẫy: bỏ qua trung gian k không tới (d[i][k]==INF) để tránh tràn; dùng long long; giữ min khi nạp cạnh song song.
// CSES: 1672
const long long FW_INF = (long long)4e18;
// Ý nghĩa: chạy Floyd-Warshall, cập nhật tại chỗ d[i][j] thành khoảng cách ngắn nhất mọi cặp đỉnh.
// Tham số: d = ma trận khoảng cách (0-based); d[i][i]=0, d[i][j]=FW_INF nếu chưa có cạnh trực tiếp.
void floyd(vector<vector<long long>>& d) {
    int n = (int)d.size();
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            if (d[i][k] < FW_INF)
                for (int j = 0; j < n; j++)
                    if (d[k][j] < FW_INF && d[i][k] + d[k][j] < d[i][j])
                        d[i][j] = d[i][k] + d[k][j];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ floyd-warshall.hpp -o demo && ./demo
int main() {
    int n = 4;
    vector<vector<long long>> d(n, vector<long long>(n, FW_INF));
    for (int i = 0; i < n; i++) d[i][i] = 0;
    d[0][1] = 4; d[1][2] = 1; d[0][2] = 10; d[2][3] = 2;
    floyd(d);
    printf("Floyd-Warshall: khoảng cách 0 -> 3 = %lld\n", d[0][3]);   // 7
    return 0;
}
#endif

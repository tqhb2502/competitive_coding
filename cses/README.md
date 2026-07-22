# CSES Problem Set — Lời giải

Kho lời giải cho [CSES Problemset](https://cses.fi/problemset/list).

Mỗi bài nằm trong một thư mục riêng, đặt theo nhóm:

```
cses/
  01_introductory/
    <task_id>_<slug>/
      problem_link.txt   # đường dẫn đến đề bài
      idea.txt           # ý tưởng giải (tiếng Việt có dấu)
      solution.py        # code (nhóm 01-09) HOẶC solution.cpp (từ nhóm 10)
```

> **Ngôn ngữ:** nhóm 01-09 dùng **Python thuần (stdlib)**. Từ **nhóm 10 trở đi
> chuyển sang C++** (`solution.cpp`) — vì CSES đặt giới hạn thời gian tối ưu cho
> C++, nên nhiều bài Python thuần dù đúng thuật toán vẫn TLE. Các lời giải Python
> có rủi ro TLE đều được ghi chú trong `idea.txt`.

## Tiến độ theo nhóm

| # | Nhóm | Số bài | Trạng thái |
|---|------|--------|------------|
| 01 | Introductory Problems | 24 | ✅ xong — 24/24 đúng thuật toán (22 PASS · 1 tối ưu chống TLE · 1 đúng nhưng cần C++/PyPy cho test nặng) |
| 02 | Sorting and Searching | 35 | ✅ xong — 35/35 đúng thuật toán (0 FAIL); 6 bài rủi ro TLE Python ghi chú trong idea.txt |
| 03 | Dynamic Programming | 23 | ✅ xong — 23/23 đúng thuật toán, **thuần stdlib** (đã bỏ numpy); nhiều bài DP O(n·x) sẽ TLE Python trên CSES (ghi chú trong idea.txt, cần C++/PyPy) |
| 04 | Graph Algorithms | 36 | ✅ xong — 36/36 đúng thuật toán, thuần stdlib (DFS/BFS lặp, tránh RecursionError); 7 bài rủi ro TLE Python ghi chú trong idea.txt |
| 05 | Range Queries | 25 | ✅ xong — 25/25 đúng thuật toán, thuần stdlib (BIT/Fenwick, segment tree lặp, sparse table); nhiều bài rủi ro TLE Python (2e5 truy vấn × log) ghi chú trong idea.txt |
| 06 | Tree Algorithms | 16 | ✅ xong — 16/16 đúng thuật toán, thuần stdlib (DFS/Euler tour lặp, binary lifting LCA, centroid decomposition); 8 bài rủi ro TLE Python ghi chú trong idea.txt |
| 07 | Mathematics | 37 | ✅ xong — 37/37 đúng thuật toán, thuần stdlib (pow modulo, nCr, lũy thừa ma trận, sàng, Sprague-Grundy, GF(2)); 3 bài rủi ro TLE ghi chú trong idea.txt |
| 08 | String Algorithms | 21 | ✅ xong — 21/21 đúng thuật toán, thuần stdlib (KMP/Z, suffix array + LCP, suffix automaton, Manacher, Aho-Corasick, trie; tránh hashing đơn); nhiều bài rủi ro TLE Python ghi chú trong idea.txt |
| 09 | Geometry | 16 | ✅ xong — 16/16 ĐÚNG (đã review độc lập, brute-force đối chiếu), thuần stdlib + số học nguyên chính xác; 8 bài rủi ro TLE Python (gồm 1742 Robot Path) ghi chú trong idea.txt |
| 10 | Advanced Techniques | 25 | ✅ **C++** — 25/25 verify, compile OK, **0 TLE-risk**, 0 FAIL (24 verify kỹ + 2143 verify nhanh) |
| 11+ | Sliding Window / Interactive / Bitwise / Construction / Advanced Graph / Counting / Additional I & II | ~110 | ⬜ chưa làm |

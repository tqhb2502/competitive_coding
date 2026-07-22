# CSES Problem Set — Lời giải

Kho lời giải cho [CSES Problemset](https://cses.fi/problemset/list).

Mỗi bài nằm trong một thư mục riêng, đặt theo nhóm:

```
cses/
  01_introductory/          # nhóm 01-09: có cả solution.py và solution.cpp
    <task_id>_<slug>/
      problem_link.txt   # đường dẫn đến đề bài
      idea.txt           # ý tưởng giải (tiếng Việt có dấu)
      solution.py        # code Python (nhóm 01-09)
      solution.cpp       # code C++  (nhóm 01-09 đã bổ sung; từ nhóm 10 chỉ có .cpp)
```

> **Ngôn ngữ:**
> - **Nhóm 01-09** ban đầu giải bằng **Python thuần (stdlib)**, nay đã **bổ sung bản
>   C++** (`solution.cpp`) cho cả **233 bài** — mỗi thư mục có cả `solution.py` và
>   `solution.cpp`.
> - **Nhóm 10 trở đi** dùng **C++** (`solution.cpp`) là chính.
>
> Lý do có C++: CSES đặt giới hạn thời gian tối ưu cho C++, nên nhiều bài Python
> thuần dù đúng thuật toán vẫn TLE (đã ghi chú trong `idea.txt`); bản C++ khắc phục.

## Tiến độ theo nhóm

| # | Nhóm | Số bài | Trạng thái |
|---|------|--------|------------|
| 01 | Introductory Problems | 24 | ✅ **Python + C++** — 24/24 đúng thuật toán (22 PASS · 1 tối ưu chống TLE · 1 đúng nhưng cần C++/PyPy cho test nặng) |
| 02 | Sorting and Searching | 35 | ✅ **Python + C++** — 35/35 đúng thuật toán (0 FAIL); 6 bài rủi ro TLE Python ghi chú trong idea.txt |
| 03 | Dynamic Programming | 23 | ✅ **Python + C++** — 23/23 đúng thuật toán, **thuần stdlib** (đã bỏ numpy); nhiều bài DP O(n·x) sẽ TLE Python trên CSES (ghi chú trong idea.txt, cần C++/PyPy) |
| 04 | Graph Algorithms | 36 | ✅ **Python + C++** — 36/36 đúng thuật toán, thuần stdlib (DFS/BFS lặp, tránh RecursionError); 7 bài rủi ro TLE Python ghi chú trong idea.txt |
| 05 | Range Queries | 25 | ✅ **Python + C++** — 25/25 đúng thuật toán, thuần stdlib (BIT/Fenwick, segment tree lặp, sparse table); nhiều bài rủi ro TLE Python (2e5 truy vấn × log) ghi chú trong idea.txt |
| 06 | Tree Algorithms | 16 | ✅ **Python + C++** — 16/16 đúng thuật toán, thuần stdlib (DFS/Euler tour lặp, binary lifting LCA, centroid decomposition); 8 bài rủi ro TLE Python ghi chú trong idea.txt |
| 07 | Mathematics | 37 | ✅ **Python + C++** — 37/37 đúng thuật toán, thuần stdlib (pow modulo, nCr, lũy thừa ma trận, sàng, Sprague-Grundy, GF(2)); 3 bài rủi ro TLE ghi chú trong idea.txt |
| 08 | String Algorithms | 21 | ✅ **Python + C++** — 21/21 đúng thuật toán, thuần stdlib (KMP/Z, suffix array + LCP, suffix automaton, Manacher, Aho-Corasick, trie; tránh hashing đơn); nhiều bài rủi ro TLE Python ghi chú trong idea.txt |
| 09 | Geometry | 16 | ✅ **Python + C++** — 16/16 ĐÚNG (đã review độc lập, brute-force đối chiếu), thuần stdlib + số học nguyên chính xác; 8 bài rủi ro TLE Python (gồm 1742 Robot Path) ghi chú trong idea.txt |
| 10 | Advanced Techniques | 25 | ✅ **C++** — 25/25 verify, compile OK, **0 TLE-risk**, 0 FAIL (24 verify kỹ + 2143 verify nhanh) |
| 11 | Sliding Window Problems | 11 | ✅ **C++** — 11/11 compile OK + verify nhanh (3 PASS · 8 FIXED · 0 FAIL), 0 TLE-risk (monotonic deque, prefix, two heaps, BIT...) |
| 12 | Interactive Problems | 6 | ✅ **C++** — 6/6 PASS (0 FIXED · 0 FAIL); mỗi bài test qua interactor giả (đúng đáp án, số truy vấn ≤ giới hạn, flush OK, không deadlock) |
| 13 | Bitwise Operations | 11 | ✅ **C++** — 11/11 compile OK + verify nhanh (1 PASS · 10 FIXED · 0 FAIL), 0 TLE-risk (XOR basis, XOR trie, SOS DP, WHT, Sierpinski) |
| 14 | Construction Problems | 8 | ✅ **C++** — 8/8 verify (validator kiểm tính hợp lệ + khả thi); 2418 Grid Path cross-check brute-force mọi lưới ≤4×4 (0 sai) |
| 15 | Advanced Graph Problems | ~28 | ⬜ chưa làm |
| 16 | Counting Problems | 18 | ✅ **C++** — 18/18 compile + sample; oracle vét cạn/invariant cho cả 18 bài, ASan/UBSan sạch; 0 TLE-risk |
| 17 | Additional Problems I | ~28 | ⬜ chưa làm |
| 18 | Additional Problems II | ~28 | ⬜ chưa làm |

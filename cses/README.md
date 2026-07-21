# CSES Problem Set — Lời giải

Kho lời giải cho [CSES Problemset](https://cses.fi/problemset/list).

Mỗi bài nằm trong một thư mục riêng, đặt theo nhóm:

```
cses/
  01_introductory/
    <task_id>_<slug>/
      problem_link.txt   # đường dẫn đến đề bài
      idea.txt           # ý tưởng giải (tiếng Việt)
      solution.py        # code Python
```

> Lưu ý: CSES đặt giới hạn thời gian tối ưu cho C++. Một số bài (Range Queries,
> Graph, Advanced...) code Python thuần đúng thuật toán vẫn có thể TLE. Những bài
> có rủi ro TLE sẽ được ghi chú trong `idea.txt`.

## Tiến độ theo nhóm

| # | Nhóm | Số bài | Trạng thái |
|---|------|--------|------------|
| 01 | Introductory Problems | 24 | ✅ xong — 24/24 đúng thuật toán (22 PASS · 1 tối ưu chống TLE · 1 đúng nhưng cần C++/PyPy cho test nặng) |
| 02 | Sorting and Searching | 35 | ✅ xong — 35/35 đúng thuật toán (0 FAIL); 6 bài rủi ro TLE Python ghi chú trong idea.txt |
| 03 | Dynamic Programming | 23 | ✅ xong — 23/23 đúng thuật toán, **thuần stdlib** (đã bỏ numpy); nhiều bài DP O(n·x) sẽ TLE Python trên CSES (ghi chú trong idea.txt, cần C++/PyPy) |
| 04 | Graph Algorithms | 36 | ✅ xong — 36/36 đúng thuật toán, thuần stdlib (DFS/BFS lặp, tránh RecursionError); 7 bài rủi ro TLE Python ghi chú trong idea.txt |
| 05 | Range Queries | 25 | ✅ xong — 25/25 đúng thuật toán, thuần stdlib (BIT/Fenwick, segment tree lặp, sparse table); nhiều bài rủi ro TLE Python (2e5 truy vấn × log) ghi chú trong idea.txt |
| 06 | Tree Algorithms | ~16 | ⬜ chưa làm |
| 07 | Mathematics | ~31 | ⬜ chưa làm |
| 08 | String Algorithms | ~17 | ⬜ chưa làm |
| 09 | Geometry | ~16 | ⬜ chưa làm |
| 10 | Advanced Techniques | ~26 | ⬜ chưa làm |
| 11+ | Sliding Window / Interactive / Bitwise / Construction / Advanced Graph / Counting / Additional I & II | ~110 | ⬜ chưa làm |

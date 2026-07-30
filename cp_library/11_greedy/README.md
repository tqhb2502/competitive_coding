# 11 — Greedy (tham lam trên đoạn / lịch biểu)

6 snippet. Tệp này **tự sinh** bởi `tools/gen_docs.py` từ header các `.hpp` — đừng sửa tay.
Tra theo dấu hiệu đề: [indexes/by-use-case.md](../indexes/by-use-case.md) · cây quyết định: [indexes/decision-trees.md](../indexes/decision-trees.md) · bẫy: [indexes/pitfalls.md](../indexes/pitfalls.md)

| Kỹ thuật | Mục đích | Bài CSES | File |
|---|---|---|---|
| Greedy Array | ba mẫu exchange/invariant: làm dãy không giảm, tổng xu thiếu nhỏ nhất, hai máy đọc việc | [1087 Shortest Subsequence](https://cses.fi/problemset/task/1087)<br>[1092 Two Sets](https://cses.fi/problemset/task/1092)<br>[1094 Increasing Array](https://cses.fi/problemset/task/1094)<br>[1631 Reading Books](https://cses.fi/problemset/task/1631)<br>[1743 String Reorder](https://cses.fi/problemset/task/1743)<br>[2180 Coin Arrangement](https://cses.fi/problemset/task/2180)<br>[2183 Missing Coin Sum](https://cses.fi/problemset/task/2183)<br>[3175 Beautiful Permutation II](https://cses.fi/problemset/task/3175) | [greedy-array.hpp](greedy-array.hpp) |
| Heap Scheduling | greedy lịch biểu dùng heap/multiset: k người/máy xem được tối đa bao nhiêu đoạn, và thứ tự việc tối ưu | [1630 Tasks and Deadlines](https://cses.fi/problemset/task/1630)<br>[1632 Movie Festival II](https://cses.fi/problemset/task/1632) | [heap-scheduling.hpp](heap-scheduling.hpp) |
| Heap Selection | tổng top-k trên mọi prefix/suffix, chọn hai vai trò qua điểm chia, và mô phỏng k vòng bubble bằng heap lookah… | [2426 Programmers and Artists](https://cses.fi/problemset/task/2426)<br>[3152 Bubble Sort Rounds II](https://cses.fi/problemset/task/3152) | [heap-selection.hpp](heap-selection.hpp) |
| Interval Scheduling | greedy trên đoạn: chọn tối đa đoạn không giao (theo điểm KẾT THÚC) và số phòng tối thiểu = độ chồng lấp lớn n… | [1164 Room Allocation](https://cses.fi/problemset/task/1164)<br>[1629 Movie Festival](https://cses.fi/problemset/task/1629) | [interval-scheduling.hpp](interval-scheduling.hpp) |
| Optimal Merge (Huffman) | luôn ghép hai trọng số nhỏ nhất để tối thiểu tổng chi phí ghép | [1161 Stick Divisions](https://cses.fi/problemset/task/1161) | [optimal-merge.hpp](optimal-merge.hpp) |
| Path Matching | greedy hối tiếc/co cạnh cho min-cost matching theo mọi lực lượng trên đường đi | [3402 Minimum Cost Pairs](https://cses.fi/problemset/task/3402) | [path-matching.hpp](path-matching.hpp) |

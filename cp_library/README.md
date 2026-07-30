# CP Library — kho snippet thi đấu

Kho code C++17 để dùng **khi đang thi**: tìm nhanh theo dấu hiệu đề, dán vào lời giải,
và **gộp một file để nộp**. Mỗi kỹ thuật là **một file `.hpp` tự chứa**, đầu file có
comment ngắn: *mục đích · khi nào dùng · độ phức tạp · cách dùng · bẫy*.

**Toolchain mục tiêu là GNU g++ 64-bit.** Trên Windows 11 dùng MinGW-w64
(khuyến nghị w64devkit) + Git Bash; MSVC/`cl.exe` và `clang-cl` không được hỗ trợ
vì thư viện dùng `bits/stdc++.h`, `__int128` và GNU PBDS.

## Dùng nhanh khi thi

1. Chép [`templates/base.cpp`](templates/base.cpp) thành `main.cpp` (fast IO + chỗ include snippet).
2. Tìm kỹ thuật — nhanh nhất là **bảng triệu chứng đề bài**:
   - [indexes/by-use-case.md](indexes/by-use-case.md) — "đề nói gì → snippet nào" (một bảng, quét mắt là ra);
   - [indexes/decision-trees.md](indexes/decision-trees.md) — chọn theo nhánh khi còn do dự;
   - [indexes/by-complexity.md](indexes/by-complexity.md) — `n` cho phép làm gì (đọc ràng buộc là biết);
   - [indexes/cses.md](indexes/cses.md) — tra hai chiều đủ 400 bài CSES: tên bài → tag/snippet và snippet → danh sách bài;
   - biết tên rồi: `grep -ril "fenwick" cp_library` (hoặc `rg -i fenwick cp_library`).
3. `#include "cp_library/02_data_structures/fenwick.hpp"` trong `main.cpp` (hoặc dán thẳng nội dung file),
   đọc 5 dòng header + khối doc của hàm sắp gọi (**đặc biệt dòng `Bẫy:`**).
4. Biên dịch: `g++ -std=c++17 -O2 -Wall -Wextra -I . main.cpp -o sol`.
5. Gộp nộp: `python3 cp_library/tools/bundle.py main.cpp -I . -o submit.cpp` → nộp `submit.cpp`.

`bundle.py` inline mọi `#include "..."` nội bộ, giữ nguyên `#include <...>`, bỏ `#pragma once`
và tự bọc guard — output là một file `.cpp` nộp thẳng được (cần Python ≥ 3.9;
trên Windows có thể thay `python3` bằng `python` hoặc `py -3`).

**Chú ý thư mục làm việc:** đường dẫn include `cp_library/...` và `-I .` ở trên giả định bạn đứng ở
**thư mục cha của `cp_library`** (gốc repo). Ở nơi khác thì `-I` trỏ vào đúng thư mục cha đó, ví dụ
`-I /d/competitive_coding`. Bốn lỗi `bundle.py` chủ động báo (đọc là biết ngay cách sửa):

| Thông báo | Nghĩa |
|---|---|
| `quoted include not found` | sai đường dẫn, hoặc thiếu `-I` trỏ tới thư mục cha của `cp_library` |
| `include escapes allowed roots` | include đi ra ngoài mọi `-I` (dùng `..` quá nhiều) |
| `absolute quoted include is not allowed` | đừng dùng `#include "d:/..."`, phải là đường dẫn tương đối |
| `output must not overwrite the source or an included file` | `-o` đang trỏ vào chính `main.cpp` hoặc một snippet |

Khi bí:

- **Sai ở test ẩn?** → [`templates/stress.sh`](templates/stress.sh) so lời giải với brute trên test ngẫu nhiên:
  `bash cp_library/templates/stress.sh main.cpp brute.cpp gen.cpp 500` (sửa `gen.cpp`/`brute.cpp` cho khớp đề).
- **Cần xem biến?** → `#include "cp_library/templates/debug.hpp"` rồi `dbg(x, v);`, biên dịch thêm `-DCP_DEBUG`
  (khi nộp không có cờ này nên `dbg` tự biến mất).
- **Chuẩn bị nộp?** → quét [indexes/pitfalls.md](indexes/pitfalls.md) mục "14 bẫy chí tử".
- **Quên cú pháp C++/STL?** → [indexes/cpp-idioms.md](indexes/cpp-idioms.md) (I/O nhanh, tràn số, comparator,
  `bitset`, `nth_element`, bẫy nhiều test case…).

## Bảng tra cứu (đang có)

### 01 — Foundations

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Simulation | mô phỏng chuyển trạng thái; dò chu kỳ Floyd | [simulation.hpp](01_foundations/simulation.hpp) |
| Frequency Counting | tần suất, RLE, mex | [frequency-counting.hpp](01_foundations/frequency-counting.hpp) |
| Prefix Sum | tổng đoạn mảng tĩnh, không cập nhật | [prefix-sum.hpp](01_foundations/prefix-sum.hpp) |
| Prefix XOR | XOR đoạn mảng tĩnh trong O(1) | [prefix-xor.hpp](01_foundations/prefix-xor.hpp) |
| Difference Array | cộng đoạn nhiều lần (offline) rồi đọc kết quả | [difference-array.hpp](01_foundations/difference-array.hpp) |
| Binary Search | lower/upper bound và tìm biên vị từ đơn điệu | [binary-search.hpp](01_foundations/binary-search.hpp) |
| Coordinate Compression | nén giá trị lớn về hạng 0..k-1 | [coordinate-compression.hpp](01_foundations/coordinate-compression.hpp) |
| Two Pointers | cặp/đoạn trên mảng đã sắp, hai con trỏ một chiều | [two-pointers.hpp](01_foundations/two-pointers.hpp) |
| Sliding Window | cửa sổ co giãn, điều kiện đơn điệu theo độ dài | [sliding-window.hpp](01_foundations/sliding-window.hpp) |
| Monotonic Stack | phần tử nhỏ/lớn hơn gần nhất, histogram | [monotonic-stack.hpp](01_foundations/monotonic-stack.hpp) |
| Monotonic Deque | min/max mọi cửa sổ độ dài k | [monotonic-deque.hpp](01_foundations/monotonic-deque.hpp) |
| Submask Enumeration | duyệt submask, SOS DP, cặp bù | [submask-enumeration.hpp](01_foundations/submask-enumeration.hpp) |
| Max Subarray (Kadane) | đoạn con liền kề có tổng lớn nhất (kèm biên) | [max-subarray.hpp](01_foundations/max-subarray.hpp) |
| Gray Code | sinh dãy mã Gray / đổi qua lại số ↔ mã Gray | [gray-code.hpp](01_foundations/gray-code.hpp) |
| Bit Operations | popcount/Hamming, đóng gói bit, đếm bit tiền tố | [bit-operations.hpp](01_foundations/bit-operations.hpp) |
| Dynamic Bitset | bitset runtime, `dp \|= dp << x`, AND + popcount | [dynamic-bitset.hpp](01_foundations/dynamic-bitset.hpp) |
| Permutation Cycles | chu trình, nghịch đảo và bậc của hoán vị | [permutation-cycles.hpp](01_foundations/permutation-cycles.hpp) |
| Backtracking | khung chọn–hoàn tác, prune và đếm nghiệm | [backtracking.hpp](01_foundations/backtracking.hpp) |
| Comparison Sort | merge sort ổn định qua oracle, đếm query | [comparison-sort.hpp](01_foundations/comparison-sort.hpp) |
| Permutation Code | hoán vị ↔ Lehmer/prefix-inversion code | [permutation-code.hpp](01_foundations/permutation-code.hpp) |
| Sweep Line | thứ tự event/tie và kiểm chứa đoạn | [sweep-line.hpp](01_foundations/sweep-line.hpp) |

### 02 — Data Structures

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| DSU (Union-Find) | hợp nhất / cùng thành phần (chỉ thêm cạnh), Kruskal | [dsu.hpp](02_data_structures/dsu.hpp) |
| Weighted DSU | ràng buộc hiệu thế, kiểm mâu thuẫn online | [weighted-dsu.hpp](02_data_structures/weighted-dsu.hpp) |
| Rollback DSU | DSU có undo (dynamic connectivity offline) | [rollback-dsu.hpp](02_data_structures/rollback-dsu.hpp) |
| Fenwick Tree | update điểm + tổng đoạn | [fenwick.hpp](02_data_structures/fenwick.hpp) |
| Fenwick Tree 2D | update điểm + tổng hình chữ nhật trên lưới | [fenwick-2d.hpp](02_data_structures/fenwick-2d.hpp) |
| Sparse Table | min/max/gcd đoạn tĩnh, query O(1) | [sparse-table.hpp](02_data_structures/sparse-table.hpp) |
| Disjoint Sparse Table | phép kết hợp bất kỳ (gcd/product), query O(1) | [disjoint-sparse-table.hpp](02_data_structures/disjoint-sparse-table.hpp) |
| Segment Tree (iterative) | update điểm + truy vấn đoạn theo monoid | [segment-tree.hpp](02_data_structures/segment-tree.hpp) |
| Lazy Segment Tree | update đoạn + truy vấn đoạn (mẫu: add/sum) | [lazy-segment-tree.hpp](02_data_structures/lazy-segment-tree.hpp) |
| Trie | đếm từ theo tiền tố (bảng chữ cái) | [trie.hpp](02_data_structures/trie.hpp) |
| Binary Trie | max XOR / truy vấn theo bit | [binary-trie.hpp](02_data_structures/binary-trie.hpp) |
| Dynamic Segment Tree | segtree trên miền lớn ~1e9, tạo node khi cần | [dynamic-segment-tree.hpp](02_data_structures/dynamic-segment-tree.hpp) |
| Persistent Segment Tree | k-th nhỏ nhất trong đoạn / nhiều phiên bản | [persistent-segment-tree.hpp](02_data_structures/persistent-segment-tree.hpp) |
| Li Chao Tree | min/max các đường a·x+b, tối ưu DP | [li-chao-tree.hpp](02_data_structures/li-chao-tree.hpp) |
| Convex Hull Trick | min/max đường khi slope & query đơn điệu | [convex-hull-trick.hpp](02_data_structures/convex-hull-trick.hpp) |
| Treap | multiset thống kê thứ tự (kth / countLess) tự cài | [treap.hpp](02_data_structures/treap.hpp) |
| Implicit Treap | dãy động: đảo đoạn / cut-paste + tổng đoạn | [implicit-treap.hpp](02_data_structures/implicit-treap.hpp) |
| Ordered Set (PBDS) | find_by_order / order_of_key, chỉ GCC | [ordered-set.hpp](02_data_structures/ordered-set.hpp) |
| Wavelet Matrix | k-th trong đoạn / rank trên mảng tĩnh | [wavelet-matrix.hpp](02_data_structures/wavelet-matrix.hpp) |
| Median Maintenance | multiset động: median dưới + tổng khoảng cách | [median-maintenance.hpp](02_data_structures/median-maintenance.hpp) |
| Run Intervals | duy trì các run cực đại khi đổi một ký tự | [run-intervals.hpp](02_data_structures/run-intervals.hpp) |
| Interval Set | hợp các đoạn nguyên động: thêm, xóa, tìm điểm đã phủ | [interval-set.hpp](02_data_structures/interval-set.hpp) |
| Leftist Heap | priority queue cần meld hai heap nhanh | [leftist-heap.hpp](02_data_structures/leftist-heap.hpp) |
| Window Mode | thêm/xóa và lấy mode của cửa sổ, tie lấy giá trị nhỏ nhất | [window-mode.hpp](02_data_structures/window-mode.hpp) |

### 03 — Graphs

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| BFS | đường đi ngắn nhất đồ thị không trọng số, thành phần liên thông | [bfs.hpp](03_graphs/bfs.hpp) |
| DFS Iterative | flood fill/rừng DFS lớn, tránh tràn stack | [dfs.hpp](03_graphs/dfs.hpp) |
| Topological Sort | thứ tự phụ thuộc / DP trên DAG / phát hiện chu trình | [topological-sort.hpp](03_graphs/topological-sort.hpp) |
| Dijkstra | đường đi ngắn nhất, trọng số không âm | [dijkstra.hpp](03_graphs/dijkstra.hpp) |
| 0-1 BFS | đường đi ngắn nhất khi cạnh chỉ 0/1 | [bfs-01.hpp](03_graphs/bfs-01.hpp) |
| Bellman-Ford | cạnh âm + phát hiện chu trình âm | [bellman-ford.hpp](03_graphs/bellman-ford.hpp) |
| Floyd-Warshall | mọi cặp khoảng cách, n nhỏ | [floyd-warshall.hpp](03_graphs/floyd-warshall.hpp) |
| SCC (Tarjan) | thành phần liên thông mạnh + rút gọn thành DAG | [scc.hpp](03_graphs/scc.hpp) |
| 2-SAT | ràng buộc "chọn 1 trong 2" / mệnh đề (a OR b) | [two-sat.hpp](03_graphs/two-sat.hpp) |
| Cầu & Khớp | cạnh/đỉnh mà bỏ đi làm mất liên thông (vô hướng) | [bridges-articulation.hpp](03_graphs/bridges-articulation.hpp) |
| MST (Kruskal) | cây khung nhỏ nhất / rừng khung nhỏ nhất | [mst.hpp](03_graphs/mst.hpp) |
| Đường đi Euler | Hierholzer có hướng/vô hướng, hỗ trợ cạnh song song | [eulerian-path.hpp](03_graphs/eulerian-path.hpp) |
| Dinic (max-flow) | luồng cực đại / lát cắt nhỏ nhất / đường cạnh-rời | [dinic.hpp](03_graphs/dinic.hpp) |
| Min-Cost Max-Flow | luồng cực đại có tổng chi phí nhỏ nhất | [min-cost-max-flow.hpp](03_graphs/min-cost-max-flow.hpp) |
| Hopcroft-Karp | ghép đôi cực đại đồ thị hai phía | [hopcroft-karp.hpp](03_graphs/hopcroft-karp.hpp) |
| Hungarian | phân công chi phí nhỏ nhất (assignment) | [hungarian.hpp](03_graphs/hungarian.hpp) |
| Tìm chu trình | IN RA chu trình (có hướng / vô hướng), phát hiện chu trình | [find-cycle.hpp](03_graphs/find-cycle.hpp) |
| K Shortest Paths | k walk ngắn nhất với trọng số không âm | [k-shortest-paths.hpp](03_graphs/k-shortest-paths.hpp) |
| Dominator Tree | đỉnh nằm trên mọi đường từ nguồn | [dominator-tree.hpp](03_graphs/dominator-tree.hpp) |
| Block-Cut Tree | vertex-BCC và truy vấn tránh một đỉnh | [block-cut-tree.hpp](03_graphs/block-cut-tree.hpp) |
| Edge Orientation | hướng cạnh thành DAG hoặc mọi outdegree chẵn | [edge-orientation.hpp](03_graphs/edge-orientation.hpp) |
| Havel–Hakimi | dựng đồ thị đơn từ dãy bậc | [havel-hakimi.hpp](03_graphs/havel-hakimi.hpp) |

### 04 — Trees

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Binary Lifting tổng quát | nhảy hàm `next`; LCA + gộp monoid cạnh trên đường đi | [binary-lifting.hpp](04_trees/binary-lifting.hpp) |
| LCA (Binary Lifting) | tổ tiên chung gần nhất / tổ tiên thứ k / khoảng cách | [lca-binary-lifting.hpp](04_trees/lca-binary-lifting.hpp) |
| Euler Tour | làm phẳng cây con thành đoạn (ghép Fenwick/segtree) | [euler-tour.hpp](04_trees/euler-tour.hpp) |
| Đường kính cây | đường đi dài nhất / đỉnh xa nhất trên cây | [tree-diameter.hpp](04_trees/tree-diameter.hpp) |
| Rerooting | tính đáp án cho mọi đỉnh làm gốc (tổng khoảng cách…) | [rerooting.hpp](04_trees/rerooting.hpp) |
| HLD | truy vấn/cập nhật trên đường đi & cây con | [hld.hpp](04_trees/hld.hpp) |
| Centroid Decomposition | đếm đường đi theo độ dài / bài "qua trọng tâm" | [centroid-decomposition.hpp](04_trees/centroid-decomposition.hpp) |
| Small-to-Large | tổng hợp trên mọi cây con (DSU on tree) | [small-to-large.hpp](04_trees/small-to-large.hpp) |
| Tree DP | ghép cặp trên cây, tổng/max khoảng cách mọi gốc (DFS lặp) | [tree-dp.hpp](04_trees/tree-dp.hpp) |
| Mã Prüfer | cây có nhãn ↔ dãy Prüfer (đếm cây, sinh cây) | [prufer.hpp](04_trees/prufer.hpp) |
| Tree Augmentation | thêm ít nhất cạnh để mọi cạnh cây nằm trên chu trình | [tree-augmentation.hpp](04_trees/tree-augmentation.hpp) |
| Tree Isomorphism | AHU chính xác cho cây có/không gốc | [tree-isomorphism.hpp](04_trees/tree-isomorphism.hpp) |
| Tree Reconstruction | preorder + inorder → cây nhị phân/postorder | [tree-reconstruction.hpp](04_trees/tree-reconstruction.hpp) |
| Long-Path Decomposition | đếm cặp đỉnh cách đúng k cạnh trên cây trong O(n) | [long-path-decomposition.hpp](04_trees/long-path-decomposition.hpp) |

### 05 — Dynamic Programming

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| LIS | dãy con tăng dài nhất, O(n log n) | [lis.hpp](05_dynamic_programming/lis.hpp) |
| Knapsack 0/1 + bounded | chọn tập tối ưu theo sức chứa; mỗi loại dùng một hoặc hữu hạn lần | [knapsack.hpp](05_dynamic_programming/knapsack.hpp) |
| Coin Change | min số xu / đếm cách (xu không giới hạn) | [coin-change.hpp](05_dynamic_programming/coin-change.hpp) |
| Edit Distance | khoảng cách chỉnh sửa hai xâu | [edit-distance.hpp](05_dynamic_programming/edit-distance.hpp) |
| Digit DP | đếm số theo ràng buộc chữ số trong đoạn | [digit-dp.hpp](05_dynamic_programming/digit-dp.hpp) |
| Bitmask DP | n nhỏ (≤20): TSP, đường Hamilton, phủ tập con | [bitmask-dp.hpp](05_dynamic_programming/bitmask-dp.hpp) |
| LCS | dãy con chung dài nhất hai xâu | [lcs.hpp](05_dynamic_programming/lcs.hpp) |
| Interval DP | DP trên đoạn (trò lấy hai đầu, gộp đoạn) | [interval-dp.hpp](05_dynamic_programming/interval-dp.hpp) |
| SOS DP | tổng trên mọi tập con/tập cha của mask | [sos-dp.hpp](05_dynamic_programming/sos-dp.hpp) |
| Slope Trick | chi phí \|a_i − b_i\| ít nhất để dãy không giảm / tăng | [slope-trick.hpp](05_dynamic_programming/slope-trick.hpp) |
| Broken Profile DP | lấp bảng n×m bằng domino, DP mặt nạ theo cột | [broken-profile-dp.hpp](05_dynamic_programming/broken-profile-dp.hpp) |
| Linear DP | rolling state và các mẫu DP quét tiền tố | [linear-dp.hpp](05_dynamic_programming/linear-dp.hpp) |
| Grid DP | đường đơn điệu và hình vuông đồng giá trị | [grid-dp.hpp](05_dynamic_programming/grid-dp.hpp) |
| Subset Sum Trace | bitset 0/1 subset-sum kèm truy vết chỉ số | [subset-sum-trace.hpp](05_dynamic_programming/subset-sum-trace.hpp) |
| Weighted Interval Scheduling | chọn các khoảng có trọng số không giao nhau, kèm truy vết | [weighted-interval-scheduling.hpp](05_dynamic_programming/weighted-interval-scheduling.hpp) |

### 06 — Strings

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| KMP | tìm mẫu / biên (border) / chu kỳ | [kmp.hpp](06_strings/kmp.hpp) |
| Z-function | khớp mẫu / so tiền tố nhanh | [z-function.hpp](06_strings/z-function.hpp) |
| KMP Automaton | DP trên xâu theo mẫu (đếm xâu chứa/không chứa mẫu) | [kmp-automaton.hpp](06_strings/kmp-automaton.hpp) |
| String Hashing | so đoạn con bằng nhau trong O(1) | [string-hashing.hpp](06_strings/string-hashing.hpp) |
| Manacher | palindrome dài nhất / đếm mọi palindrome | [manacher.hpp](06_strings/manacher.hpp) |
| Suffix Array | xâu con phân biệt / lặp / thứ tự hậu tố | [suffix-array.hpp](06_strings/suffix-array.hpp) |
| Aho-Corasick | khớp nhiều mẫu cùng lúc trong văn bản | [aho-corasick.hpp](06_strings/aho-corasick.hpp) |
| Suffix Automaton | đếm/vị trí/k-th substring theo distinct hoặc multiplicity | [suffix-automaton.hpp](06_strings/suffix-automaton.hpp) |
| Minimal Rotation | phép quay nhỏ nhất của xâu tuần hoàn | [min-rotation.hpp](06_strings/min-rotation.hpp) |
| Eertree | palindrome phân biệt / dài nhất kết thúc tại i | [eertree.hpp](06_strings/eertree.hpp) |
| Burrows–Wheeler Transform | BWT và inverse BWT bằng LF mapping | [bwt.hpp](06_strings/bwt.hpp) |
| Distinct Subsequences | đếm subsequence phân biệt, loại trùng | [distinct-subsequences.hpp](06_strings/distinct-subsequences.hpp) |

### 07 — Mathematics

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Integer Math | căn nguyên, valuation, LCM có chặn, digit query | [integer-math.hpp](07_mathematics/integer-math.hpp) |
| Modular Arithmetic | a^e mod m / nghịch đảo modulo (mod nguyên tố) | [modular-arithmetic.hpp](07_mathematics/modular-arithmetic.hpp) |
| Combinatorics | C(n,r) / P(n,r) mod nguyên tố, O(1) truy vấn | [combinatorics.hpp](07_mathematics/combinatorics.hpp) |
| Linear Sieve | nguyên tố / phân tích thừa số / hàm nhân tính | [sieve.hpp](07_mathematics/sieve.hpp) |
| Matrix Exponentiation | hồi quy tuyến tính / đếm đường độ dài k | [matrix-exponentiation.hpp](07_mathematics/matrix-exponentiation.hpp) |
| Extended Euclid | nghịch đảo mod bất kỳ / CRT / a·x+b·y=g | [extended-euclid.hpp](07_mathematics/extended-euclid.hpp) |
| Factorization | nguyên tố / phân tích thừa số n tới ~1e18 | [factorization.hpp](07_mathematics/factorization.hpp) |
| Gaussian Elimination | giải hệ tuyến tính mod nguyên tố | [gaussian-elimination.hpp](07_mathematics/gaussian-elimination.hpp) |
| Mobius & Euler Phi | nghịch đảo Mobius / đếm nguyên tố cùng nhau | [mobius.hpp](07_mathematics/mobius.hpp) |
| NTT + CRT | tích chập hai modulus và hệ số nguyên không âm chính xác | [ntt.hpp](07_mathematics/ntt.hpp) |
| Sprague-Grundy | trò chơi Nim / Grundy (mex) | [sprague-grundy.hpp](07_mathematics/sprague-grundy.hpp) |
| XOR Basis | XOR lớn nhất / đếm giá trị XOR / XOR nhỏ thứ k của tập con | [xor-basis.hpp](07_mathematics/xor-basis.hpp) |
| Walsh-Hadamard | tích chập XOR (c[k] = Σ a[i]·b[j], i^j = k) | [walsh-hadamard.hpp](07_mathematics/walsh-hadamard.hpp) |
| Divisor Blocks | tổng dạng Σ f(i)·⌊n/i⌋ với n tới 1e12 | [divisor-blocks.hpp](07_mathematics/divisor-blocks.hpp) |
| Lucas | C(n, k) mod p nguyên tố NHỎ, n tới 1e18 (kèm chẵn/lẻ) | [lucas.hpp](07_mathematics/lucas.hpp) |
| Inclusion-Exclusion | đếm "chia hết bởi ít nhất một" / derangement | [inclusion-exclusion.hpp](07_mathematics/inclusion-exclusion.hpp) |
| Burnside | đếm cấu hình trên vòng khi QUAY/phản xạ coi là giống | [burnside.hpp](07_mathematics/burnside.hpp) |
| Josephus | vòng tròn loại người thứ k: thứ tự loại / người sống sót | [josephus.hpp](07_mathematics/josephus.hpp) |
| Kth Permutation | hoán vị thứ k ↔ hạng của hoán vị (cơ số giai thừa) | [kth-permutation.hpp](07_mathematics/kth-permutation.hpp) |
| Divisor Sieve | đếm bội và số subset có gcd chính xác | [divisor-sieve.hpp](07_mathematics/divisor-sieve.hpp) |
| Semiring Matrix | lũy thừa ma trận tổng quát/min-plus | [semiring-matrix.hpp](07_mathematics/semiring-matrix.hpp) |
| Probability & Expectation | tích chập phân phối, CDF/indicator, random walk | [probability-expectation.hpp](07_mathematics/probability-expectation.hpp) |
| Labeled Combinatorics | phân phối thành phần functional graph/tournament | [labeled-combinatorics.hpp](07_mathematics/labeled-combinatorics.hpp) |
| Eulerian Numbers | đếm hoán vị theo số descent/round | [eulerian-numbers.hpp](07_mathematics/eulerian-numbers.hpp) |

### 08 — Geometry

Các file hình học chia sẻ `Point`/`cross`/`orient` qua guard `CP_GEO_BASE` — dán một file vẫn tự chứa, bundle nhiều file không định nghĩa lại.

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Geometry Basics | cross / dot / orient / điểm trên đoạn | [geometry-basics.hpp](08_geometry/geometry-basics.hpp) |
| Convex Hull | bao lồi tập điểm (CCW) | [convex-hull.hpp](08_geometry/convex-hull.hpp) |
| Polygon Area & Pick | diện tích + điểm nguyên biên/trong (Pick) | [polygon-area.hpp](08_geometry/polygon-area.hpp) |
| Point in Polygon | điểm trong/trên/ngoài đa giác (winding) | [point-in-polygon.hpp](08_geometry/point-in-polygon.hpp) |
| Segment Intersection | hai đoạn có giao nhau không | [segment-intersection.hpp](08_geometry/segment-intersection.hpp) |
| Closest Pair | cặp điểm gần nhau nhất (d² chính xác) | [closest-pair.hpp](08_geometry/closest-pair.hpp) |
| Rotating Calipers | đường kính / cặp điểm xa nhất | [rotating-calipers.hpp](08_geometry/rotating-calipers.hpp) |
| Line Intersection | giao điểm hai đường thẳng | [line-intersection.hpp](08_geometry/line-intersection.hpp) |
| Manhattan Distance | đường kính online và tổng L1 mọi cặp | [manhattan-distance.hpp](08_geometry/manhattan-distance.hpp) |

### 09 — Offline Algorithms

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Mo's Algorithm | nhiều truy vấn đoạn tĩnh, add/remove rẻ | [mo-algorithm.hpp](09_offline_algorithms/mo-algorithm.hpp) |
| Sqrt Decomposition | range-update + range-sum theo khối | [sqrt-decomposition.hpp](09_offline_algorithms/sqrt-decomposition.hpp) |
| Offline Dynamic Connectivity | cạnh thêm/xóa theo thời gian + truy vấn liên thông | [offline-dynamic-connectivity.hpp](09_offline_algorithms/offline-dynamic-connectivity.hpp) |
| CDQ Divide & Conquer | thứ tự bộ phận 3 chiều (thời gian, x, y) | [cdq-divide-conquer.hpp](09_offline_algorithms/cdq-divide-conquer.hpp) |
| Offline LCA (Tarjan) | mọi truy vấn LCA trong một lượt DFS + DSU | [offline-lca.hpp](09_offline_algorithms/offline-lca.hpp) |
| Parallel Binary Search | chặt nhị phân đồng thời mọi truy vấn theo thời gian | [parallel-binary-search.hpp](09_offline_algorithms/parallel-binary-search.hpp) |

### 10 — Advanced Techniques

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Meet in the Middle | n ≤ ~40: đếm/tìm tập con theo tổng | [meet-in-the-middle.hpp](10_advanced_techniques/meet-in-the-middle.hpp) |
| Ternary / Convex Search | cực tiểu hàm lồi trên miền nguyên (`convexMin` chịu được plateau) | [ternary-search.hpp](10_advanced_techniques/ternary-search.hpp) |
| Functional Graph | mỗi đỉnh 1 cạnh ra: nhảy k bước, chu trình, khoảng cách | [functional-graph.hpp](10_advanced_techniques/functional-graph.hpp) |
| Bitset Reachability | bao đóng bắc cầu DAG (đếm/hỏi đỉnh tới được) | [bitset-reachability.hpp](10_advanced_techniques/bitset-reachability.hpp) |
| Kruskal Reconstruction Tree | bottleneck trên đường / ngày sớm nhất liên thông | [kruskal-reconstruction-tree.hpp](10_advanced_techniques/kruskal-reconstruction-tree.hpp) |
| Divide & Conquer DP | chia k đoạn, opt đơn điệu → O(k·n log n) | [divide-conquer-dp.hpp](10_advanced_techniques/divide-conquer-dp.hpp) |
| Knuth Optimization | DP đoạn cắt/gộp thỏa bất đẳng thức tứ giác → O(n²) | [knuth-optimization.hpp](10_advanced_techniques/knuth-optimization.hpp) |
| Constructive Algorithms | các khối dựng hoán vị/cấu hình theo invariant | [constructive.hpp](10_advanced_techniques/constructive.hpp) |
| Interactive Search | query budget, callback I/O và truy vấn bit song song | [interactive-search.hpp](10_advanced_techniques/interactive-search.hpp) |
| K-best Enumeration | k tổng subset nhỏ nhất, có số âm | [k-best-enumeration.hpp](10_advanced_techniques/k-best-enumeration.hpp) |
| K-best Combinations | k tổng nhỏ nhất của subset đúng m phần tử | [k-best-combinations.hpp](10_advanced_techniques/k-best-combinations.hpp) |
| Multiset Reconstruction | dựng dãy từ multiset tổng mọi cặp | [multiset-reconstruction.hpp](10_advanced_techniques/multiset-reconstruction.hpp) |

### 11 — Greedy

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Interval Scheduling | chọn tối đa đoạn không giao; số phòng ít nhất + gán phòng | [interval-scheduling.hpp](11_greedy/interval-scheduling.hpp) |
| Heap Scheduling | k người xem tối đa mấy đoạn; thứ tự việc tối ưu (SPT) | [heap-scheduling.hpp](11_greedy/heap-scheduling.hpp) |
| Greedy Array | tăng dãy, missing sum và phân tải hai máy | [greedy-array.hpp](11_greedy/greedy-array.hpp) |
| Optimal Merge | Huffman/ghép hai trọng số nhỏ nhất | [optimal-merge.hpp](11_greedy/optimal-merge.hpp) |
| Heap Selection | top-k prefix/suffix và bubble lookahead | [heap-selection.hpp](11_greedy/heap-selection.hpp) |
| Path Matching | min-cost matching theo mọi cardinality trên path | [path-matching.hpp](11_greedy/path-matching.hpp) |

**Đủ 11 nhóm.** Mỗi snippet tự biên dịch, có demo chạy được (`#ifdef CP_DEMO`) và link ngược `// CSES:`.
Các API lõi/dễ lỗi biên còn được kiểm bằng property test đối chiếu brute/oracle trong
[`tests/regression.cpp`](tests/regression.cpp).

## Tra ngược từ bài CSES

Cả 400 bài trong [`cses/`](../cses) có dòng `Tags:` gần đầu `idea.txt` liệt kê kỹ thuật lời giải dùng.
Tra tên bài, nhóm, tag và link idea tập trung tại [`indexes/cses.md`](indexes/cses.md):

```sh
rg -l '^Tags: .*\bfenwick\b' cses --glob idea.txt   # những bài dùng Fenwick
```

Tên tag trùng tên file `.hpp` (id kỹ thuật) cộng vài bổ ngữ trong
[`tools/tag_vocab.txt`](tools/tag_vocab.txt). Kỹ thuật chuẩn chưa có bài CSES trực tiếp phải nằm trong
[`tools/cses_unmapped_allowlist.txt`](tools/cses_unmapped_allowlist.txt) kèm lý do; CI chặn ngoại lệ ngầm
hoặc ngoại lệ đã lỗi thời.

## Cấu trúc

```text
cp_library/
├── 01_foundations/ … 11_greedy/   # mỗi kỹ thuật một .hpp (+ README từng nhóm, TỰ SINH)
├── indexes/     # by-use-case (triệu chứng → snippet), by-complexity (n → ngân sách), cpp-idioms (mẹo C++),
│                # decision-trees (chọn theo nhánh), cses (400 bài ↔ snippet), pitfalls (bẫy)
├── templates/   # base / graph / geometry / interactive + debug.hpp, gen.cpp, brute.cpp, stress.sh
├── tests/       # property/regression test đối chiếu brute cho các API lõi và ca biên
└── tools/       # bundle.py (gộp 1 file), verify.sh, gen_docs.py, check_docs.py,
                 # link_cses.py, check_cses_tags.py, tag_vocab.txt
```

Phần tra cứu tự sinh (bảng "Khi dùng"/ĐPT/Bẫy trong `indexes/`, README từng nhóm) do
[`tools/gen_docs.py`](tools/gen_docs.py) tạo **trực tiếp từ header các `.hpp`** nên không bao giờ lệch code;
`verify.sh` chạy `gen_docs.py --check` để chặn lệch. Phần viết tay: `README.md`, `CONTRIBUTING.md`,
`indexes/decision-trees.md`, bảng triệu chứng + cheat-sheet + "14 bẫy chí tử".

## Quy ước snippet

- `#pragma once` + `#include <bits/stdc++.h>` + `using namespace std;` — tự chứa, dán là chạy.
- Tên type/hàm **duy nhất & rõ nghĩa** (DSU, Fenwick, SegTree…) để bundle nhiều file không đụng nhau;
  tránh hàm free tên chung (`gcd`, `Edge`…).
- Comment header 5 dòng: *mục đích · khi dùng · độ phức tạp · cách dùng · bẫy*.
  Identifier tiếng Anh, comment tiếng Việt.
- Ghi rõ **index 0-based/1-based**, kiểu số / overflow, và identity của phép gộp.
- Mỗi entry có **demo chạy được** trong `#ifdef CP_DEMO int main(){...}#endif` (không phá bundle/include):
  `g++ -std=c++17 -DCP_DEMO -x c++ cp_library/02_data_structures/dsu.hpp -o demo && ./demo`.
- Dòng `// CSES: <mã bài>` (sinh bởi `tools/link_cses.py` từ `Tags:`) liệt kê bài CSES dùng kỹ thuật đó.

## Kiểm chứng

```sh
export PATH="$HOME/Downloads/w64devkit/bin:$PATH"   # để ưu tiên đúng g++ (Windows)
bash cp_library/tools/verify.sh
```

`verify.sh` làm 6 việc: (1) biên dịch độc lập mọi `.hpp` + chạy mọi demo `CP_DEMO`; (2) biên dịch/chạy
mọi mẫu trong `templates/` + `debug.hpp` (bật và tắt `CP_DEBUG`) + smoke test `stress.sh`; (3) kiểm luồng
bundle-1-file và các đường dẫn bị cấm; (4) biên dịch/chạy mọi `tests/*.cpp`; (5) kiểm tag CSES
(`check_cses_tags.py`, `link_cses.py --check`); (6) kiểm **tài liệu khớp hệ thống tệp**
(`check_docs.py`: README/indexes phủ đủ snippet, không link chết, mọi `.hpp` đủ header +
`// CSES:` + demo + khối doc hàm; `gen_docs.py --check`: phần tự sinh không lệch header).
Yêu cầu: **bash thật** (Git Bash — bash rút gọn của w64devkit không đủ), `g++` và Python ≥ 3.9.
Script tự tìm `python3`, `python`, rồi `py -3`; cũng có thể đặt biến `PYTHON` thành đường dẫn interpreter.
Đang sửa nhanh và muốn vòng lặp ngắn? `FAST=1 bash cp_library/tools/verify.sh` (biên dịch `-O0`).
Khi audit tạm trên host không có GNU PBDS (ví dụ Apple Clang), có thể thêm
`SKIP_GNU_ONLY=1`; chế độ này báo rõ và chỉ bỏ compile `ordered-set.hpp`, không thay thế lượt verify
đầy đủ bằng MinGW/GCC.

Tính đúng đắn của từng snippet dựa vào **stress-test đối chiếu brute/oracle** (n nhỏ, hàng nghìn–hàng trăm
nghìn vòng, kèm ca biên và ca quy mô lớn) cộng **review đối kháng** — quy trình này đã bắt được lỗi thật:
MLE 3.7 GB do mảng đếm theo giá trị, tràn `long long` khi mod > 3e9, tràn `__int128` có dấu gần 2^64,
và ternary search sai khi hàm có plateau. Máy Windows/w64devkit không có ASan/UBSan.

## Thêm snippet mới

Xem [CONTRIBUTING.md](CONTRIBUTING.md).

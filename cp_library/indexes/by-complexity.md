# Tra theo độ phức tạp

Đọc ràng buộc `n` → biết được **phép làm gì** → chọn snippet. Con số dưới đây là ước lượng thận trọng
cho C++17, giới hạn 1–2 giây (khoảng 1e8–5e8 thao tác đơn giản), KHÔNG phải cam kết: hằng số lớn
(map, set, đệ quy sâu, chia lấy dư) có thể chậm hơn 5–10 lần.

## n cho phép làm gì

| n | Ngân sách | Thường là | Snippet tiêu biểu |
|---:|---|---|---|
| ≤ 10 | n! ≈ 3.6e6 | duyệt mọi hoán vị | brute + `next_permutation` |
| ≤ 20 | 2^n · n ≈ 2e7 | DP trên tập con | `bitmask-dp`, `submask-enumeration`, `sos-dp` |
| ≤ 40 | 2^(n/2) ≈ 1e6 | chia đôi rồi ghép | `meet-in-the-middle` |
| ≤ 100 | n^4 = 1e8 | DP nhiều chiều, Floyd | `floyd-warshall`, `matrix-exponentiation` |
| ≤ 500 | n^3 = 1.25e8 | Floyd, Gauss, luồng nhỏ | `gaussian-elimination`, `hungarian`, `dinic` |
| ≤ 3 000 | n^2 = 9e6 | DP hai chiều thoải mái | `edit-distance`, `lcs`, `interval-dp`, `knuth-optimization`, `divide-conquer-dp` |
| ≤ 10 000 | n^2 = 1e8 (sát) | DP hai chiều tiết kiệm | `knapsack` (n·W), `bitset-reachability` |
| ≤ 2e5 | n log^2 n ≈ 1e8 | cây/đoạn nhiều tầng | `hld`, `centroid-decomposition`, `mo-algorithm`, `cdq-divide-conquer`, `offline-dynamic-connectivity`, `parallel-binary-search`, `small-to-large` |
| ≤ 2e5 | n log n ≈ 4e6 | sort + cấu trúc 1 tầng | `fenwick`, `segment-tree`, `lazy-segment-tree`, `dijkstra`, `mst`, `suffix-array`, `lis`, `convex-hull`, `closest-pair`, `kruskal-reconstruction-tree`, `lca-binary-lifting`, `functional-graph`, `sparse-table`, `disjoint-sparse-table`, `dynamic-segment-tree`, `persistent-segment-tree`, `wavelet-matrix`, `treap`, `implicit-treap`, `ordered-set`, `binary-trie`, `li-chao-tree`, `convex-hull-trick`, `sqrt-decomposition`, `ntt`, `combinatorics`, `mobius`, `sieve` |
| ≤ 1e6 | n hoặc n·26 | tuyến tính, một lượt | `prefix-sum`, `difference-array`, `two-pointers`, `sliding-window`, `monotonic-stack`, `monotonic-deque`, `coordinate-compression`, `bfs`, `bfs-01`, `topological-sort`, `scc`, `two-sat`, `bridges-articulation`, `eulerian-path`, `dsu`, `rollback-dsu`, `trie`, `kmp`, `z-function`, `string-hashing`, `manacher`, `aho-corasick`, `suffix-automaton`, `eertree`, `min-rotation`, `euler-tour`, `tree-diameter`, `rerooting`, `offline-lca`, `coin-change`, `digit-dp`, `sprague-grundy`, `geometry-basics`, `polygon-area`, `point-in-polygon`, `segment-intersection`, `line-intersection`, `rotating-calipers`, `binary-search`, `ternary-search`, `fenwick-2d`, `hopcroft-karp`, `min-cost-max-flow`, `bellman-ford`, `factorization`, `extended-euclid`, `modular-arithmetic`, `gaussian-elimination`, `hld` |
| ≤ 1e18 | O(log n) hoặc O(n^{1/4}) | không được duyệt hết | `modular-arithmetic`, `extended-euclid`, `matrix-exponentiation`, `factorization`, `digit-dp`, `functional-graph` |

Ghi chú nhanh:

- `n·m` với lưới 1000×1000 = 1e6 → BFS/DP trên lưới luôn ổn.
- Có `q` truy vấn: nhân thêm `q`. `q·log n` với `q, n ≤ 2e5` ≈ 3.5e6 → thoải mái; `q·sqrt(n)` ≈ 9e7 → sát.
- Đệ quy sâu > ~1e5 khung dễ tràn stack: các snippet trong kho đã chuyển sang **DFS lặp** ở những chỗ nguy hiểm.
- Bộ nhớ: 512 MB ≈ 1.3e8 `int` hoặc 6.7e7 `long long`. `bitset-reachability` tốn `n²/8` byte (n = 5e4 → ~312 MB!).

## Bảng độ phức tạp đầy đủ theo nhóm

<!-- GEN:BEGIN (tự sinh bởi tools/gen_docs.py — đừng sửa tay) -->

### 01 — Nền tảng (mảng, đoạn, hai con trỏ, bitmask)

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `backtracking` | O(số node trạng thái thực sự duyệt * chi phí sinh ứng viên); bộ nhớ O(depth + trạng thái). | [backtracking.hpp](../01_foundations/backtracking.hpp) |
| `binary-search` | O(log n) cho sorted search; O(log(hi-lo+1)) lần gọi pred; bộ nhớ O(1). | [binary-search.hpp](../01_foundations/binary-search.hpp) |
| `bit-operations` | pack O(\|s\|), countSetBitsUpTo O(64), các phép còn lại O(1); bộ nhớ O(1). | [bit-operations.hpp](../01_foundations/bit-operations.hpp) |
| `comparison-sort` | stableOracleMergeSort O(n log n) phép gán và tối đa n*ceil(log2(n))-2^ceil(log2(n))+1 phép so sánh; bộ nhớ O(n). | [comparison-sort.hpp](../01_foundations/comparison-sort.hpp) |
| `coordinate-compression` | build O(n log n); id/val O(log n); bộ nhớ O(n). | [coordinate-compression.hpp](../01_foundations/coordinate-compression.hpp) |
| `difference-array` | add O(1); build O(n); bộ nhớ O(n). | [difference-array.hpp](../01_foundations/difference-array.hpp) |
| `dynamic-bitset` | set/test O(1); shiftOr/countAnd O(n/64); bộ nhớ O(n/64). | [dynamic-bitset.hpp](../01_foundations/dynamic-bitset.hpp) |
| `frequency-counting` | frequencyTable kỳ vọng O(n), RLE O(n), mex O(n); bộ nhớ O(số giá trị phân biệt hoặc n). | [frequency-counting.hpp](../01_foundations/frequency-counting.hpp) |
| `gray-code` | grayCode O(2^n · n) thời gian, bộ nhớ O(2^n · n) cho kết quả; grayEncode O(1); grayDecode = grayRank O(log W) = 6 phép xor với W = 64 bit. | [gray-code.hpp](../01_foundations/gray-code.hpp) |
| `max-subarray` | O(n) một lượt quét, không tiền xử lý; bộ nhớ O(1) ngoài mảng đầu vào. | [max-subarray.hpp](../01_foundations/max-subarray.hpp) |
| `monotonic-deque` | O(n) — mỗi index vào/ra deque một lần; bộ nhớ O(k). | [monotonic-deque.hpp](../01_foundations/monotonic-deque.hpp) |
| `monotonic-stack` | O(n) — mỗi index push và pop tối đa một lần. | [monotonic-stack.hpp](../01_foundations/monotonic-stack.hpp) |
| `permutation-code` | mọi phép encode/decode O(n log n); bộ nhớ O(n), dùng Fenwick tần suất + order statistic. | [permutation-code.hpp](../01_foundations/permutation-code.hpp) |
| `permutation-cycles` | mọi hàm O(n); bộ nhớ O(n). | [permutation-cycles.hpp](../01_foundations/permutation-cycles.hpp) |
| `prefix-sum` | build O(n); query O(1); bộ nhớ O(n). | [prefix-sum.hpp](../01_foundations/prefix-sum.hpp) |
| `prefix-xor` | build O(n), query O(1), bộ nhớ O(n). Index 0-based, đoạn nửa mở \[l,r). | [prefix-xor.hpp](../01_foundations/prefix-xor.hpp) |
| `simulation` | simulateUntil O(số bước), O(1) phụ; floydCycleInfo O(mu + lambda), O(1) phụ. | [simulation.hpp](../01_foundations/simulation.hpp) |
| `sliding-window` | O(n) — mỗi phần tử vào và ra cửa sổ đúng một lần. | [sliding-window.hpp](../01_foundations/sliding-window.hpp) |
| `submask-enumeration` | tổng trên mọi mask 0..2^n-1 là O(3^n); một mask là O(2^popcount(mask)). | [submask-enumeration.hpp](../01_foundations/submask-enumeration.hpp) |
| `sweep-line` | orderedSweepEvents O(e log e), intervalContainmentFlags O(n log n); bộ nhớ O(e) hoặc O(n), gồm cả bản sao output/thứ tự. | [sweep-line.hpp](../01_foundations/sweep-line.hpp) |
| `two-pointers` | O(n) sau khi mảng đã sort. | [two-pointers.hpp](../01_foundations/two-pointers.hpp) |

### 02 — Cấu trúc dữ liệu

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `binary-trie` | insert/erase/maxXor O(B); bộ nhớ O(số phần tử · B). B = số bit (mặc định 30, giá trị < 2^30). | [binary-trie.hpp](../02_data_structures/binary-trie.hpp) |
| `convex-hull-trick` | add O(1) khấu hao, query O(1) khấu hao (con trỏ chỉ tiến); bộ nhớ O(n). | [convex-hull-trick.hpp](../02_data_structures/convex-hull-trick.hpp) |
| `disjoint-sparse-table` | build O(n log n); query O(1); bộ nhớ O(n log n). Đoạn nửa mở \[l, r), l < r. | [disjoint-sparse-table.hpp](../02_data_structures/disjoint-sparse-table.hpp) |
| `dsu` | find/unite ~O(α(n)) khấu hao; bộ nhớ O(n). Index 0-based. | [dsu.hpp](../02_data_structures/dsu.hpp) |
| `dynamic-segment-tree` | update/query O(log N); bộ nhớ O(số update · log N). N là kích thước MIỀN, không phải số phần tử. | [dynamic-segment-tree.hpp](../02_data_structures/dynamic-segment-tree.hpp) |
| `fenwick-2d` | add/query O(log R · log C); bộ nhớ O(R·C). Ngoài 0-based; trong 1-based. | [fenwick-2d.hpp](../02_data_structures/fenwick-2d.hpp) |
| `fenwick` | mỗi update/query O(log n); bộ nhớ O(n). | [fenwick.hpp](../02_data_structures/fenwick.hpp) |
| `implicit-treap` | build O(n); reverse/sum O(log n) kỳ vọng; bộ nhớ O(n). Chỉ số 0-based, đoạn nửa mở \[l, r). | [implicit-treap.hpp](../02_data_structures/implicit-treap.hpp) |
| `interval-set` | O((k+1) log m) cho insert/erase đụng k đoạn; contains/first/last O(log m); bộ nhớ O(m). | [interval-set.hpp](../02_data_structures/interval-set.hpp) |
| `lazy-segment-tree` | build O(n); mỗi update/query O(log n); bộ nhớ O(n). Index mảng 0-based, đoạn nửa mở \[l,r). | [lazy-segment-tree.hpp](../02_data_structures/lazy-segment-tree.hpp) |
| `leftist-heap` | make/top O(1); meld/push/pop O(log n); bộ nhớ O(tổng số lần push). Root là handle int, -1 = rỗng. | [leftist-heap.hpp](../02_data_structures/leftist-heap.hpp) |
| `li-chao-tree` | addLine/query O(log C), addSegment O(log^2 C); bộ nhớ theo số node được chạm. Miền \[lo,hi\] đóng. | [li-chao-tree.hpp](../02_data_structures/li-chao-tree.hpp) |
| `median-maintenance` | add/erase O(log n), median/cost O(1); bộ nhớ O(n). | [median-maintenance.hpp](../02_data_structures/median-maintenance.hpp) |
| `ordered-set` | insert/erase/find_by_order/order_of_key O(log n); CHỈ chạy trên g++ (đuôi mở rộng GCC, không portable). | [ordered-set.hpp](../02_data_structures/ordered-set.hpp) |
| `persistent-segment-tree` | build O(n log n); truy vấn/cập nhật O(log n), copyVersion O(1); bộ nhớ O(n log n + q log n). | [persistent-segment-tree.hpp](../02_data_structures/persistent-segment-tree.hpp) |
| `rollback-dsu` | find/unite O(log n); rollback O(số union đã hoàn tác); bộ nhớ O(n). Index 0-based. | [rollback-dsu.hpp](../02_data_structures/rollback-dsu.hpp) |
| `run-intervals` | dựng O(n log n), assign/flipBit O(log n), maxRun O(1), runRange O(log n); bộ nhớ O(n). | [run-intervals.hpp](../02_data_structures/run-intervals.hpp) |
| `segment-tree` | build O(n); set/query/findFirst O(log n); bộ nhớ O(2·pow2(n)). Index 0-based, đoạn \[l, r). | [segment-tree.hpp](../02_data_structures/segment-tree.hpp) |
| `sparse-table` | build O(n log n); query O(1); bộ nhớ O(n log n). Đoạn nửa mở \[l, r), l < r. | [sparse-table.hpp](../02_data_structures/sparse-table.hpp) |
| `treap` | insert/erase/kth/countLess O(log n) kỳ vọng; bộ nhớ O(n). | [treap.hpp](../02_data_structures/treap.hpp) |
| `trie` | insert/query O(\|s\|); bộ nhớ O(tổng độ dài · ALPHA). | [trie.hpp](../02_data_structures/trie.hpp) |
| `wavelet-matrix` | build O(n log MAX); kth/countLess O(log MAX); bộ nhớ O(n log MAX). | [wavelet-matrix.hpp](../02_data_structures/wavelet-matrix.hpp) |
| `weighted-dsu` | add/diff gần O(alpha(n)) amortized; bộ nhớ O(n). Đỉnh 0-based. | [weighted-dsu.hpp](../02_data_structures/weighted-dsu.hpp) |
| `window-mode` | add/remove/mode O(log D), frequency O(log D); bộ nhớ O(D + maxFreq lịch sử). | [window-mode.hpp](../02_data_structures/window-mode.hpp) |

### 03 — Đồ thị

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `bellman-ford` | O(n * m); bộ nhớ O(n). edges = {(u, v, w)} cạnh CÓ HƯỚNG u->v; BF_INF = không tới. | [bellman-ford.hpp](../03_graphs/bellman-ford.hpp) |
| `bfs-01` | O(n + m); bộ nhớ O(n). adj\[u\] = {(v, w)} với w thuộc {0, 1}. | [bfs-01.hpp](../03_graphs/bfs-01.hpp) |
| `bfs` | O(n + m); bộ nhớ O(n). Đỉnh 0-based; adj\[u\] = danh sách đỉnh kề. | [bfs.hpp](../03_graphs/bfs.hpp) |
| `block-cut-tree` | dựng O(n + m + (n + B) log(n + B)), truy vấn O(log(n + B)); B là số block. | [block-cut-tree.hpp](../03_graphs/block-cut-tree.hpp) |
| `bridges-articulation` | O(n + m); bộ nhớ O(n + m). Đỉnh 0-based; nhận cạnh SONG SONG đúng (theo id cạnh, không theo đỉnh cha). | [bridges-articulation.hpp](../03_graphs/bridges-articulation.hpp) |
| `dfs` | O(n+m); bộ nhớ O(n+m) gồm adj và stack. Đỉnh 0-based. | [dfs.hpp](../03_graphs/dfs.hpp) |
| `dijkstra` | O((n + m) log n); bộ nhớ O(n). adj\[u\] = {(v, w)}; DIJ_INF = không tới được. | [dijkstra.hpp](../03_graphs/dijkstra.hpp) |
| `dinic` | O(V^2 * E); bộ nhớ O(V + E). Đỉnh 0-based; cap là long long; addEdge có hướng (dùng cap hai chiều nếu cần). | [dinic.hpp](../03_graphs/dinic.hpp) |
| `dominator-tree` | O((n + m) log n) với link-eval/path compression dạng đơn giản; bộ nhớ O(n + m). | [dominator-tree.hpp](../03_graphs/dominator-tree.hpp) |
| `edge-orientation` | O(n+m); bộ nhớ O(n+m). Đỉnh 0-based; hỗ trợ cạnh song song, không hỗ trợ self-loop ở bản parity. | [edge-orientation.hpp](../03_graphs/edge-orientation.hpp) |
| `eulerian-path` | O(n + m); bộ nhớ O(n + m). Đỉnh 0-based; path hợp lệ có m+1 đỉnh. | [eulerian-path.hpp](../03_graphs/eulerian-path.hpp) |
| `find-cycle` | findCycleDirected O(n + m), findCycleUndirected O(n + m) (mỗi cạnh xét 2 lần); bộ nhớ phụ O(n), danh sách kề O(n + m). | [find-cycle.hpp](../03_graphs/find-cycle.hpp) |
| `floyd-warshall` | O(n^3); bộ nhớ O(n^2). Khởi tạo d\[i\]\[j\]=FW_INF (i!=j, chưa có cạnh), d\[i\]\[i\]=0. | [floyd-warshall.hpp](../03_graphs/floyd-warshall.hpp) |
| `havel-hakimi` | O((n + m) log n), m = sum(degree)/2; bộ nhớ O(n + m). | [havel-hakimi.hpp](../03_graphs/havel-hakimi.hpp) |
| `hopcroft-karp` | O(E * sqrt(V)); bộ nhớ O(V + E). Left 0-based \[0, nl), right 0-based \[0, nr); addEdge(u, v) nối chúng. | [hopcroft-karp.hpp](../03_graphs/hopcroft-karp.hpp) |
| `hungarian` | O(n^2 * m); bộ nhớ O(n * m). Hàng/cột 0-based; ma trận n x m với n <= m; chi phí long long (cho phép âm). | [hungarian.hpp](../03_graphs/hungarian.hpp) |
| `k-shortest-paths` | O(k*m log(k*m)) thời gian, O(k*n+m) bộ nhớ trong trường hợp xấu. | [k-shortest-paths.hpp](../03_graphs/k-shortest-paths.hpp) |
| `min-cost-max-flow` | O(F * V * E) xấu nhất (F = giá trị luồng); bộ nhớ O(V + E). Đỉnh 0-based; cap, cost là long long. | [min-cost-max-flow.hpp](../03_graphs/min-cost-max-flow.hpp) |
| `mst` | O(m log m) (sắp cạnh); bộ nhớ O(n + m). Đỉnh 0-based; trọng số long long. | [mst.hpp](../03_graphs/mst.hpp) |
| `scc` | build O(n + m), condensation O(n + m log m) (khử trùng bằng set); bộ nhớ O(n + m). comp\[v\] = id thành phần, tô-pô NGƯỢC (sink id nhỏ). | [scc.hpp](../03_graphs/scc.hpp) |
| `topological-sort` | O(n + m); bộ nhớ O(n). Đỉnh 0-based; adj\[u\] = các đỉnh v với cạnh u->v. | [topological-sort.hpp](../03_graphs/topological-sort.hpp) |
| `two-sat` | O(n + biên); bộ nhớ O(n + biên). Biến 0-based; literal 2*i = (x_i đúng), 2*i+1 = (x_i sai). | [two-sat.hpp](../03_graphs/two-sat.hpp) |

### 04 — Cây

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `binary-lifting` | build O(n log K), jump/path O(log K); bộ nhớ O(n log K). Đỉnh/state 0-based. | [binary-lifting.hpp](../04_trees/binary-lifting.hpp) |
| `centroid-decomposition` | CentroidPaths O(n log²n); CentroidNearest build O(n log n), add/query O(log n); bộ nhớ O(n log n). | [centroid-decomposition.hpp](../04_trees/centroid-decomposition.hpp) |
| `euler-tour` | build O(n); mỗi truy vấn tùy cấu trúc đi kèm; bộ nhớ O(n). Đỉnh 0-based; dựng bằng DFS LẶP (không tràn stack). | [euler-tour.hpp](../04_trees/euler-tour.hpp) |
| `hld` | build O(n), update O(log n), queryPath/querySubtree O(log^2 n); bộ nhớ O(n). Đỉnh 0-based; segtree tổng lồng sẵn. | [hld.hpp](../04_trees/hld.hpp) |
| `lca-binary-lifting` | build O(n log n), lca/kth/dist O(log n); bộ nhớ O(n log n). Đỉnh 0-based; dựng bằng BFS (không tràn stack). | [lca-binary-lifting.hpp](../04_trees/lca-binary-lifting.hpp) |
| `long-path-decomposition` | O(n) thời gian và O(n) bộ nhớ; DFS lặp, an toàn cây dây n lớn. Đỉnh 0-based, cây không trọng số. | [long-path-decomposition.hpp](../04_trees/long-path-decomposition.hpp) |
| `prufer` | pruferEncode O(n) (kề CSR + BFS lấy cha + con trỏ lá, KHÔNG sort/heap), pruferDecode O(n), pruferTreeCount O(log n); bộ nhớ O(n). | [prufer.hpp](../04_trees/prufer.hpp) |
| `rerooting` | O(n) (một lượt xuống gom cây con + một lượt lên đổi gốc); bộ nhớ O(n). Đỉnh 0-based; cây không trọng số. | [rerooting.hpp](../04_trees/rerooting.hpp) |
| `small-to-large` | O(n log^2 n) (mỗi phần tử bị dời O(log n) lần × O(log n) thao tác set); bộ nhớ O(n). Đỉnh 0-based. | [small-to-large.hpp](../04_trees/small-to-large.hpp) |
| `tree-augmentation` | O(n); bộ nhớ O(n). Đỉnh 0-based. | [tree-augmentation.hpp](../04_trees/tree-augmentation.hpp) |
| `tree-diameter` | O(n); bộ nhớ O(n). Đỉnh 0-based; adj\[u\] = {(v, w)} trọng số long long >= 0; DFS LẶP (không tràn stack). | [tree-diameter.hpp](../04_trees/tree-diameter.hpp) |
| `tree-dp` | mỗi hàm O(n) — một lượt xuống (gom cây con) + một lượt lên (đổi gốc); bộ nhớ O(n). Không sort, không đệ quy. | [tree-dp.hpp](../04_trees/tree-dp.hpp) |
| `tree-isomorphism` | O(n log n) mỗi lần mã hóa (tổng chi phí sort các danh sách con); bộ nhớ O(n). | [tree-isomorphism.hpp](../04_trees/tree-isomorphism.hpp) |
| `tree-reconstruction` | O(n) kỳ vọng (kiểm tra nhãn bằng unordered_set + stack); bộ nhớ O(n). | [tree-reconstruction.hpp](../04_trees/tree-reconstruction.hpp) |

### 05 — Quy hoạch động

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `bitmask-dp` | O(2^n * n^2); bộ nhớ O(2^n * n). Đỉnh 0-based; dp\[mask\]\[v\] = trạng thái "đã thăm mask, đang ở v". | [bitmask-dp.hpp](../05_dynamic_programming/bitmask-dp.hpp) |
| `broken-profile-dp` | countTilings O(n*m*2^min(n,m)), bộ nhớ O(2^min(n,m)); countTilingsMasked cùng ĐPT (tự chuyển vị), | [broken-profile-dp.hpp](../05_dynamic_programming/broken-profile-dp.hpp) |
| `coin-change` | O(target * m); bộ nhớ O(target). coinMin trả -1 nếu không tạo được; đếm cách lấy theo MOD. | [coin-change.hpp](../05_dynamic_programming/coin-change.hpp) |
| `digit-dp` | O(số_chữ_số * trạng_thái * 10); bộ nhớ theo số trạng thái. Trạng thái: (vị trí, chữ số trước, đang-tight, đã-bắt-đầu). | [digit-dp.hpp](../05_dynamic_programming/digit-dp.hpp) |
| `edit-distance` | O(n*m); bộ nhớ O(n*m). dp\[i\]\[j\] = khoảng cách giữa a\[0..i) và b\[0..j). | [edit-distance.hpp](../05_dynamic_programming/edit-distance.hpp) |
| `grid-dp` | O(n*m); bộ nhớ O(m) cho các API bên dưới. | [grid-dp.hpp](../05_dynamic_programming/grid-dp.hpp) |
| `interval-dp` | O(n^2); bộ nhớ O(n^2). dp\[i\]\[j\] = HIỆU điểm (người đi trước - người đi sau) tối ưu trên a\[i..j\]. | [interval-dp.hpp](../05_dynamic_programming/interval-dp.hpp) |
| `knapsack` | 0/1 và bounded đều O(nW); bộ nhớ O(W). Bounded không phụ thuộc độ lớn count. | [knapsack.hpp](../05_dynamic_programming/knapsack.hpp) |
| `lcs` | O(n*m); bộ nhớ O(n*m). dp\[i\]\[j\] = LCS của a\[0..i) và b\[0..j). | [lcs.hpp](../05_dynamic_programming/lcs.hpp) |
| `linear-dp` | runner O(steps*(width + transition)), O(width); Array Description O(nm), O(m); Removing Digits O(n log10 n), O(1). | [linear-dp.hpp](../05_dynamic_programming/linear-dp.hpp) |
| `lis` | O(n log n); bộ nhớ O(n). Mảng bất kỳ; kết quả là ĐỘ DÀI (không phải chính dãy con). | [lis.hpp](../05_dynamic_programming/lis.hpp) |
| `slope-trick` | minCostNonDecreasing O(n log n) (mỗi phần tử 1 push + tối đa 1 pop/push); minCostIncreasing O(n log n) (thêm O(n) dựng a_i - i); bộ nhớ O(n). | [slope-trick.hpp](../05_dynamic_programming/slope-trick.hpp) |
| `sos-dp` | O(n * 2^n) (nhanh hơn O(3^n) duyệt trực tiếp submask); bộ nhớ O(2^n). f có kích thước ĐÚNG 2^n. | [sos-dp.hpp](../05_dynamic_programming/sos-dp.hpp) |
| `subset-sum-trace` | dựng O(n*limit/64 + limit), canMake O(1), subsetIndices O(số phần tử truy vết); bộ nhớ O(limit). | [subset-sum-trace.hpp](../05_dynamic_programming/subset-sum-trace.hpp) |
| `weighted-interval-scheduling` | O(n log n) thời gian do sort + lower_bound; O(n) bộ nhớ. Cho phép chọn rỗng với giá trị 0. | [weighted-interval-scheduling.hpp](../05_dynamic_programming/weighted-interval-scheduling.hpp) |

### 06 — Xâu

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `aho-corasick` | build O(A * số_node) (A = 26), countOccurrences O(\|t\| + số_node); bộ nhớ O(A * số_node). Bảng chữ 'a'..'z'. | [aho-corasick.hpp](../06_strings/aho-corasick.hpp) |
| `bwt` | transform O(n log n), inverse O(n + 256); bộ nhớ O(n). Thứ tự ký tự theo unsigned byte. | [bwt.hpp](../06_strings/bwt.hpp) |
| `distinct-subsequences` | O(n + 256), bộ nhớ O(256). Hỗ trợ mọi byte trong std::string. | [distinct-subsequences.hpp](../06_strings/distinct-subsequences.hpp) |
| `eertree` | O(n * A) (A = 26); bộ nhớ O(n * A). Node 0 = gốc ảo (len -1), node 1 = gốc rỗng (len 0). | [eertree.hpp](../06_strings/eertree.hpp) |
| `kmp-automaton` | kmpAutomaton O(\|p\| * \|alphabet\|) thời gian và bộ nhớ; countStringsAvoiding O(n * \|p\| * (d + 1)) thời gian, O(\|p\| * (d + 1)) bộ nhớ, với d = số ký tự PHÂN BIỆT tr… | [kmp-automaton.hpp](../06_strings/kmp-automaton.hpp) |
| `kmp` | prefixFunction O(n), kmpSearch O(\|t\| + \|p\|); bộ nhớ O(n). Biên dài nhất = pi.back(); chu kỳ nhỏ nhất = n - pi.back(). | [kmp.hpp](../06_strings/kmp.hpp) |
| `manacher` | O(n) dựng; longest/count O(n); bộ nhớ O(n). d1\[i\] = bán kính palindrome LẺ tâm i; d2\[i\] = palindrome CHẴN tâm (i-1,i). | [manacher.hpp](../06_strings/manacher.hpp) |
| `min-rotation` | O(n); bộ nhớ O(n) (dùng s + s). Trả về CHỈ SỐ bắt đầu; xâu quay nhỏ nhất = s\[i..\] + s\[..i). | [min-rotation.hpp](../06_strings/min-rotation.hpp) |
| `string-hashing` | dựng O(n), get O(1); bộ nhớ O(n). Hai đoạn bằng nhau <=> get() bằng nhau (xác suất va chạm cực nhỏ). | [string-hashing.hpp](../06_strings/string-hashing.hpp) |
| `suffix-array` | suffixArray O(n log^2 n), lcpArray O(n); bộ nhớ O(n). sa\[k\] = vị trí bắt đầu hậu tố hạng k (0-based). | [suffix-array.hpp](../06_strings/suffix-array.hpp) |
| `suffix-automaton` | dựng O(n * A), A=26; propagate/DP O(n * A); mỗi pattern O(\|p\|), truy vết O(\|đáp án\| * A); bộ nhớ O(n * A). | [suffix-automaton.hpp](../06_strings/suffix-automaton.hpp) |
| `z-function` | zFunction O(n), zSearch O(\|p\| + \|t\|); bộ nhớ O(n). Quy ước z\[0\] = n (toàn bộ xâu). | [z-function.hpp](../06_strings/z-function.hpp) |

### 07 — Toán

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `burnside` | countNecklaces/countBracelets O(sqrt(n) + d(n)·log n) (phân tích n + duyệt ước + lũy thừa nhanh); bộ nhớ O(log n). | [burnside.hpp](../07_mathematics/burnside.hpp) |
| `combinatorics` | dựng O(n), C/P O(1); bộ nhớ O(n). mod phải NGUYÊN TỐ và > n. Nhân dùng __int128 nên mod tới ~1e18 an toàn. | [combinatorics.hpp](../07_mathematics/combinatorics.hpp) |
| `divisor-blocks` | mọi hàm O(sqrt n) (khoảng 2*sqrt(n) khối); bộ nhớ O(1), riêng floorBlocks O(sqrt n) để chứa kết quả. | [divisor-blocks.hpp](../07_mathematics/divisor-blocks.hpp) |
| `divisor-sieve` | dựng và exactGcdSubsetCounts O(M log M), largest O(M); bộ nhớ O(M+n), M=max(max_value, max(values)). | [divisor-sieve.hpp](../07_mathematics/divisor-sieve.hpp) |
| `eulerian-numbers` | eulerianNumberRow O(n^2) thời gian, O(n) bộ nhớ; eulerianNumberExplicitPrimeMod O((k+1) log n) thời gian, O(k) bộ nhớ. | [eulerian-numbers.hpp](../07_mathematics/eulerian-numbers.hpp) |
| `extended-euclid` | extgcd O(log min(a, b)); crt O(log); bộ nhớ O(1). invMod cần gcd(a, m) = 1. | [extended-euclid.hpp](../07_mathematics/extended-euclid.hpp) |
| `factorization` | millerRabin O(log^2 n); factorize ~O(n^{1/4}) mỗi thừa số; bộ nhớ O(số thừa số). Nhân dùng UNSIGNED __int128. | [factorization.hpp](../07_mathematics/factorization.hpp) |
| `gaussian-elimination` | O(n·m·min(n,m)); bộ nhớ O(n·m). p PHẢI nguyên tố (dùng nghịch đảo Fermat). Trả trạng thái + một nghiệm. | [gaussian-elimination.hpp](../07_mathematics/gaussian-elimination.hpp) |
| `inclusion-exclusion` | countDivisibleByAny O(2^k * k) (k <= ~20, tính cả gộp lcm); derangements O(n); bộ nhớ O(1). | [inclusion-exclusion.hpp](../07_mathematics/inclusion-exclusion.hpp) |
| `integer-math` | sqrt O(log 2^64), factorial exponent/digit query O(log n), lcm O(log min); bộ nhớ O(1). | [integer-math.hpp](../07_mathematics/integer-math.hpp) |
| `josephus` | josephusOrder O(n log n) (Fenwick + tìm phần tử còn sống thứ i); | [josephus.hpp](../07_mathematics/josephus.hpp) |
| `kth-permutation` | kthPermutation O(n^2) (mỗi bước xóa 1 phần tử khỏi danh sách còn trống); permutationRank O(n^2); | [kth-permutation.hpp](../07_mathematics/kth-permutation.hpp) |
| `labeled-combinatorics` | functional graph O(n²)/O(n); tournament O(n³)/O(n²), phù hợp n<=500. | [labeled-combinatorics.hpp](../07_mathematics/labeled-combinatorics.hpp) |
| `lucas` | dựng bảng giai thừa mod p O(p) (nhớ lại, chỉ dựng khi p đổi); mỗi truy vấn O(log_p n); binomOddParity O(1); bộ nhớ O(p). | [lucas.hpp](../07_mathematics/lucas.hpp) |
| `matrix-exponentiation` | nhân O(n^3), pow O(n^3 log e); bộ nhớ O(n^2). Ma trận vuông n x n, phần tử theo mod (long long + __int128). | [matrix-exponentiation.hpp](../07_mathematics/matrix-exponentiation.hpp) |
| `mobius` | dựng O(n); bộ nhớ O(n). mu\[1\]=phi\[1\]=1; mu\[n\] thuộc {-1,0,1}; phi\[n\] = số số trong \[1,n\] nguyên tố cùng nhau với n. | [mobius.hpp](../07_mathematics/mobius.hpp) |
| `modular-arithmetic` | O(log e); bộ nhớ O(1). Nhân dùng __int128 nên an toàn với mod tới ~1e18. | [modular-arithmetic.hpp](../07_mathematics/modular-arithmetic.hpp) |
| `ntt` | mỗi phép tích chập O((n+m) log(n+m)); bộ nhớ O(n+m). | [ntt.hpp](../07_mathematics/ntt.hpp) |
| `probability-expectation` | dice O(dice^2*sides^2); E\[max\] O(max); inversion O(n^2); walkers O(R*steps*E); bộ nhớ tùy hàm. | [probability-expectation.hpp](../07_mathematics/probability-expectation.hpp) |
| `semiring-matrix` | nhân O(r*k*c), pow O(n^3 log e), vector*pow O(n^3 log e + n^2 log e); bộ nhớ O(n^2). | [semiring-matrix.hpp](../07_mathematics/semiring-matrix.hpp) |
| `sieve` | dựng O(n); factorize O(log x); bộ nhớ O(n). spf\[x\] = ước nguyên tố nhỏ nhất của x (x >= 2). | [sieve.hpp](../07_mathematics/sieve.hpp) |
| `sprague-grundy` | nimXor O(số đống); grundySubtraction O(n · \|moves\|); mex O(\|s\|). Người đi trước THẮNG <=> XOR Grundy != 0. | [sprague-grundy.hpp](../07_mathematics/sprague-grundy.hpp) |
| `walsh-hadamard` | walshHadamard O(n log n); xorConvolution O(n log n) (gồm cả pad lên luỹ thừa 2); bộ nhớ O(n). | [walsh-hadamard.hpp](../07_mathematics/walsh-hadamard.hpp) |
| `xor-basis` | insert / canForm O(B), maxXor O(B), kthXor O(B) với B = 64 bit (mỗi thao tác chỉ quét vài lượt qua 64 bit); | [xor-basis.hpp](../07_mathematics/xor-basis.hpp) |

### 08 — Hình học

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `closest-pair` | O(n log n); bộ nhớ O(n). Trả BÌNH PHƯƠNG khoảng cách nhỏ nhất; lấy sqrt nếu cần khoảng cách thực. | [closest-pair.hpp](../08_geometry/closest-pair.hpp) |
| `convex-hull` | O(n log n); bộ nhớ O(n). Trả các đỉnh bao theo NGƯỢC CHIỀU KIM ĐỒNG HỒ, KHÔNG gồm điểm thẳng hàng trên cạnh. | [convex-hull.hpp](../08_geometry/convex-hull.hpp) |
| `geometry-basics` | mọi thao tác O(1); bộ nhớ O(1). Tọa độ NGUYÊN, \|x\|,\|y\| tới ~1e9 để cross không tràn long long (~9.2e18). | [geometry-basics.hpp](../08_geometry/geometry-basics.hpp) |
| `line-intersection` | O(1); bộ nhớ O(1). Định thức dùng long long (chính xác), tọa độ giao là double (có sai số nhỏ). | [line-intersection.hpp](../08_geometry/line-intersection.hpp) |
| `manhattan-distance` | online add/diameter O(1), bộ nhớ O(1); pair sum O(n log n), bộ nhớ O(n). | [manhattan-distance.hpp](../08_geometry/manhattan-distance.hpp) |
| `point-in-polygon` | O(n) mỗi truy vấn; bộ nhớ O(1). Dùng số học NGUYÊN (không sai số); đa giác đơn, đỉnh theo thứ tự bất kỳ. | [point-in-polygon.hpp](../08_geometry/point-in-polygon.hpp) |
| `polygon-area` | mọi hàm O(n); bộ nhớ O(1). Trả 2×diện tích (long long, chính xác); \|A\| = area, dấu > 0 nếu đỉnh theo CCW. | [polygon-area.hpp](../08_geometry/polygon-area.hpp) |
| `rotating-calipers` | O(n log n) (sắp xếp dựng bao); bộ nhớ O(n). Trả BÌNH PHƯƠNG khoảng cách lớn nhất (long long, chính xác). | [rotating-calipers.hpp](../08_geometry/rotating-calipers.hpp) |
| `segment-intersection` | O(1); bộ nhớ O(1). Số học NGUYÊN (không sai số) qua hướng quay orient; xử lý cả trường hợp thẳng hàng chồng lấn. | [segment-intersection.hpp](../08_geometry/segment-intersection.hpp) |

### 09 — Thuật toán offline

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `cdq-divide-conquer` | O(n log^2 n); bộ nhớ O(n). Chiều thứ nhất = THỨ TỰ trong mảng (thời gian), hai chiều còn lại là x và y. | [cdq-divide-conquer.hpp](../09_offline_algorithms/cdq-divide-conquer.hpp) |
| `mo-algorithm` | O((n + q) sqrt(n) + n log n + q log q) (gồm cả nén giá trị và sắp truy vấn); bộ nhớ O(n + q). | [mo-algorithm.hpp](../09_offline_algorithms/mo-algorithm.hpp) |
| `offline-dynamic-connectivity` | O((E log T + Q) * log n) qua cây phân đoạn theo thời gian + DSU có rollback; bộ nhớ O(E log T + n). | [offline-dynamic-connectivity.hpp](../09_offline_algorithms/offline-dynamic-connectivity.hpp) |
| `offline-lca` | O((n + q) α(n)); bộ nhớ O(n + q). Đỉnh 0-based; DFS LẶP (không tràn stack); cây phải LIÊN THÔNG. | [offline-lca.hpp](../09_offline_algorithms/offline-lca.hpp) |
| `parallel-binary-search` | O((n + m + q) log m · α(n)); bộ nhớ O(n + m + q). Cạnh thứ d (1-based) xây vào NGÀY d; DSU dựng lại (O(n)) mỗi vòng. | [parallel-binary-search.hpp](../09_offline_algorithms/parallel-binary-search.hpp) |
| `sqrt-decomposition` | dựng O(n), mỗi thao tác O(sqrt(n)); bộ nhớ O(n). Khối lưu tổng + lazy cộng; chỉ số 0-based, đoạn \[l, r\] ĐÓNG. | [sqrt-decomposition.hpp](../09_offline_algorithms/sqrt-decomposition.hpp) |

### 10 — Kỹ thuật nâng cao

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `bitset-reachability` | dựng O((n + m)·n/64); reachable O(1); count O(n/64); bộ nhớ n·ceil(n/64)·8 byte ≈ n^2/8 (n=5e4 -> ~312 MB!). | [bitset-reachability.hpp](../10_advanced_techniques/bitset-reachability.hpp) |
| `constructive` | các hàm O(n); bộ nhớ O(n). | [constructive.hpp](../10_advanced_techniques/constructive.hpp) |
| `divide-conquer-dp` | O(k·n log n) lần gọi cost; bộ nhớ O(n). cost(l, r) là chi phí đoạn NỬA MỞ \[l, r) (phần tử l..r-1). | [divide-conquer-dp.hpp](../10_advanced_techniques/divide-conquer-dp.hpp) |
| `functional-graph` | dựng O(n log K); kth/dist O(log K); stepsUntilRepeat O(1); bộ nhớ O(n log K) với K = maxK (mặc định 1e9). | [functional-graph.hpp](../10_advanced_techniques/functional-graph.hpp) |
| `interactive-search` | mỗi performBudgetedInteractiveQuery ngoài callback là O(1); tạo/giải B truy vấn bit là O(items*B), B=floor(log2(maxLabel))+1. | [interactive-search.hpp](../10_advanced_techniques/interactive-search.hpp) |
| `k-best-combinations` | O(n log n + r log r), bộ nhớ O(n+r), r=min(k,C(n,m)). | [k-best-combinations.hpp](../10_advanced_techniques/k-best-combinations.hpp) |
| `k-best-enumeration` | O(n log n + k log k); bộ nhớ O(n+k). | [k-best-enumeration.hpp](../10_advanced_techniques/k-best-enumeration.hpp) |
| `knuth-optimization` | O(n^2) nhờ opt\[i\]\[j-1\] <= opt\[i\]\[j\] <= opt\[i+1\]\[j\]; bộ nhớ O(n^2). cost(i, j) là chi phí đoạn NỬA MỞ \[i, j). | [knuth-optimization.hpp](../10_advanced_techniques/knuth-optimization.hpp) |
| `kruskal-reconstruction-tree` | dựng O(m log m + n log n); bottleneck O(log n); bộ nhớ O(n log n). Lá 0..n-1 là đỉnh gốc, node >= n là node hợp. | [kruskal-reconstruction-tree.hpp](../10_advanced_techniques/kruskal-reconstruction-tree.hpp) |
| `meet-in-the-middle` | O(2^(n/2) * (n + log)); bộ nhớ O(2^(n/2)). Nửa trái enum, nửa phải sort + tra cứu nhị phân. | [meet-in-the-middle.hpp](../10_advanced_techniques/meet-in-the-middle.hpp) |
| `multiset-reconstruction` | O(n^3 log n) theo cận thô, gồm sort; bộ nhớ O(n^2). | [multiset-reconstruction.hpp](../10_advanced_techniques/multiset-reconstruction.hpp) |
| `ternary-search` | O(log(hi-lo)) lần gọi f; bộ nhớ O(1). convexMin chặt nhị phân trên f(m) <= f(m+1) nên CHỊU ĐƯỢC đoạn bằng nhau. | [ternary-search.hpp](../10_advanced_techniques/ternary-search.hpp) |

### 11 — Greedy (tham lam trên đoạn / lịch biểu)

| Kỹ thuật | Độ phức tạp & bộ nhớ | File |
|---|---|---|
| `greedy-array` | O(n) trừ smallestMissingSubsetSum O(n log n) do sort; bộ nhớ O(n) khi phải sort bản sao. | [greedy-array.hpp](../11_greedy/greedy-array.hpp) |
| `heap-scheduling` | maxMoviesKPeople O(n log n) (sort n log n + n lần upper_bound/erase/insert trên multiset min(k,n) phần tử); maxTasksProfit O(n log n) (chỉ sort); bộ nhớ O(n). | [heap-scheduling.hpp](../11_greedy/heap-scheduling.hpp) |
| `heap-selection` | top-k O(n*(1+log(min(n,k)+1))), O(n+min(n,k)); two-role O(n log n), O(n); bubble O(n*(1+log(min(n,passes+1)))), O(n). | [heap-selection.hpp](../11_greedy/heap-selection.hpp) |
| `interval-scheduling` | maxNonOverlappingPick/maxNonOverlapping O(n log n) (sort theo r); minRooms O(n log n) (sort 2n sự kiện); minRoomsAssign O(n log n) (sort theo l + heap); bộ nhớ O(n). | [interval-scheduling.hpp](../11_greedy/interval-scheduling.hpp) |
| `optimal-merge` | O(n log n); bộ nhớ O(n). | [optimal-merge.hpp](../11_greedy/optimal-merge.hpp) |
| `path-matching` | O((m+k) log m) cho m cạnh và các k=0..maxPairs; bộ nhớ O(m). | [path-matching.hpp](../11_greedy/path-matching.hpp) |

<!-- GEN:END -->

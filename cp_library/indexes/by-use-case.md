# Tra theo dấu hiệu đề bài

Dùng khi **chưa nhớ tên thuật toán** nhưng đã đọc ra thao tác/cấu trúc của đề. Cách nhanh nhất:
quét bảng "triệu chứng" dưới đây → mở đúng `.hpp` → đọc 5 dòng header (mục đích · khi dùng · ĐPT ·
dùng · bẫy) → dán vào lời giải.

Xem thêm: [decision-trees.md](decision-trees.md) (chọn theo nhánh) ·
[by-complexity.md](by-complexity.md) (n cho phép làm gì) · [pitfalls.md](pitfalls.md) (bẫy hay chết).

## Triệu chứng đề bài → snippet

| Đề nói gì | Snippet |
|---|---|
| "tổng đoạn", mảng KHÔNG đổi | `prefix-sum` |
| "cộng v vào mọi phần tử trong đoạn", chỉ đọc kết quả SAU CÙNG | `difference-array` |
| cập nhật MỘT phần tử + hỏi tổng đoạn | `fenwick` |
| cập nhật cả ĐOẠN + hỏi đoạn | `lazy-segment-tree`, `sqrt-decomposition` |
| gộp đoạn theo monoid (max/min/gcd) + cập nhật điểm | `segment-tree` |
| min/max/gcd đoạn, mảng TĨNH, hỏi rất nhiều | `sparse-table`, `disjoint-sparse-table` |
| min/max của MỌI cửa sổ độ dài k | `monotonic-deque` |
| "phần tử lớn/nhỏ hơn gần nhất bên trái/phải", histogram | `monotonic-stack` |
| đếm/tối ưu đoạn con thỏa điều kiện đơn điệu theo độ dài | `sliding-window`, `two-pointers` |
| "giá trị nhỏ nhất vẫn thỏa / lớn nhất vẫn được" | `binary-search` |
| giá trị tới 1e9 nhưng chỉ cần thứ hạng | `coordinate-compression` |
| "phần tử nhỏ thứ k trong đoạn" (mảng tĩnh) | `persistent-segment-tree`, `wavelet-matrix` |
| "phần tử nhỏ thứ k" trên tập ĐỘNG (thêm/xóa) | `ordered-set`, `treap` |
| chỉ số/tọa độ tới 1e9, không muốn nén | `dynamic-segment-tree` |
| XOR lớn nhất với một số | `binary-trie` |
| đếm từ / đếm tiền tố | `trie` |
| gộp nhóm, "cùng nhóm chưa?" (chỉ thêm cạnh) | `dsu` |
| như trên nhưng cần HOÀN TÁC | `rollback-dsu` |
| tối ưu DP dạng `min(a·x + b)` | `convex-hull-trick`, `li-chao-tree` |
| lưới 2D: cập nhật điểm + tổng hình chữ nhật | `fenwick-2d` |
| đảo đoạn / chèn-xóa giữa dãy nhiều lần | `implicit-treap` |
| đường đi ngắn nhất, các cạnh dài như nhau | `bfs` |
| đường đi ngắn nhất, trọng số ≥ 0 | `dijkstra` |
| cạnh chỉ nặng 0 hoặc 1 | `bfs-01` |
| có cạnh ÂM / cần phát hiện chu trình âm | `bellman-ford` |
| mọi cặp khoảng cách, n ≤ ~500 | `floyd-warshall` |
| thứ tự phụ thuộc, "đồ thị có chu trình không" | `topological-sort` |
| nén chu trình có hướng thành DAG | `scc` |
| ràng buộc "chọn 1 trong 2 / nếu... thì..." | `two-sat` |
| cạnh/đỉnh mà bỏ đi làm mất liên thông | `bridges-articulation` |
| nối mọi đỉnh với tổng trọng số nhỏ nhất | `mst` |
| "cạnh lớn nhất nhỏ nhất trên đường" / ngày sớm nhất liên thông | `kruskal-reconstruction-tree`, `parallel-binary-search` |
| đi qua MỌI cạnh đúng một lần | `eulerian-path` |
| "tối đa bao nhiêu đường không chung cạnh", lát cắt nhỏ nhất | `dinic` |
| luồng kèm chi phí mỗi đơn vị | `min-cost-max-flow` |
| ghép cặp hai phía nhiều nhất | `hopcroft-karp` |
| gán n việc cho n người, tổng chi phí nhỏ nhất | `hungarian` |
| mỗi đỉnh có ĐÚNG một cạnh ra ("teleporter") | `functional-graph` |
| "từ u tới được bao nhiêu đỉnh" trên DAG | `bitset-reachability` |
| tổ tiên chung / khoảng cách trên cây, nhiều truy vấn | `lca-binary-lifting`, `offline-lca` |
| truy vấn CÂY CON (tổng/đếm) | `euler-tour` |
| truy vấn + cập nhật trên ĐƯỜNG ĐI của cây | `hld` |
| đường đi dài nhất trên cây | `tree-diameter` |
| tính đáp án cho MỌI đỉnh làm gốc | `rerooting` |
| đếm cặp đỉnh theo khoảng cách trên cây | `centroid-decomposition` |
| đếm giá trị phân biệt trong mọi cây con | `small-to-large` |
| ghép cặp cực đại trên CÂY / khoảng cách từ mọi đỉnh | `tree-dp`, `rerooting` |
| cây có nhãn ↔ dãy số, đếm số cây | `prufer` |
| dãy con tăng dài nhất | `lis` |
| chọn tập con dưới sức chứa / "tổng nào tạo được" | `knapsack` |
| đổi tiền: ít xu nhất / đếm số cách | `coin-change` |
| biến xâu A thành xâu B với ít phép nhất | `edit-distance` |
| dãy con chung dài nhất | `lcs` |
| đếm số trong [a, b] theo ràng buộc CHỮ SỐ | `digit-dp` |
| n ≤ 20 và phải duyệt theo tập con (TSP, phủ) | `bitmask-dp` |
| tổng trên mọi tập con / tập cha của mask | `sos-dp`, `submask-enumeration` |
| ít phép ±1 nhất để dãy KHÔNG GIẢM / tăng | `slope-trick` |
| lấp bảng bằng domino / DP mặt nạ theo cột | `broken-profile-dp` |
| game lấy hai đầu / gộp đoạn tối ưu | `interval-dp`, `knuth-optimization` |
| chia mảng thành ĐÚNG k đoạn, chi phí đoạn "đẹp" | `divide-conquer-dp` |
| n ≤ 40, đếm tập con theo tổng | `meet-in-the-middle` |
| cực tiểu của hàm "giảm rồi tăng" | `ternary-search` |
| tìm mẫu trong văn bản / biên / chu kỳ | `kmp`, `z-function` |
| so hai đoạn con có bằng nhau (rất nhiều lần) | `string-hashing` |
| palindrome dài nhất / đếm palindrome | `manacher`, `eertree` |
| đếm xâu con phân biệt / xâu con lặp dài nhất | `suffix-array`, `suffix-automaton` |
| khớp NHIỀU mẫu cùng lúc | `aho-corasick` |
| ĐẾM XÂU độ dài n chứa / không chứa một mẫu | `kmp-automaton` |
| phép quay nhỏ nhất của xâu vòng | `min-rotation` |
| a^e mod m, nghịch đảo modulo | `modular-arithmetic`, `extended-euclid` |
| C(n, r) mod nguyên tố, nhiều truy vấn | `combinatorics` |
| nguyên tố / phân tích thừa số tới ~1e6 | `sieve` |
| kiểm nguyên tố / phân tích n tới ~1e18 | `factorization` |
| hồi quy tuyến tính với k rất lớn, đếm đường độ dài k | `matrix-exponentiation` |
| giải hệ phương trình tuyến tính theo mod | `gaussian-elimination` |
| đếm cặp nguyên tố cùng nhau, nghịch đảo Mobius | `mobius` |
| nhân đa thức / tích chập lớn | `ntt` |
| trò chơi hai người lấy quân, ai thắng | `sprague-grundy` |
| XOR lớn nhất / XOR thứ k / đếm XOR phân biệt của tập con | `xor-basis` |
| đếm cặp–bộ theo XOR, tích chập XOR | `walsh-hadamard` |
| tổng Σ f(i)·⌊n/i⌋, tổng số ước của 1..n với n tới 1e12 | `divisor-blocks` |
| C(n, k) mod p NHỎ với n tới 1e18, C(n,k) chẵn hay lẻ | `lucas` |
| "chia hết bởi ít nhất một trong k số", hoán vị không điểm cố định | `inclusion-exclusion` |
| đếm vòng cổ / tô màu vòng mà quay được là giống nhau | `burnside` |
| vòng tròn, đếm k rồi loại: ai sống sót / thứ tự bị loại | `josephus` |
| hoán vị thứ k theo từ điển, hoặc hoán vị này đứng thứ mấy | `kth-permutation` |
| đoạn con liền kề có tổng lớn nhất | `max-subarray` |
| ba điểm rẽ trái/phải, điểm có nằm trên đoạn | `geometry-basics` |
| bao lồi của tập điểm | `convex-hull` |
| diện tích đa giác / đếm điểm nguyên trong đa giác | `polygon-area` |
| điểm nằm trong đa giác? | `point-in-polygon` |
| hai đoạn thẳng có cắt nhau? | `segment-intersection` |
| giao điểm hai đường thẳng | `line-intersection` |
| cặp điểm gần nhất / xa nhất | `closest-pair`, `rotating-calipers` |
| nhiều truy vấn đoạn, thêm/bớt một phần tử là rẻ | `mo-algorithm` |
| cạnh XUẤT HIỆN rồi BIẾN MẤT theo thời gian | `offline-dynamic-connectivity` |
| đếm bộ ba (thời gian, x, y) đơn điệu | `cdq-divide-conquer` |
| "có chu trình không", và IN RA chu trình đó | `find-cycle` |
| chọn tối đa hoạt động không trùng giờ | `interval-scheduling` |
| ít phòng/máy nhất để chứa mọi đoạn | `interval-scheduling` |
| k người/máy làm được tối đa bao nhiêu việc | `heap-scheduling` |
| xếp thứ tự việc để tổng thời gian chờ nhỏ nhất | `heap-scheduling` |
| sinh dãy mã Gray / hai số liền kề khác 1 bit | `gray-code` |

## Bảng "Khi dùng" đầy đủ theo nhóm

<!-- GEN:BEGIN (tự sinh bởi tools/gen_docs.py — đừng sửa tay) -->

### 01 — Nền tảng (mảng, đoạn, hai con trỏ, bitmask)

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `backtracking` | n nhỏ; cần liệt kê/đếm cấu hình thỏa ràng buộc, Hamilton/queens/grid path với undo state. | [backtracking.hpp](../01_foundations/backtracking.hpp) |
| `binary-search` | tìm vị trí chèn/rank trong sorted array; hoặc pred dạng F..F,T..T (hay T..T,F..F) trên \[lo,hi\]. | [binary-search.hpp](../01_foundations/binary-search.hpp) |
| `bit-operations` | trạng thái vừa trong 64 bit; khoảng cách Hamming; tổng số bit 1 trên \[0,n\]; công thức theo bit/XOR. | [bit-operations.hpp](../01_foundations/bit-operations.hpp) |
| `comparison-sort` | chỉ biết thứ tự qua callback/query; cần giữ thứ tự tương đối của phần tử tương đương; cần chứng minh không vượt query budget. | [comparison-sort.hpp](../01_foundations/comparison-sort.hpp) |
| `coordinate-compression` | chỉ cần thứ tự tương đối để làm chỉ số cho Fenwick/segment tree... | [coordinate-compression.hpp](../01_foundations/coordinate-compression.hpp) |
| `difference-array` | nhiều lệnh "+v trên \[l, r)", chỉ cần đọc mảng sau khi xong. | [difference-array.hpp](../01_foundations/difference-array.hpp) |
| `dynamic-bitset` | subset-sum bằng dp \|= dp<<x khi tổng lớn; giao hai hàng bit; đánh dấu tập tới vài triệu bit. | [dynamic-bitset.hpp](../01_foundations/dynamic-bitset.hpp) |
| `frequency-counting` | đếm phần tử phân biệt/số lần xuất hiện; xử lý alphabet; gom đoạn bằng nhau; tìm mex. | [frequency-counting.hpp](../01_foundations/frequency-counting.hpp) |
| `gray-code` | liệt kê mọi xâu nhị phân độ dài n sao cho hai xâu liên tiếp khác 1 bit; đánh số/giải mã vòng Gray. | [gray-code.hpp](../01_foundations/gray-code.hpp) |
| `max-subarray` | "đoạn con liên tiếp có tổng lớn nhất", lãi/lỗ tối đa của một khoảng ngày liên tục. | [max-subarray.hpp](../01_foundations/max-subarray.hpp) |
| `monotonic-deque` | sliding window minimum/maximum (phần tử hết hạn ở đầu cửa sổ). | [monotonic-deque.hpp](../01_foundations/monotonic-deque.hpp) |
| `monotonic-stack` | largest rectangle in histogram, đếm đóng góp theo phần tử chặn đầu tiên. | [monotonic-stack.hpp](../01_foundations/monotonic-stack.hpp) |
| `permutation-code` | khôi phục hoán vị từ số nghịch thế tăng thêm theo từng vị trí; xếp hạng/giải mã hoán vị; chọn phần tử còn trống theo thứ tự. | [permutation-code.hpp](../01_foundations/permutation-code.hpp) |
| `permutation-cycles` | lặp hoán vị, số vòng trở về identity, xử lý từng cycle, đổi giữa permutation và inverse. | [permutation-cycles.hpp](../01_foundations/permutation-cycles.hpp) |
| `prefix-sum` | nhiều truy vấn tổng đoạn \[l, r), mảng không đổi. | [prefix-sum.hpp](../01_foundations/prefix-sum.hpp) |
| `prefix-xor` | nhiều truy vấn XOR trên đoạn, mảng không cập nhật; phép XOR có nghịch đảo là chính nó. | [prefix-xor.hpp](../01_foundations/prefix-xor.hpp) |
| `simulation` | đề mô tả trực tiếp trạng thái kế tiếp; cần mô phỏng an toàn hoặc tìm preperiod/period của dãy trạng thái. | [simulation.hpp](../01_foundations/simulation.hpp) |
| `sliding-window` | tính hợp lệ ĐƠN ĐIỆU theo độ dài (thêm phần tử làm xấu đi, bớt làm tốt lên). | [sliding-window.hpp](../01_foundations/sliding-window.hpp) |
| `submask-enumeration` | DP trên tập con, chia mask thành cặp bù (s, mask ^ s). | [submask-enumeration.hpp](../01_foundations/submask-enumeration.hpp) |
| `sweep-line` | biến đối tượng hình học/thời gian thành OPEN/QUERY/CLOSE; cần quy định rõ thứ tự sự kiện cùng tọa độ; nested ranges. | [sweep-line.hpp](../01_foundations/sweep-line.hpp) |
| `two-pointers` | tìm cặp theo tổng, đếm cặp thỏa điều kiện đơn điệu theo tổng. | [two-pointers.hpp](../01_foundations/two-pointers.hpp) |

### 02 — Cấu trúc dữ liệu

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `binary-trie` | tìm max (x XOR y) với y trong tập; duy trì tập số nguyên không âm theo bit. | [binary-trie.hpp](../02_data_structures/binary-trie.hpp) |
| `convex-hull-trick` | DP dp\[i\]=min_j(a_j*x_i + b_j) với a_j giảm dần theo j, x_i không giảm. Nhẹ hơn Li Chao. | [convex-hull-trick.hpp](../02_data_structures/convex-hull-trick.hpp) |
| `disjoint-sparse-table` | range gcd / product-mod / min-count... trên mảng tĩnh, cần query O(1) cho op không nghịch đảo. | [disjoint-sparse-table.hpp](../02_data_structures/disjoint-sparse-table.hpp) |
| `dsu` | connectivity chỉ THÊM cạnh, Kruskal MST, gộp nhóm offline, kiểm tra đồ thị hai phía động. | [dsu.hpp](../02_data_structures/dsu.hpp) |
| `dynamic-segment-tree` | chỉ số/tọa độ tới ~1e9 không muốn nén; point update + range sum online. | [dynamic-segment-tree.hpp](../02_data_structures/dynamic-segment-tree.hpp) |
| `fenwick-2d` | lưới kích thước cố định (R·C vừa phải), update ô + tổng vùng con. | [fenwick-2d.hpp](../02_data_structures/fenwick-2d.hpp) |
| `fenwick` | nhiều update điểm xen truy vấn prefix/đoạn; PrefixMax dùng khi giá trị tại điểm chỉ tăng bằng chmax. | [fenwick.hpp](../02_data_structures/fenwick.hpp) |
| `implicit-treap` | cần đảo ngược đoạn \[l, r) nhiều lần + hỏi tổng đoạn; hoặc chèn/xóa ở giữa dãy. | [implicit-treap.hpp](../02_data_structures/implicit-treap.hpp) |
| `interval-set` | các đoạn đã thăm trên mỗi hàng/cột, lịch tài nguyên, hợp/xóa range động bằng ordered map. | [interval-set.hpp](../02_data_structures/interval-set.hpp) |
| `lazy-segment-tree` | CSES 1735 (add/assign/sum), 2425 (add/min/max), 1741 (độ dài đang được phủ khi sweep). | [lazy-segment-tree.hpp](../02_data_structures/lazy-segment-tree.hpp) |
| `leftist-heap` | DSU cần gộp heap của component; divide-and-conquer/graph cần meld nhanh hơn std::priority_queue. | [leftist-heap.hpp](../02_data_structures/leftist-heap.hpp) |
| `li-chao-tree` | tối ưu DP với slope/query bất kỳ; addSegment dùng khi mỗi đường chỉ hợp lệ trên một khoảng x. | [li-chao-tree.hpp](../02_data_structures/li-chao-tree.hpp) |
| `median-maintenance` | median/cost của cửa sổ trượt hoặc multiset động; cần xử lý giá trị trùng. | [median-maintenance.hpp](../02_data_structures/median-maintenance.hpp) |
| `ordered-set` | cần "phần tử nhỏ thứ k" và "số phần tử nhỏ hơn x" trên tập ĐỘNG; ngắn và nhanh hơn tự viết treap. | [ordered-set.hpp](../02_data_structures/ordered-set.hpp) |
| `persistent-segment-tree` | cần range order statistic/value-sum trên mảng tĩnh hoặc point-assign/range-sum theo version. | [persistent-segment-tree.hpp](../02_data_structures/persistent-segment-tree.hpp) |
| `rollback-dsu` | offline dynamic connectivity (segment tree theo thời gian), quay lui trạng thái. | [rollback-dsu.hpp](../02_data_structures/rollback-dsu.hpp) |
| `run-intervals` | chuỗi động với point update; cần độ dài run lớn nhất hoặc run chứa một vị trí. | [run-intervals.hpp](../02_data_structures/run-intervals.hpp) |
| `segment-tree` | update điểm + query sum/min/max/gcd/Node; hoặc tìm trái nhất nơi max/sum/count đạt ngưỡng. | [segment-tree.hpp](../02_data_structures/segment-tree.hpp) |
| `sparse-table` | nhiều truy vấn min/max/gcd đoạn, mảng không đổi. | [sparse-table.hpp](../02_data_structures/sparse-table.hpp) |
| `treap` | chèn/xóa + hỏi phần tử nhỏ thứ k / đếm số phần tử nhỏ hơn x, khi PBDS không tiện. | [treap.hpp](../02_data_structures/treap.hpp) |
| `trie` | đếm từ có tiền tố cho trước, kiểm tra tồn tại, word-break DP. | [trie.hpp](../02_data_structures/trie.hpp) |
| `wavelet-matrix` | nhiều truy vấn k-th / rank trên đoạn của mảng không đổi; thay cho merge sort tree / persistent. | [wavelet-matrix.hpp](../02_data_structures/wavelet-matrix.hpp) |
| `weighted-dsu` | phương trình hiệu, tọa độ tương đối; thêm ràng buộc online và hỏi hiệu/kiểm mâu thuẫn. | [weighted-dsu.hpp](../02_data_structures/weighted-dsu.hpp) |
| `window-mode` | cửa sổ trượt cần phần tử xuất hiện nhiều nhất với tie-break theo giá trị. | [window-mode.hpp](../02_data_structures/window-mode.hpp) |

### 03 — Đồ thị

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `bellman-ford` | có cạnh trọng số âm; cần biết đỉnh bị chu trình âm chi phối hoặc cần xuất chính chu trình âm. | [bellman-ford.hpp](../03_graphs/bellman-ford.hpp) |
| `bfs-01` | đồ thị/lưới mà mỗi cạnh tốn 0 hoặc 1 (vd đi thẳng tốn 0, đổi hướng tốn 1). | [bfs-01.hpp](../03_graphs/bfs-01.hpp) |
| `bfs` | đường đi ngắn nhất đồ thị không trọng số, khoảng cách theo tầng, thành phần liên thông. | [bfs.hpp](../03_graphs/bfs.hpp) |
| `block-cut-tree` | khớp, các "khối" không bị tách bởi một đỉnh, truy vấn đường đi tránh một đỉnh. | [block-cut-tree.hpp](../03_graphs/block-cut-tree.hpp) |
| `bridges-articulation` | cạnh/đỉnh mà bỏ đi làm đồ thị mất liên thông; xử lý mạng đường, thành phần 2-cạnh/2-đỉnh. | [bridges-articulation.hpp](../03_graphs/bridges-articulation.hpp) |
| `dfs` | flood fill/thành phần; duyệt sâu đồ thị tới 2e5+ đỉnh mà recursion có thể tràn stack. | [dfs.hpp](../03_graphs/dfs.hpp) |
| `dijkstra` | đường đi ngắn nhất có trọng số >= 0 (đồ thị hoặc lưới). | [dijkstra.hpp](../03_graphs/dijkstra.hpp) |
| `dinic` | max-flow / min-cut, ghép đôi hai phía, số đường đi cạnh-rời, bài toán mô hình hoá thành luồng. | [dinic.hpp](../03_graphs/dinic.hpp) |
| `dominator-tree` | tìm các đỉnh nằm trên MỌI đường từ source tới v; control-flow graph; critical vertices. | [dominator-tree.hpp](../03_graphs/dominator-tree.hpp) |
| `edge-orientation` | cần hướng cạnh không chu trình; hoặc chọn hướng sao cho bậc ra mỗi đỉnh có parity 0. | [edge-orientation.hpp](../03_graphs/edge-orientation.hpp) |
| `eulerian-path` | cần lộ trình dùng hết cạnh (đường/chu trình Euler). | [eulerian-path.hpp](../03_graphs/eulerian-path.hpp) |
| `find-cycle` | đề đòi in ra một "round trip" (dãy đỉnh của chu trình), hoặc kiểm tra đồ thị là DAG / là rừng. | [find-cycle.hpp](../03_graphs/find-cycle.hpp) |
| `floyd-warshall` | cần khoảng cách mọi cặp đỉnh, n nhỏ; hoặc đóng gói quan hệ đạt tới. | [floyd-warshall.hpp](../03_graphs/floyd-warshall.hpp) |
| `havel-hakimi` | ghép cặp sao cho đỉnh i có đúng degree\[i\] hàng xóm, mỗi cặp nhiều nhất một cạnh. | [havel-hakimi.hpp](../03_graphs/havel-hakimi.hpp) |
| `hopcroft-karp` | ghép cực đại giữa hai tập (left/right); nền tảng cho định lý König, phủ đỉnh nhỏ nhất. | [hopcroft-karp.hpp](../03_graphs/hopcroft-karp.hpp) |
| `hungarian` | gán n hàng cho n (hoặc m >= n) cột phân biệt sao cho tổng chi phí nhỏ nhất (assignment problem). | [hungarian.hpp](../03_graphs/hungarian.hpp) |
| `k-shortest-paths` | cần k route ngắn nhất trên đồ thị có trọng số không âm, k nhỏ; đường đi không bắt buộc đơn. | [k-shortest-paths.hpp](../03_graphs/k-shortest-paths.hpp) |
| `min-cost-max-flow` | bài toán luồng có chi phí mỗi đơn vị trên cạnh; phân công (assignment); ghép có trọng số. | [min-cost-max-flow.hpp](../03_graphs/min-cost-max-flow.hpp) |
| `mst` | nối mọi đỉnh với tổng trọng số nhỏ nhất; đồ thị không liên thông -> trả rừng khung nhỏ nhất. | [mst.hpp](../03_graphs/mst.hpp) |
| `scc` | rút gọn chu trình có hướng thành DAG, đếm/gộp thành phần, tiền xử lý 2-SAT. | [scc.hpp](../03_graphs/scc.hpp) |
| `topological-sort` | xếp thứ tự phụ thuộc, DP trên DAG, kiểm tra đồ thị có chu trình hay không. | [topological-sort.hpp](../03_graphs/topological-sort.hpp) |
| `two-sat` | ràng buộc dạng "chọn 1 trong 2 / nếu ... thì ..."; mỗi mệnh đề đúng 2 literal. | [two-sat.hpp](../03_graphs/two-sat.hpp) |

### 04 — Cây

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `binary-lifting` | lặp hàm/next-greater nhiều bước; query tổ tiên; min/max/sum/xor cạnh trên path cây tĩnh. | [binary-lifting.hpp](../04_trees/binary-lifting.hpp) |
| `centroid-decomposition` | đếm cặp theo khoảng cách; hoặc online add đỉnh active + hỏi khoảng cách tới active gần nhất. | [centroid-decomposition.hpp](../04_trees/centroid-decomposition.hpp) |
| `euler-tour` | cây con của v là đoạn liên tục \[tin\[v\], tout\[v\]); ghép với Fenwick/segment-tree cho tổng/cập nhật cây con. | [euler-tour.hpp](../04_trees/euler-tour.hpp) |
| `hld` | cập nhật điểm + hỏi TỔNG trên đường u-v (theo đỉnh) hoặc cây con; đổi phép gộp segtree sang max/min tùy bài. | [hld.hpp](../04_trees/hld.hpp) |
| `lca-binary-lifting` | nhiều truy vấn LCA / khoảng cách hai đỉnh / nhảy lên k bậc trên cây tĩnh có gốc. | [lca-binary-lifting.hpp](../04_trees/lca-binary-lifting.hpp) |
| `long-path-decomposition` | duy nhất bài đếm pair/path có khoảng cách CỐ ĐỊNH cần nhanh hơn centroid O(n log n). | [long-path-decomposition.hpp](../04_trees/long-path-decomposition.hpp) |
| `prufer` | đề cho dãy Prüfer đòi dựng lại cây (hoặc ngược lại); sinh cây có nhãn ngẫu nhiên ĐỀU; đếm cây có nhãn (Cayley n^(n-2)). | [prufer.hpp](../04_trees/prufer.hpp) |
| `rerooting` | cần đại lượng phụ thuộc "toàn cây nhìn từ v" cho mọi v (tổng/khoảng cách xa nhất/đếm...) trên CÂY. | [rerooting.hpp](../04_trees/rerooting.hpp) |
| `small-to-large` | truy vấn tổng hợp trên MỌI cây con (đếm màu phân biệt, tần suất...) khi giữ được tập của con lớn nhất. | [small-to-large.hpp](../04_trees/small-to-large.hpp) |
| `tree-augmentation` | nối cặp lá của cây để sau khi thêm cạnh, bỏ bất kỳ một cạnh cũ vẫn liên thông. | [tree-augmentation.hpp](../04_trees/tree-augmentation.hpp) |
| `tree-diameter` | tìm độ dài + hai đầu mút đường kính; đỉnh xa nhất từ một đỉnh; nền cho nhiều bài cây. | [tree-diameter.hpp](../04_trees/tree-diameter.hpp) |
| `tree-dp` | đề hỏi đáp án cho TỪNG đỉnh (tổng/max khoảng cách tới các đỉnh khác) hoặc DP hai trạng thái dp\[v\]\[0/1\] trên cây. | [tree-dp.hpp](../04_trees/tree-dp.hpp) |
| `tree-isomorphism` | so sánh hình dạng hai cây khi bỏ qua nhãn và thứ tự các con. | [tree-isomorphism.hpp](../04_trees/tree-isomorphism.hpp) |
| `tree-reconstruction` | biết thứ tự gốc-trái-phải và trái-gốc-phải của các nhãn phân biệt. | [tree-reconstruction.hpp](../04_trees/tree-reconstruction.hpp) |

### 05 — Quy hoạch động

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `bitmask-dp` | n nhỏ (<= 20) cần duyệt mọi tập đỉnh đã thăm; TSP, đếm/đường Hamilton, phủ tập con. | [bitmask-dp.hpp](../05_dynamic_programming/bitmask-dp.hpp) |
| `broken-profile-dp` | đếm cách lát domino/xếp gạch trên lưới có MỘT chiều nhỏ (min(n, m) <= ~14), có hoặc không ô bị chặn. | [broken-profile-dp.hpp](../05_dynamic_programming/broken-profile-dp.hpp) |
| `coin-change` | mỗi loại xu dùng bao nhiêu lần cũng được; hỏi min số xu, hoặc đếm cách (có/không phân biệt thứ tự). | [coin-change.hpp](../05_dynamic_programming/coin-change.hpp) |
| `digit-dp` | đếm số trong đoạn theo ràng buộc chữ số (kề nhau, tổng chữ số, chứa mẫu...); ghép f(b)-f(a-1) cho \[a, b\]. | [digit-dp.hpp](../05_dynamic_programming/digit-dp.hpp) |
| `edit-distance` | đo độ khác nhau giữa hai xâu; nền cho so khớp mờ, gợi ý sửa lỗi. | [edit-distance.hpp](../05_dynamic_programming/edit-distance.hpp) |
| `grid-dp` | trạng thái phụ thuộc ô trên/trái/chéo; đếm đường đi phải-xuống; DP hình vuông trên lưới. | [grid-dp.hpp](../05_dynamic_programming/grid-dp.hpp) |
| `interval-dp` | bài toán gộp/tách/chọn trên đoạn liên tục mà lời giải phụ thuộc hai đầu; game lấy đầu-cuối. | [interval-dp.hpp](../05_dynamic_programming/interval-dp.hpp) |
| `knapsack` | tối ưu dưới sức chứa; mỗi vật dùng một lần hoặc mỗi loại có giới hạn count\[i\]. | [knapsack.hpp](../05_dynamic_programming/knapsack.hpp) |
| `lcs` | đo phần chung theo THỨ TỰ (không cần liền kề) giữa hai dãy/xâu; so khớp phiên bản. | [lcs.hpp](../05_dynamic_programming/lcs.hpp) |
| `linear-dp` | trạng thái lớp i chỉ phụ thuộc lớp i-1; cần giảm O(số_bước*width) bộ nhớ xuống O(width); DP theo tiền tố đã sort. | [linear-dp.hpp](../05_dynamic_programming/linear-dp.hpp) |
| `lis` | tìm dãy con tăng dài nhất; nền cho nhiều bài (xếp hộp, lịch, phủ...). | [lis.hpp](../05_dynamic_programming/lis.hpp) |
| `slope-trick` | "mỗi thao tác ±1 một phần tử, tổng thao tác nhỏ nhất để dãy không giảm (hoặc tăng ngặt)". | [slope-trick.hpp](../05_dynamic_programming/slope-trick.hpp) |
| `sos-dp` | cần tổng/tích hợp theo quan hệ tập con-tập cha trên toàn bộ 2^n mask (đếm cặp AND/OR, tích chập tập con). | [sos-dp.hpp](../05_dynamic_programming/sos-dp.hpp) |
| `subset-sum-trace` | tổng giới hạn khá lớn; cần vừa kiểm tra reachability vừa dựng một tập chỉ số tạo ra tổng. | [subset-sum-trace.hpp](../05_dynamic_programming/subset-sum-trace.hpp) |
| `weighted-interval-scheduling` | công việc/dự án có start, finish, profit; nhận việc sau chỉ khi start > finish trước. | [weighted-interval-scheduling.hpp](../05_dynamic_programming/weighted-interval-scheduling.hpp) |

### 06 — Xâu

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `aho-corasick` | đếm/định vị nhiều mẫu trong một văn bản; mẫu nào xuất hiện; đếm số lần mỗi mẫu (kể cả chồng lấn). | [aho-corasick.hpp](../06_strings/aho-corasick.hpp) |
| `bwt` | đề cho cột cuối của các rotation đã sắp xếp; nén chuỗi/FM-index; cần inverse BWT có sentinel duy nhất. | [bwt.hpp](../06_strings/bwt.hpp) |
| `distinct-subsequences` | đếm subsequence phân biệt của một xâu dài; cần loại trùng do các ký tự lặp lại. | [distinct-subsequences.hpp](../06_strings/distinct-subsequences.hpp) |
| `eertree` | đếm palindrome con PHÂN BIỆT, palindrome dài nhất kết thúc tại mỗi vị trí, tổng số palindrome con. | [eertree.hpp](../06_strings/eertree.hpp) |
| `kmp-automaton` | đếm xâu độ dài n KHÔNG chứa (hoặc CÓ chứa) một mẫu; DP với trạng thái "đã khớp bao nhiêu ký tự đầu của mẫu"; theo dõi số lần mẫu xuất hiện khi ghép d… | [kmp-automaton.hpp](../06_strings/kmp-automaton.hpp) |
| `kmp` | đếm/định vị mọi lần xuất hiện của mẫu; tìm biên dài nhất; chu kỳ nhỏ nhất của xâu. | [kmp.hpp](../06_strings/kmp.hpp) |
| `manacher` | tìm xâu con đối xứng dài nhất; đếm số xâu con là palindrome; tiền xử lý cho truy vấn palindrome. | [manacher.hpp](../06_strings/manacher.hpp) |
| `min-rotation` | chuẩn hóa xâu tuần hoàn (so hai xâu có là quay của nhau), biểu diễn nhỏ nhất của vòng. | [min-rotation.hpp](../06_strings/min-rotation.hpp) |
| `string-hashing` | so sánh nhanh hai đoạn con bằng nhau, đếm đoạn phân biệt, tìm mẫu, kiểm palindrome. | [string-hashing.hpp](../06_strings/string-hashing.hpp) |
| `suffix-array` | đếm xâu con phân biệt, tìm xâu con lặp dài nhất, so khớp mẫu, thứ tự từ điển các hậu tố. | [suffix-array.hpp](../06_strings/suffix-array.hpp) |
| `suffix-automaton` | đếm xâu con phân biệt; tìm pattern sớm nhất; số lần xuất hiện; xâu con thứ k (phân biệt hoặc tính cả lặp). | [suffix-automaton.hpp](../06_strings/suffix-automaton.hpp) |
| `z-function` | khớp mẫu (nối p + sep + t), tìm biên/chu kỳ, so tiền tố nhanh; thay thế KMP trong nhiều bài. | [z-function.hpp](../06_strings/z-function.hpp) |

### 07 — Toán

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `burnside` | đếm cấu hình trên vòng tròn mà QUAY được coi là giống nhau (vòng cổ, tô màu đỉnh đa giác, xâu tuần hoàn). | [burnside.hpp](../07_mathematics/burnside.hpp) |
| `combinatorics` | nhiều truy vấn tổ hợp/chỉnh hợp theo mod nguyên tố; Catalan, stars-and-bars, hoán vị lặp. | [combinatorics.hpp](../07_mathematics/combinatorics.hpp) |
| `divisor-blocks` | tổng số ước / tổng các ước của mọi số trong \[1, n\], đếm bội (chuỗi điều hòa), n rất lớn (tới 1e12+). | [divisor-blocks.hpp](../07_mathematics/divisor-blocks.hpp) |
| `divisor-sieve` | gcd lớn nhất của một nhóm phần tử; đếm subset theo gcd; dữ liệu dương có giá trị tối đa vừa phải. | [divisor-sieve.hpp](../07_mathematics/divisor-sieve.hpp) |
| `eulerian-numbers` | đếm phân phối số descent/ascents/runs; số lượt Collecting Numbers = 1 + descent của hoán vị nghịch đảo. | [eulerian-numbers.hpp](../07_mathematics/eulerian-numbers.hpp) |
| `extended-euclid` | nghịch đảo modulo khi mod KHÔNG nguyên tố; giải hệ đồng dư (CRT); nghiệm phương trình a*x + b*y = c. | [extended-euclid.hpp](../07_mathematics/extended-euclid.hpp) |
| `factorization` | n QUÁ LỚN để sàng (tới ~1.8e19): kiểm nguyên tố, phân tích thừa số, đếm/tổng ước của số lớn. | [factorization.hpp](../07_mathematics/factorization.hpp) |
| `gaussian-elimination` | giải/đếm nghiệm hệ tuyến tính theo mod nguyên tố (kể cả GF(2) với p = 2); tìm hạng ma trận. | [gaussian-elimination.hpp](../07_mathematics/gaussian-elimination.hpp) |
| `inclusion-exclusion` | "đếm x trong \[1,n\] chia hết bởi ít nhất một trong k số", "đếm hoán vị mà không ai giữ vị trí của mình". | [inclusion-exclusion.hpp](../07_mathematics/inclusion-exclusion.hpp) |
| `integer-math` | tránh sai số sqrt; trailing zeros/factorial prime exponent; chu kỳ gcd/lcm; nhóm số theo độ dài. | [integer-math.hpp](../07_mathematics/integer-math.hpp) |
| `josephus` | đề "vòng tròn, đếm k rồi loại": cần thứ tự bị loại, ai sống sót, hoặc người bị loại thứ q (k = 2). | [josephus.hpp](../07_mathematics/josephus.hpp) |
| `kth-permutation` | đề cho k và hỏi hoán vị thứ k theo thứ tự từ điển, hoặc cho hoán vị và hỏi nó đứng thứ mấy. | [kth-permutation.hpp](../07_mathematics/kth-permutation.hpp) |
| `labeled-combinatorics` | đếm cấu trúc theo đúng k thành phần; công thức Lagrange hoặc ghép dãy SCC có nhãn. | [labeled-combinatorics.hpp](../07_mathematics/labeled-combinatorics.hpp) |
| `lucas` | tổ hợp mod nguyên tố nhỏ (p <= n), đếm đường đi/tam giác Pascal mod 2, XOR pyramid, bài "C(n,k) chẵn/lẻ". | [lucas.hpp](../07_mathematics/lucas.hpp) |
| `matrix-exponentiation` | số Fibonacci/hồi quy tuyến tính bậc k; đếm số đường đi đúng k cạnh trên đồ thị; k rất lớn. | [matrix-exponentiation.hpp](../07_mathematics/matrix-exponentiation.hpp) |
| `mobius` | đếm theo nghịch đảo Mobius (đếm cặp nguyên tố cùng nhau, số không chia hết...), tổng phi, hàm nhân tính. | [mobius.hpp](../07_mathematics/mobius.hpp) |
| `modular-arithmetic` | tính a^e mod m, nghịch đảo modulo khi m NGUYÊN TỐ; nền cho tổ hợp, quy hoạch đếm theo mod. | [modular-arithmetic.hpp](../07_mathematics/modular-arithmetic.hpp) |
| `ntt` | nhân đa thức/sinh hàm/tương quan; cần hệ số thật không âm nhỏ hơn tích hai modulus. | [ntt.hpp](../07_mathematics/ntt.hpp) |
| `probability-expectation` | tổng biến ngẫu nhiên rời rạc; E\[max\]; E\[số inversion\]; E\[số state rỗng sau các Markov walk độc lập\]. | [probability-expectation.hpp](../07_mathematics/probability-expectation.hpp) |
| `semiring-matrix` | matrix exponentiation mà phép cộng/nhân không phải +/*; min-plus shortest walk; boolean reachability. | [semiring-matrix.hpp](../07_mathematics/semiring-matrix.hpp) |
| `sieve` | kiểm tra nguyên tố, phân tích thừa số, đếm/tổng ước, các hàm số học nhân tính tới n. | [sieve.hpp](../07_mathematics/sieve.hpp) |
| `sprague-grundy` | trò chơi công bằng (impartial) tổng của nhiều ván độc lập; xác định thắng/thua bằng XOR các Grundy. | [sprague-grundy.hpp](../07_mathematics/sprague-grundy.hpp) |
| `walsh-hadamard` | đếm cặp/bộ theo XOR trên miền giá trị nhỏ (< 2^20), luỹ thừa tích chập XOR, tập giá trị XOR đạt được. | [walsh-hadamard.hpp](../07_mathematics/walsh-hadamard.hpp) |
| `xor-basis` | XOR lớn nhất của tập con; đếm số giá trị XOR phân biệt (= 2^rank); giá trị XOR nhỏ thứ k; hạng GF(2). | [xor-basis.hpp](../07_mathematics/xor-basis.hpp) |

### 08 — Hình học

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `closest-pair` | tìm cặp điểm gần nhau nhất; kết quả dùng bình phương khoảng cách (long long, CHÍNH XÁC, không sai số). | [closest-pair.hpp](../08_geometry/closest-pair.hpp) |
| `convex-hull` | tìm bao lồi; nền cho đường kính tập điểm, kiểm điểm trong bao, tối ưu trên bao. | [convex-hull.hpp](../08_geometry/convex-hull.hpp) |
| `geometry-basics` | nền tảng mọi bài hình học nguyên; kiểm thẳng hàng / trái-phải / trên đoạn; dùng chung cho các file khác. | [geometry-basics.hpp](../08_geometry/geometry-basics.hpp) |
| `line-intersection` | tìm giao điểm hai đường thẳng; dựng đường thẳng qua hai điểm; kiểm song song / trùng. | [line-intersection.hpp](../08_geometry/line-intersection.hpp) |
| `manhattan-distance` | thêm điểm rồi hỏi khoảng cách L1 lớn nhất; tính tổng \|dx\|+\|dy\| trên mọi cặp điểm. | [manhattan-distance.hpp](../08_geometry/manhattan-distance.hpp) |
| `point-in-polygon` | truy vấn điểm thuộc đa giác (kể cả đa giác lõm); phân loại điểm so với đa giác đỉnh nguyên. | [point-in-polygon.hpp](../08_geometry/point-in-polygon.hpp) |
| `polygon-area` | tính diện tích đa giác đơn; đếm điểm nguyên trong/trên đa giác đỉnh nguyên (Pick: A = I + B/2 - 1). | [polygon-area.hpp](../08_geometry/polygon-area.hpp) |
| `rotating-calipers` | cặp điểm xa nhau nhất; bề rộng; các bài tối ưu trên hai điểm đối xứng của bao lồi. | [rotating-calipers.hpp](../08_geometry/rotating-calipers.hpp) |
| `segment-intersection` | phát hiện hai đoạn cắt nhau; đếm giao điểm (kết hợp quét); kiểm đa giác tự cắt. | [segment-intersection.hpp](../08_geometry/segment-intersection.hpp) |

### 09 — Thuật toán offline

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `cdq-divide-conquer` | với mỗi i cần đếm số j ĐỨNG TRƯỚC mà x_j <= x_i và y_j <= y_i (bộ ba đơn điệu, đếm điểm bị trội). | [cdq-divide-conquer.hpp](../09_offline_algorithms/cdq-divide-conquer.hpp) |
| `mo-algorithm` | truy vấn đoạn tĩnh mà thêm/bớt MỘT phần tử ở hai đầu là rẻ (đếm phân biệt, tần suất, nghịch thế...). | [mo-algorithm.hpp](../09_offline_algorithms/mo-algorithm.hpp) |
| `offline-dynamic-connectivity` | đồ thị thay đổi (thêm/xóa cạnh) offline; truy vấn liên thông theo thời điểm; đếm thành phần theo thời gian. | [offline-dynamic-connectivity.hpp](../09_offline_algorithms/offline-dynamic-connectivity.hpp) |
| `offline-lca` | biết trước toàn bộ cặp truy vấn (offline) và muốn nhanh/gọn hơn binary lifting; nền cho khoảng cách cây. | [offline-lca.hpp](../09_offline_algorithms/offline-lca.hpp) |
| `parallel-binary-search` | mỗi truy vấn cần tìm mốc thời gian nhỏ nhất thỏa điều kiện đơn điệu, mà mô phỏng lại từng truy vấn thì chậm. | [parallel-binary-search.hpp](../09_offline_algorithms/parallel-binary-search.hpp) |
| `sqrt-decomposition` | range-update + range-query khi muốn code ngắn hơn lazy segment tree; nền cho nhiều biến thể khối. | [sqrt-decomposition.hpp](../09_offline_algorithms/sqrt-decomposition.hpp) |

### 10 — Kỹ thuật nâng cao

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `bitset-reachability` | DAG cần "từ u tới được bao nhiêu đỉnh" hoặc "u có tới được v?" với n <= ~5e4 (đồ thị bất kỳ: nén SCC trước). | [bitset-reachability.hpp](../10_advanced_techniques/bitset-reachability.hpp) |
| `constructive` | đề chấp nhận bất kỳ cấu hình hợp lệ; cần xây trực tiếp rồi chứng minh invariant thay vì tối ưu/tìm kiếm. | [constructive.hpp](../10_advanced_techniques/constructive.hpp) |
| `divide-conquer-dp` | chia mảng thành ĐÚNG k đoạn liên tiếp, chi phí mỗi đoạn cho bởi cost(l, r) thỏa bất đẳng thức tứ giác. | [divide-conquer-dp.hpp](../10_advanced_techniques/divide-conquer-dp.hpp) |
| `functional-graph` | bài "teleporter"/hàm kế tiếp: đi đúng k bước từ x; số bước tới khi gặp đỉnh đã thăm; đường đi ngắn nhất a -> b. | [functional-graph.hpp](../10_advanced_techniques/functional-graph.hpp) |
| `interactive-search` | bài interactive có giới hạn truy vấn; muốn tách logic thuật toán khỏi cin/cout để mock judge; một query trả thông tin cho nhiều vị trí. | [interactive-search.hpp](../10_advanced_techniques/interactive-search.hpp) |
| `k-best-combinations` | n lớn, không thể duyệt C(n,m), nhưng chỉ cần một số ít tổng nhỏ nhất. | [k-best-combinations.hpp](../10_advanced_techniques/k-best-combinations.hpp) |
| `k-best-enumeration` | n lớn nhưng chỉ cần k tổng tập con nhỏ nhất; phần tử có thể âm; tính cả multiplicity theo tập chỉ số. | [k-best-enumeration.hpp](../10_advanced_techniques/k-best-enumeration.hpp) |
| `knuth-optimization` | bài "cắt/gộp đoạn" mà chi phí thỏa bất đẳng thức tứ giác (vd chi phí cắt = tổng đoạn, gộp đá, Huffman-kiểu). | [knuth-optimization.hpp](../10_advanced_techniques/knuth-optimization.hpp) |
| `kruskal-reconstruction-tree` | hỏi "cạnh lớn nhất nhỏ nhất" trên đường u-v; ngày sớm nhất hai đỉnh liên thông; truy vấn theo ngưỡng trọng số. | [kruskal-reconstruction-tree.hpp](../10_advanced_techniques/kruskal-reconstruction-tree.hpp) |
| `meet-in-the-middle` | n <= ~40 (2^n quá lớn nhưng 2^(n/2) ổn): đếm/tìm tập con theo tổng, bài "tổng bằng S". | [meet-in-the-middle.hpp](../10_advanced_techniques/meet-in-the-middle.hpp) |
| `multiset-reconstruction` | biết n và n(n-1)/2 pair sums (có lặp); cần tìm bất kỳ dãy phù hợp hoặc kết luận vô nghiệm. | [multiset-reconstruction.hpp](../10_advanced_techniques/multiset-reconstruction.hpp) |
| `ternary-search` | cần điểm/giá trị nhỏ nhất của hàm "giảm rồi tăng" mà không có công thức đóng (tìm MAX thì dùng với -f). | [ternary-search.hpp](../10_advanced_techniques/ternary-search.hpp) |

### 11 — Greedy (tham lam trên đoạn / lịch biểu)

| Kỹ thuật | Khi nào dùng (dấu hiệu đề) | File |
|---|---|---|
| `greedy-array` | quyết định trái-phải bắt buộc; các đồng dương đã sort mở rộng đoạn tổng tạo được; cận dưới tải/largest job. | [greedy-array.hpp](../11_greedy/greedy-array.hpp) |
| `heap-scheduling` | đoạn thời gian chia cho k "người/máy" sao cho mỗi người không xem hai đoạn chồng lấn; hoặc xếp thứ tự việc để tối thiểu tổng thời điểm hoàn thành. | [heap-scheduling.hpp](../11_greedy/heap-scheduling.hpp) |
| `heap-selection` | thử mọi split nhưng mỗi phía phải giữ k điểm lớn nhất; lấy phần tử tốt nhất trong cửa sổ ứng viên động; k rất lớn không thể mô phỏng vòng. | [heap-selection.hpp](../11_greedy/heap-selection.hpp) |
| `interval-scheduling` | chọn nhiều nhất phim/hoạt động không trùng giờ; chia mọi đoạn vào ít phòng/máy nhất; đo số đoạn chồng nhau nhiều nhất. | [interval-scheduling.hpp](../11_greedy/interval-scheduling.hpp) |
| `optimal-merge` | cắt thanh đảo ngược thành ghép; nối file/rope; xây cây prefix khi mỗi lần ghép tốn tổng hai phần. | [optimal-merge.hpp](../11_greedy/optimal-merge.hpp) |
| `path-matching` | chọn đúng k cạnh không chung đỉnh trên path; cần đáp án cho nhiều k (CSES Minimum Cost Pairs). | [path-matching.hpp](../11_greedy/path-matching.hpp) |

<!-- GEN:END -->

# Bẫy — checklist chống WA / RE / TLE / MLE

Đọc mục "Bẫy chí tử" trước khi nộp; phần cuối là **toàn bộ dòng `Bẫy:`** của 98 snippet (tự sinh, luôn khớp code).

## 14 bẫy chí tử (phần lớn đã thực sự gây sai trong kho này)

1. **Mảng đếm theo GIÁ TRỊ phải nén trước.** `cnt[a[i]]` với `a[i]` tới 1e9 → cấp phát 4 GB → MLE ngay
   (đúng lỗi từng có ở `mo-algorithm`, nay đã tự nén bên trong). Dùng `coordinate-compression` khi chỉ cần thứ hạng.
2. **Nhân theo mod > ~3e9 phải ép `(__int128)`.** `long long * long long % mod` tràn âm im lặng
   (từng sai ở `combinatorics` và `extended-euclid`/CRT). Với mod ≤ 1e9 thì `long long` là đủ.
3. **`mulmod` cho n gần 2^64 phải dùng `(unsigned __int128)`.** `__int128` CÓ DẤU vẫn tràn khi tích ~n² > 2^127
   (từng làm `factorization`/Miller-Rabin báo số nguyên tố là hợp số).
4. **Ternary search chỉ đúng khi hàm GIẢM NGẶT rồi TĂNG NGẶT.** Hàm có đoạn bằng nhau (sinh từ chia nguyên,
   `max`, `ceil`) làm `ternaryMin` trả sai im lặng → dùng `convexMin`; hàm chỉ "gần lồi" (`ceil(A/x)+B·x`) thì
   phải quét thêm ±10 quanh kết quả.
5. **ĐPT phải tính CẢ phần phụ.** Sort truy vấn trong `mo-algorithm`, dựng lại DSU mỗi vòng trong
   `parallel-binary-search`, khử trùng bằng `set` trong `scc.condensation` — đều là số hạng thật.
6. **DSU cần hoàn tác thì KHÔNG được nén đường.** `rollback-dsu` và `offline-dynamic-connectivity` hợp theo
   rank/size để rollback đúng — thay bằng DSU nén đường là sai.
7. **Bộ nhớ n²/8.** `bitset-reachability` với n = 5e4 tốn ~312 MB. Kiểm giới hạn bộ nhớ trước khi dùng.
8. **Đoạn đóng `[l, r]` vs nửa mở `[l, r)`.** Trong kho: `fenwick`/`segment-tree`/`euler-tour`/`divide-conquer-dp`
   dùng NỬA MỞ; `sqrt-decomposition`/`mo-algorithm`/`lazy-segment-tree` dùng ĐÓNG. Đọc dòng `Dùng:` trước khi gọi.
9. **1-based của đề vs 0-based của snippet.** Mọi snippet đồ thị/cây dùng 0-based → nhớ `--a; --b;` khi đọc input.
10. **`main` trần trong file được include sẽ phá bundle.** Demo phải nằm trong `#ifdef CP_DEMO`.
11. **Hình học: đừng định nghĩa lại `Point`/`cross`.** Các file `08_geometry` chia sẻ khối guard `CP_GEO_BASE`;
    tự khai báo lại (như template cũ từng làm) gây `redefinition of struct Point` khi bundle.
12. **Tọa độ hình học > ~1e9 làm `cross` tràn `long long`.** Khi đó phải đổi sang `__int128`.
13. **Hợp đồng HAI PHA: nạp hết dữ liệu rồi mới `build()`/`solve()`.** Thêm cạnh/mẫu sau khi đã build là sai im lặng:
    `scc.build()`, `aho-corasick.build()`, `two-sat.solve()`, `hopcroft-karp.maxMatching()` (đọc `matchL` sau đó),
    `offline-dynamic-connectivity.solve()`, `kruskal-reconstruction-tree` (cạnh phải sắp tăng TRƯỚC khi dựng).
14. **Có hàm chỉ gọi được MỘT lần** vì làm thay đổi trạng thái: `eertree.countAllPalindromes()` (lan `cnt`),
    `rollback-dsu.rollback()` (đúng thứ tự LIFO). Ngược lại `centroid-decomposition.countPaths()` và
    `parallel-binary-search` thì tự reset nên gọi lại được — cứ đọc dòng `Bẫy:` của từng file.

## Quy trình 30 giây trước khi nộp

- Kiểu số: mọi tổng/tích có thể vượt `int` → `long long`? (đặc biệt tổng n·max = 2e5·1e9 = 2e14)
- Trường hợp biên: n = 1, mảng rỗng, mọi phần tử bằng nhau, đồ thị không liên thông, đáp án = 0 hay -1?
- Đọc lại dòng `Bẫy:` của snippet vừa dán.
- Nếu còn nghi ngờ tính đúng: `bash cp_library/templates/stress.sh main.cpp brute.cpp gen.cpp 500`.

## Toàn bộ dòng "Bẫy" theo nhóm

<!-- GEN:BEGIN (tự sinh bởi tools/gen_docs.py — đừng sửa tay) -->

### 01 — Nền tảng (mảng, đoạn, hai con trỏ, bitmask)

- [`backtracking`](../01_foundations/backtracking.hpp): undo phải đảo CHÍNH XÁC mọi thay đổi của take; depth đệ quy quá lớn sẽ tràn stack; xếp ứng viên tốt trước để prune.
- [`binary-search`](../01_foundations/binary-search.hpp): mảng phải sort theo <; pred phải đơn điệu; sentinel hi+1/lo-1 không biểu diễn được ở biên LLONG thì ném overflow_error.
- [`bit-operations`](../01_foundations/bit-operations.hpp): packBinary64 chỉ nhận tối đa 64 ký tự '0'/'1'; tổng bit tới n=2^64-1 cần unsigned __int128.
- [`comparison-sort`](../01_foundations/comparison-sort.hpp): oracleLess(a,b) phải là strict-weak-order và KHÔNG được thay đổi thứ tự ẩn; merge gọi oracleLess(phải,trái) để ưu tiên bản trái khi tương đương.
- [`coordinate-compression`](../01_foundations/coordinate-compression.hpp): id trả hạng theo lower_bound nên với x không có trong tập vẫn cho vị trí chèn.
- [`difference-array`](../01_foundations/difference-array.hpp): đoạn nửa mở \[l, r); dùng long long; add hết rồi mới build một lần.
- [`dynamic-bitset`](../01_foundations/dynamic-bitset.hpp): chỉ số 0-based \[0,n); shift âm bị từ chối; shiftOr là IN-PLACE nhưng duyệt cao xuống nên đúng 0/1 knapsack.
- [`frequency-counting`](../01_foundations/frequency-counting.hpp): unordered_map có worst-case O(n²); cần custom hash khi input đối kháng; mex chỉ xét giá trị trong \[0,n\].
- [`gray-code`](../01_foundations/gray-code.hpp): grayCode giữ CẢ 2^n xâu trong RAM — n <= 16 (CSES) là 64 Ki xâu; n lớn hơn nên in trực tiếp từ grayEncode(i).
- [`max-subarray`](../01_foundations/max-subarray.hpp): đoạn KHÔNG RỖNG — mọi phần tử âm thì đáp án là phần tử LỚN NHẤT (âm), tuyệt đối không trả 0;
- [`monotonic-deque`](../01_foundations/monotonic-deque.hpp): deque lưu INDEX; loại đầu khi index <= i-k; kết quả có đúng n-k+1 phần tử.
- [`monotonic-stack`](../01_foundations/monotonic-stack.hpp): quyết định phía nào giữ phần tử BẰNG (strict) để không đếm trùng đoạn.
- [`permutation-code`](../01_foundations/permutation-code.hpp): p phải chứa đúng 0..n-1; Lehmer l\[i\] thuộc \[0,n-1-i\], còn prefix code c\[i\] thuộc \[0,i\] — hai quy ước KHÔNG hoán đổi trực tiếp.
- [`permutation-cycles`](../01_foundations/permutation-cycles.hpp): p phải là hoán vị 0-based của \[0,n); bậc có thể cực lớn nên hàm order nhận cap và trả cap+1 khi vượt.
- [`prefix-sum`](../01_foundations/prefix-sum.hpp): dùng long long tránh tràn; r là biên phải MỞ; sum(0, n) = tổng cả mảng.
- [`prefix-xor`](../01_foundations/prefix-xor.hpp): đoạn là \[l,r), không phải \[l,r\]; rỗng trả 0; dùng uint64_t nếu cần đúng mẫu bit âm.
- [`simulation`](../01_foundations/simulation.hpp): step phải xác định; limit < 0 nghĩa là không giới hạn; Floyd chỉ dùng khi dãy cuối cùng đi vào chu trình.
- [`sliding-window`](../01_foundations/sliding-window.hpp): chỉ đúng khi điều kiện đơn điệu theo độ dài; countSubarrays cần phần tử KHÔNG âm.
- [`submask-enumeration`](../01_foundations/submask-enumeration.hpp): kiểm s == 0 SAU khi xử lý rồi mới break; (s-1)&m mới nhảy đúng submask kế.
- [`sweep-line`](../01_foundations/sweep-line.hpp): tie là phần cốt lõi: stabbing đoạn ĐÓNG thường OPEN < QUERY < CLOSE; đoạn NỬA MỞ thường CLOSE < OPEN < QUERY tại cùng x.
- [`two-pointers`](../01_foundations/two-pointers.hpp): input PHẢI đã sort tăng dần; chỉ số trả về theo mảng đã sort; đếm dùng long long.

### 02 — Cấu trúc dữ liệu

- [`binary-trie`](../02_data_structures/binary-trie.hpp): chọn B đủ lớn; maxXor yêu cầu tập khác rỗng; erase phải khớp giá trị đã insert.
- [`convex-hull-trick`](../02_data_structures/convex-hull-trick.hpp): CHỈ đúng với hai thứ tự trên; slope trùng tự giữ intercept nhỏ hơn; kết quả a*x+b phải vừa long long.
- [`disjoint-sparse-table`](../02_data_structures/disjoint-sparse-table.hpp): op phải KẾT HỢP; query kết hợp đúng thứ tự trái->phải nên non-commutative cũng đúng.
- [`dsu`](../02_data_structures/dsu.hpp): KHÔNG hỗ trợ tách/xóa cạnh — cần undo thì dùng rollback-dsu; API kiểm tra index.
- [`dynamic-segment-tree`](../02_data_structures/dynamic-segment-tree.hpp): dùng long long; pos trong \[0, N); node cấp phát động (không realloc-cache tham chiếu Node&).
- [`fenwick-2d`](../02_data_structures/fenwick-2d.hpp): tốn O(R·C) bộ nhớ — tọa độ lớn phải nén trước; dùng long long.
- [`fenwick`](../02_data_structures/fenwick.hpp): r/l là biên nửa mở; lowerBound/kth chỉ đúng khi mọi tần suất không âm; API kiểm tra index/range.
- [`implicit-treap`](../02_data_structures/implicit-treap.hpp): khóa theo VỊ TRÍ (không theo giá trị); nhớ push lazy trước khi đi xuống; dùng long long cho tổng.
- [`interval-set`](../02_data_structures/interval-set.hpp): đoạn ĐÓNG và tọa độ NGUYÊN; insert tự gộp cả đoạn kề nhau; optional rỗng nghĩa là chưa có điểm phủ.
- [`lazy-segment-tree`](../02_data_structures/lazy-segment-tree.hpp): assign GHI ĐÈ action cũ; mọi giá trị/tổng/độ dài phải vừa long long; count phủ không được âm.
- [`leftist-heap`](../02_data_structures/leftist-heap.hpp): meld/push/pop trả ROOT MỚI; không dùng lại hai root cũ sau meld; node đã pop không thu hồi.
- [`li-chao-tree`](../02_data_structures/li-chao-tree.hpp): query ngoài miền/rỗng và kết quả không vừa long long sẽ ném exception; hệ số vẫn là long long.
- [`median-maintenance`](../02_data_structures/median-maintenance.hpp): median() là median DƯỚI khi size chẵn; erase trả false nếu không thấy; tổng dùng long long.
- [`ordered-set`](../02_data_structures/ordered-set.hpp): set PHÂN BIỆT (không trùng); muốn multiset -> dùng ordered_set<pair<T,int>> gắn chỉ số phụ; chỉ dùng với GCC.
- [`persistent-segment-tree`](../02_data_structures/persistent-segment-tree.hpp): đoạn \[l,r), k 1-based; version của PersistentArraySum là 0-based; tổng phải vừa long long.
- [`rollback-dsu`](../02_data_structures/rollback-dsu.hpp): TUYỆT ĐỐI không thêm nén đường (find phải O(log n) để undo được); snapshot lấy trước khi unite.
- [`run-intervals`](../02_data_structures/run-intervals.hpp): vị trí 0-based; runRange trả đoạn NỬA MỞ \[l,r); flipBit chỉ nhận ký tự '0'/'1'; chuỗi rỗng có maxRun=0.
- [`segment-tree`](../02_data_structures/segment-tree.hpp): id phải trung hòa; op kết hợp; pred của findFirst phải đơn điệu false->true khi nối thêm bên phải.
- [`sparse-table`](../02_data_structures/sparse-table.hpp): CHỈ dùng cho phép idempotent (đoạn chồng lấn vẫn đúng); tổng thì KHÔNG được.
- [`treap`](../02_data_structures/treap.hpp): cho phép trùng (multiset); kth dùng 1-based; erase(x) chỉ xóa MỘT bản; priority ngẫu nhiên cố định seed.
- [`trie`](../02_data_structures/trie.hpp): ALPHA và gốc 'a' cố định — đổi bảng chữ cái phải sửa; ký tự ngoài \['a','z'\] sẽ hỏng.
- [`wavelet-matrix`](../02_data_structures/wavelet-matrix.hpp): giá trị phải KHÔNG âm (âm thì cộng offset trước); đoạn nửa mở \[l, r); mảng tĩnh (không cập nhật).
- [`weighted-dsu`](../02_data_structures/weighted-dsu.hpp): delta và tổng thế dùng long long; false nghĩa là ràng buộc mới mâu thuẫn; không hỗ trợ xóa/rollback.
- [`window-mode`](../02_data_structures/window-mode.hpp): remove(x) trả false nếu x không có; mode() là nullopt khi rỗng; T phải so sánh được bằng std::less.

### 03 — Đồ thị

- [`bellman-ford`](../03_graphs/bellman-ford.hpp): mọi tổng đường đi hữu hạn cần nằm NGHIÊM NGẶT trong (-BF_INF, BF_INF), vì hai biên là sentinel;
- [`bfs-01`](../03_graphs/bfs-01.hpp): trọng số PHẢI là 0 hoặc 1; cạnh 0 đẩy front, cạnh 1 đẩy back; nếu trọng số bất kỳ dùng dijkstra.
- [`bfs`](../03_graphs/bfs.hpp): chỉ đúng khi MỌI cạnh cùng trọng số; trọng số 0/1 dùng bfs-01, không âm dùng dijkstra.
- [`block-cut-tree`](../03_graphs/block-cut-tree.hpp): đồ thị VÔ HƯỚNG, addEdge đúng một lần/cạnh; hỗ trợ cạnh song song.
- [`bridges-articulation`](../03_graphs/bridges-articulation.hpp): đồ thị VÔ HƯỚNG; addEdge một lần cho mỗi cạnh; khuyên (self-loop) không là cầu; bản lặp không tràn stack.
- [`dfs`](../03_graphs/dfs.hpp): đồ thị có hướng vẫn duyệt reachability theo chiều cạnh; exitOrder là postorder; parent không duy nhất.
- [`dijkstra`](../03_graphs/dijkstra.hpp): KHÔNG dùng được với cạnh âm (dùng bellman-ford); mọi khoảng cách hữu hạn cần < DIJ_INF;
- [`dinic`](../03_graphs/dinic.hpp): cap phải >= 0; cạnh vô hướng -> addEdge(u,v,c) rồi addEdge(v,u,c); s != t.
- [`dominator-tree`](../03_graphs/dominator-tree.hpp): dominance chỉ xét các đường bắt đầu tại source; phải gọi lại build sau khi thêm cạnh.
- [`edge-orientation`](../03_graphs/edge-orientation.hpp): even-outdegree tồn tại iff mỗi thành phần có số cạnh chẵn; output giữ đúng thứ tự edge input.
- [`eulerian-path`](../03_graphs/eulerian-path.hpp): đồ thị phải liên thông nếu bỏ các đỉnh cô lập và thỏa điều kiện bậc; cạnh vô hướng dùng edge-id.
- [`find-cycle`](../03_graphs/find-cycle.hpp): đỉnh 0-based (CSES 1-based → cộng 1 khi in); bản vô hướng bỏ cạnh cha theo ID CẠNH (mỗi cạnh 1 id, đẩy 2 chiều) — bỏ theo đỉnh cha sẽ mất chu trình do cạnh song song; kết quả rỗng nghĩa là KHÔNG có chu trình; self-loop cho ra {u, u}, cạnh song song cho ra {u, v, u}.
- [`floyd-warshall`](../03_graphs/floyd-warshall.hpp): bỏ qua trung gian k không tới (d\[i\]\[k\]==INF) để tránh tràn; dùng long long; giữ min khi nạp cạnh song song.
- [`havel-hakimi`](../03_graphs/havel-hakimi.hpp): bậc phải thuộc \[0,n-1\], tổng bậc chẵn; kết quả là một realization bất kỳ.
- [`hopcroft-karp`](../03_graphs/hopcroft-karp.hpp): chỉ dùng cho đồ thị HAI PHÍA; u thuộc trái, v thuộc phải; DFS đường tăng có thể sâu O(nl),
- [`hungarian`](../03_graphs/hungarian.hpp): cần n <= m (nếu n > m thì chuyển vị); tìm MIN (muốn MAX thì đặt cost = -giá trị); mọi ô phải được set.
- [`k-shortest-paths`](../03_graphs/k-shortest-paths.hpp): cạnh phải không âm; kết quả là WALK nên chu trình được phép; phép cộng bão hòa ở KSP_INF để tránh tràn.
- [`min-cost-max-flow`](../03_graphs/min-cost-max-flow.hpp): cho phép cost ÂM trên cạnh (SPFA) nhưng KHÔNG được có chu trình âm; dùng long long; s != t.
- [`mst`](../03_graphs/mst.hpp): dùng long long cho tổng; nếu đồ thị rời rạc thì tong là tổng của RỪNG (số cạnh < n-1).
- [`scc`](../03_graphs/scc.hpp): chỉ cho đồ thị CÓ HƯỚNG; bản LẶP nên không tràn stack; addEdge xong hết mới build().
- [`topological-sort`](../03_graphs/topological-sort.hpp): chỉ dùng cho đồ thị CÓ HƯỚNG; kết quả rỗng đồng nghĩa CÓ chu trình (không phải "không có đỉnh").
- [`two-sat`](../03_graphs/two-sat.hpp): solve() trả false nếu mâu thuẫn; addClause(i,fi,j,fj) nghĩa (x_i=fi HOẶC x_j=fj); dựng trước, solve sau.

### 04 — Cây

- [`binary-lifting`](../04_trees/binary-lifting.hpp): next phải nằm trong \[0,n); pathAggregate gộp CẠNH; op/id phải là monoid, thứ tự path vẫn được giữ.
- [`centroid-decomposition`](../04_trees/centroid-decomposition.hpp): cây không trọng số, đỉnh 0-based; CentroidNearest chỉ THÊM active (không xóa); phải là cây/liên thông.
- [`euler-tour`](../04_trees/euler-tour.hpp): đoạn nửa mở \[tin, tout); size(v) = tout\[v\]-tin\[v\]; u là tổ tiên v <=> tin\[u\] <= tin\[v\] < tout\[u\].
- [`hld`](../04_trees/hld.hpp): giá trị đặt trên ĐỈNH (path gồm cả hai đầu và LCA); dựng bằng DFS LẶP; đổi '+' trong segtree để làm max/min.
- [`lca-binary-lifting`](../04_trees/lca-binary-lifting.hpp): adj là cây VÔ HƯỚNG (n-1 cạnh); cha của gốc = chính gốc; kth trả -1 nếu k > depth\[v\].
- [`long-path-decomposition`](../04_trees/long-path-decomposition.hpp): adj phải là một CÂY vô hướng; đây là tối ưu chuyên biệt cho đúng MỘT độ dài k, không phải HLD.
- [`prufer`](../04_trees/prufer.hpp): nhãn 1-BASED như CSES và mọi phần tử code phải thuộc \[1, n\]; n <= 2 -> code rỗng, pruferDecode(rỗng) hiểu là n = 2 nên roundtrip n = 1 mất; pruferEncode giả định đầu vào ĐÚNG là cây (không tự kiểm tra).
- [`rerooting`](../04_trees/rerooting.hpp): đây là MẪU cho tổng khoảng cách; đổi công thức gộp (down/up) để giải bài rerooting khác; dùng long long.
- [`small-to-large`](../04_trees/small-to-large.hpp): đây là MẪU đếm màu phân biệt; đổi cấu trúc (set -> map tần suất...) cho bài khác; DFS LẶP (không tràn stack).
- [`tree-augmentation`](../04_trees/tree-augmentation.hpp): adj phải là một cây vô hướng đối xứng; n=2 trả một cạnh song song (đề phải cho phép multiedge).
- [`tree-diameter`](../04_trees/tree-diameter.hpp): chỉ đúng trên CÂY (liên thông, không chu trình) với trọng số KHÔNG âm; cây rỗng/n=1 -> len = 0.
- [`tree-dp`](../04_trees/tree-dp.hpp): đỉnh 0-based (CSES 1-based → trừ 1, hoặc truyền n+1 và để trống đỉnh 0); tổng khoảng cách phải long long (~n^2/2); cây đơn, không tự khuyên.
- [`tree-isomorphism`](../04_trees/tree-isomorphism.hpp): adj phải là danh sách kề của một cây vô hướng 0-based. ID chỉ được so sánh
- [`tree-reconstruction`](../04_trees/tree-reconstruction.hpp): nhãn phải PHÂN BIỆT; triển khai hoàn toàn lặp để an toàn với cây lệch n đỉnh.

### 05 — Quy hoạch động

- [`bitmask-dp`](../05_dynamic_programming/bitmask-dp.hpp): n <= ~20 (2^n bùng nổ); TSP cần dist\[i\]\[i\]=0; đếm đường Hamilton lấy theo MOD; dùng long long.
- [`broken-profile-dp`](../05_dynamic_programming/broken-profile-dp.hpp): chỉ CỘNG nên mod tùy ý <= ~4.6e18; 2^min(n,m) nổ nếu CẢ HAI chiều lớn; số ô trống lẻ luôn cho 0;
- [`coin-change`](../05_dynamic_programming/coin-change.hpp): THỨ TỰ vòng lặp: ordered = target NGOÀI, xu TRONG; unordered = xu NGOÀI, target TRONG; dùng long long + MOD.
- [`digit-dp`](../05_dynamic_programming/digit-dp.hpp): xử lý số 0 đứng đầu bằng cờ "started"; chỉ nhớ hóa khi KHÔNG tight; đổi điều kiện trong vòng để giải bài khác.
- [`edit-distance`](../05_dynamic_programming/edit-distance.hpp): chi phí mỗi phép = 1; hàng/cột 0 khởi tạo bằng i, j (chèn/xóa toàn bộ); phân biệt hoa/thường.
- [`grid-dp`](../05_dynamic_programming/grid-dp.hpp): lưới phải chữ nhật; đường đi chỉ xuống/phải; modulo >0; sizes trả ma trận nên tốn O(n*m) output.
- [`interval-dp`](../05_dynamic_programming/interval-dp.hpp): đây là MẪU cho trò lấy hai đầu; đổi công thức dp cho bài đoạn khác (gộp đá, đa giác...); dùng long long.
- [`knapsack`](../05_dynamic_programming/knapsack.hpp): weight phải >0, capacity >=0; giá trị/tích count*value và đáp án phải vừa long long.
- [`lcs`](../05_dynamic_programming/lcs.hpp): "dãy con" (subsequence) không phải "xâu con" (substring); có thể có nhiều LCS, hàm trả một cái.
- [`linear-dp`](../05_dynamic_programming/linear-dp.hpp): callback phải GHI CỘNG vào next đã được reset về T{} và không được resize; step là 0-based của PHÉP CHUYỂN, initial là lớp trước step 0.
- [`lis`](../05_dynamic_programming/lis.hpp): "ngặt" dùng lower_bound (thay phần tử bằng nhau); "không ngặt" dùng upper_bound; chỉ trả độ dài.
- [`slope-trick`](../05_dynamic_programming/slope-trick.hpp): chi phí tới ~2e14 nên PHẢI long long; chỉ trả CHI PHÍ (không dựng lại dãy tối ưu); minCostIncreasing giả định dãy đích NGUYÊN (bước >= 1) và cho phép a'_i âm.
- [`sos-dp`](../05_dynamic_programming/sos-dp.hpp): f.size() phải là lũy thừa 2 (= 2^n); dùng long long tránh tràn; "con" gồm chính mask và tập rỗng.
- [`subset-sum-trace`](../05_dynamic_programming/subset-sum-trace.hpp): values phải KHÔNG ÂM, mỗi chỉ số dùng tối đa một lần; tổng trong \[0,limit\]; index kết quả 0-based.
- [`weighted-interval-scheduling`](../05_dynamic_programming/weighted-interval-scheduling.hpp): khoảng ĐÓNG \[start,finish\]; nếu đề cho nửa mở phải đổi điều kiện; tổng profit phải vừa long long.

### 06 — Xâu

- [`aho-corasick`](../06_strings/aho-corasick.hpp): gọi addPattern hết rồi mới build (build biến trie thành automaton); mẫu trùng -> đếm riêng từng bản; chữ thường.
- [`bwt`](../06_strings/bwt.hpp): sentinel phải xuất hiện đúng một lần và không có sẵn trong s; mặc định '#'; kết quả transform luôn chứa sentinel.
- [`distinct-subsequences`](../06_strings/distinct-subsequences.hpp): đây là subsequence (không cần liên tiếp), không phải substring; mod phải dương; dùng __int128 khi mod lớn.
- [`eertree`](../06_strings/eertree.hpp): addChar trả độ dài palindrome DÀI NHẤT kết thúc tại ký tự vừa thêm; countAllPalindromes chỉ gọi MỘT lần (làm hỏng cnt).
- [`kmp-automaton`](../06_strings/kmp-automaton.hpp): trạng thái chạy 0..\|p\| nên bảng có \|p\| + 1 HÀNG; hàng \|p\| (vừa khớp trọn mẫu) vẫn có cạnh đi tiếp theo fail link nên đếm được lần khớp CHỒNG LẤN — muốn chặn thì tự bỏ trạng thái \|p\| như countStringsAvoiding làm;
- [`kmp`](../06_strings/kmp.hpp): pi\[i\] là biên THẬT SỰ (proper) dài nhất của s\[0..i\]; mẫu rỗng trả rỗng; phân biệt hoa/thường.
- [`manacher`](../06_strings/manacher.hpp): độ dài palindrome lẻ tâm i = 2*d1\[i\]-1, chẵn = 2*d2\[i\]; count đếm cả các palindrome độ dài 1; xâu rỗng -> 0.
- [`min-rotation`](../06_strings/min-rotation.hpp): trả về chỉ số (0-based), không phải xâu; xâu rỗng trả 0; so sánh theo mã ký tự (phân biệt hoa/thường).
- [`string-hashing`](../06_strings/string-hashing.hpp): đoạn nửa mở \[l, r); dùng HAI mod gộp 64-bit để tránh va chạm; base > mọi mã ký tự; không dùng cho input đối kháng cực đoan.
- [`suffix-array`](../06_strings/suffix-array.hpp): không thêm ký tự canh gác (dùng rank = -1 khi vượt biên); số xâu con phân biệt = n*(n+1)/2 - tổng lcp; xâu rỗng -> rỗng.
- [`suffix-automaton`](../06_strings/suffix-automaton.hpp): chỉ nhận 'a'..'z'; k là 1-based, trả "" nếu ngoài miền; gọi extend sau propagate vẫn an toàn nhưng phải truy vấn lại.
- [`z-function`](../06_strings/z-function.hpp): z\[0\] đặt = n; zSearch dùng ký tự ngăn cách '\\x01' (phải KHÔNG xuất hiện trong t, p); mẫu rỗng trả rỗng.

### 07 — Toán

- [`burnside`](../07_mathematics/burnside.hpp): mod phải NGUYÊN TỐ và gcd(n, mod) = 1 để có nghịch đảo của n (mod nguyên tố > n là đủ; countBracelets cần thêm mod lẻ);
- [`combinatorics`](../07_mathematics/combinatorics.hpp): mod PHẢI nguyên tố; dựng với n đủ lớn (>= mọi n truy vấn); C trả 0 khi r < 0 hoặc r > n.
- [`divisor-blocks`](../07_mathematics/divisor-blocks.hpp): i chạy 1-based, khối \[l, r\] là đoạn ĐÓNG và phủ kín \[1, n\] không chồng lấn; n <= 0 trả về 0 / rỗng.
- [`divisor-sieve`](../07_mathematics/divisor-sieve.hpp): chỉ nhận số dương <= M; exact đếm subset theo chỉ số phần tử (hai giá trị bằng nhau vẫn là hai lựa chọn); mod dương.
- [`eulerian-numbers`](../07_mathematics/eulerian-numbers.hpp): k là số descent 0-based (CSES hỏi lượt r thì dùng A(n,r-1)); A(0,0)=1, còn với n>0 chỉ k in \[0,n-1\].
- [`extended-euclid`](../07_mathematics/extended-euclid.hpp): invMod trả -1 nếu gcd(a, m) != 1; crt trả {-1, -1} nếu vô nghiệm; kết quả CRT trong \[0, lcm(m1, m2)).
- [`factorization`](../07_mathematics/factorization.hpp): n là unsigned long long (< 2^64); Miller-Rabin ở đây XÁC ĐỊNH (bộ cơ số cố định) cho mọi n 64-bit.
- [`gaussian-elimination`](../07_mathematics/gaussian-elimination.hpp): p nguyên tố; A là n hàng × m cột; biến tự do đặt = 0 trong nghiệm trả về; dùng __int128 khi nhân.
- [`inclusion-exclusion`](../07_mathematics/inclusion-exclusion.hpp): mọi phần tử của primes phải >= 1 (không âm, không 0); đoạn đếm là \[1, n\] ĐÓNG hai đầu nên n = 0 cho 0.
- [`integer-math`](../07_mathematics/integer-math.hpp): k của digitInPositiveIntegers là 1-based; valuation cần p nguyên tố; lcmCapped trả cap+1 nếu vượt.
- [`josephus`](../07_mathematics/josephus.hpp): MỌI nhãn/chỉ số ở đây là 1-based (người 1..n, q = 1..n); luật đếm là "loại người thứ k" nên người bị loại
- [`kth-permutation`](../07_mathematics/kth-permutation.hpp): k và hạng đều 0-BASED (hoán vị nhỏ nhất 0,1,...,n-1 có hạng 0); phần tử là 0..n-1 (0-based),
- [`labeled-combinatorics`](../07_mathematics/labeled-combinatorics.hpp): mod phải nguyên tố và mod>n; vector có n+1 phần tử, index k là số thành phần; n=0 trả answer\[0\]=1.
- [`lucas`](../07_mathematics/lucas.hpp): p PHẢI nguyên tố và đủ nhỏ để cấp bảng O(p) (p <= 1 trả 0); k < 0 hay k > n trả 0/false; bảng giai thừa là static nên nhóm truy vấn cùng p, đổi p liên tục sẽ dựng lại mỗi lần.
- [`matrix-exponentiation`](../07_mathematics/matrix-exponentiation.hpp): đặt mod trước khi nạp phần tử; identity theo mod (1 % mod); dùng __int128 khi nhân; e >= 0.
- [`mobius`](../07_mathematics/mobius.hpp): chỉ cho x <= n đã sàng; mu\[x\]=0 nếu x có thừa số bình phương; mu là hàm NHÂN TÍNH.
- [`modular-arithmetic`](../07_mathematics/modular-arithmetic.hpp): modInv dùng Fermat -> m PHẢI nguyên tố (nếu không, dùng extended-euclid); e >= 0; kết quả trong \[0, m).
- [`ntt`](../07_mathematics/ntt.hpp): exact chỉ nhận hệ số không âm, size NTT <=2²¹ và mỗi hệ số kết quả < tích hai modulus.
- [`probability-expectation`](../07_mathematics/probability-expectation.hpp): xác suất dùng long double; transition\[u\] phải có tổng xác suất 1; các walker phải độc lập; chỉ làm tròn khi in.
- [`semiring-matrix`](../07_mathematics/semiring-matrix.hpp): pow chỉ cho ma trận vuông, e >= 0; hai toán hạng phải dùng cùng semiring; INF phải lớn hơn mọi đáp án thật.
- [`sieve`](../07_mathematics/sieve.hpp): chỉ dùng cho x <= n đã sàng; 0 và 1 không nguyên tố; factorize dùng spf nên rất nhanh.
- [`sprague-grundy`](../07_mathematics/sprague-grundy.hpp): chỉ đúng cho trò IMPARTIAL; tổng ván = XOR các Grundy; vị trí THUA (P-position) <=> Grundy = 0.
- [`walsh-hadamard`](../07_mathematics/walsh-hadamard.hpp): xorConvolution pad hai mảng lên CÙNG luỹ thừa 2 nên độ dài kết quả là luỹ thừa 2 (không phải \|a\|+\|b\|-1);
- [`xor-basis`](../07_mathematics/xor-basis.hpp): mọi giá trị là unsigned long long (KHÔNG dùng số âm); tập giá trị XOR luôn chứa 0 (tập con rỗng) nên

### 08 — Hình học

- [`closest-pair`](../08_geometry/closest-pair.hpp): cần >= 2 điểm (trả LLONG_MAX nếu ít hơn); kết quả là d^2; tọa độ tới ~1e9 để d^2 không tràn long long.
- [`convex-hull`](../08_geometry/convex-hull.hpp): cross <= 0 để loại điểm thẳng hàng (bao chặt); đổi thành < 0 nếu muốn GIỮ điểm trên cạnh; điểm trùng đã khử.
- [`geometry-basics`](../08_geometry/geometry-basics.hpp): tọa độ lớn hơn ~1e9 phải dùng __int128; orient: >0 CCW (trái), <0 CW (phải), 0 thẳng hàng.
- [`line-intersection`](../08_geometry/line-intersection.hpp): đây là ĐƯỜNG THẲNG (vô hạn), không phải ĐOẠN; ok=false khi song song; tọa độ ~1e9 để định thức không tràn.
- [`manhattan-distance`](../08_geometry/manhattan-distance.hpp): x+y, x-y và đường kính phải vừa long long; tổng cặp trả __int128; mỗi cặp không thứ tự đếm một lần.
- [`point-in-polygon`](../08_geometry/point-in-polygon.hpp): đa giác phải ĐƠN (không tự cắt); dùng winding nên đúng cả đa giác LÕM; điểm trên biên trả 1 (ưu tiên).
- [`polygon-area`](../08_geometry/polygon-area.hpp): polygonArea2 trả 2A CÓ DẤU (lấy trị tuyệt đối để ra diện tích); Pick chỉ đúng với đỉnh NGUYÊN; đa giác không tự cắt.
- [`rotating-calipers`](../08_geometry/rotating-calipers.hpp): đỉnh đối xứng luôn nằm trên BAO LỒI nên chỉ cần kẹp trên bao; kết quả là d^2; tọa độ tới ~1e9.
- [`segment-intersection`](../08_geometry/segment-intersection.hpp): trả true cả khi chỉ CHẠM tại một đầu mút hoặc CHỒNG một phần; tọa độ tới ~1e9 để cross không tràn.

### 09 — Thuật toán offline

- [`cdq-divide-conquer`](../09_offline_algorithms/cdq-divide-conquer.hpp): chiều thời gian là chỉ số mảng (0-based); so sánh KHÔNG NGẶT (<=) ở cả x và y; y được nén nên giá trị tùy ý.
- [`mo-algorithm`](../09_offline_algorithms/mo-algorithm.hpp): đây là MẪU đếm phân biệt; đổi hàm add/rem để tính đại lượng khác; truy vấn \[l, r\] BAO GỒM cả hai đầu.
- [`offline-dynamic-connectivity`](../09_offline_algorithms/offline-dynamic-connectivity.hpp): cạnh tồn tại trong NỬA MỞ \[tl, tr); DSU KHÔNG nén đường (để rollback); thời gian t trong \[0, T).
- [`offline-lca`](../09_offline_algorithms/offline-lca.hpp): adj phải là CÂY liên thông chứa root; truy vấn (u, u) trả về u; đỉnh không tới được từ root -> đáp án -1.
- [`parallel-binary-search`](../09_offline_algorithms/parallel-binary-search.hpp): đây là MẪU (đổi điều kiện kiểm tra để giải bài khác); điều kiện phải ĐƠN ĐIỆU theo thời gian; ngày 1-based.
- [`sqrt-decomposition`](../09_offline_algorithms/sqrt-decomposition.hpp): đoạn ĐÓNG \[l, r\]; dùng long long cho tổng; khối biên xử lý từng phần, khối trọn dùng lazy.

### 10 — Kỹ thuật nâng cao

- [`bitset-reachability`](../10_advanced_techniques/bitset-reachability.hpp): chỉ đúng với DAG (có chu trình -> nén bằng scc.hpp rồi chạy trên condensation); bộ nhớ n^2/8 nên n phải nhỏ.
- [`constructive`](../10_advanced_techniques/constructive.hpp): mọi output ở đây là hoán vị 0-based trừ evenOdd trả 1-based; k nghịch thế phải trong \[0,n(n-1)/2\].
- [`divide-conquer-dp`](../10_advanced_techniques/divide-conquer-dp.hpp): cost PHẢI thỏa tính đơn điệu của opt (vd tổng bình phương, tổng khoảng cách tới trung vị); k <= n; trả 4e18 nếu vô nghiệm.
- [`functional-graph`](../10_advanced_techniques/functional-graph.hpp): nxt\[v\] là ĐÚNG MỘT đỉnh (0-based, cho phép nxt\[v\]==v); kth chỉ đúng khi k <= maxK; dist trả -1 nếu không tới.
- [`interactive-search`](../10_advanced_techniques/interactive-search.hpp): emitAndFlush PHẢI gửi trọn request và flush trước khi return; helper không tự làm I/O. Budget được trừ TRƯỚC callback (callback lỗi vẫn coi là đã dùng).
- [`k-best-combinations`](../10_advanced_techniques/k-best-combinations.hpp): m phải thuộc \[0,n\], k >= 0; kết quả có thể ngắn hơn k nếu k>C(n,m).
- [`k-best-enumeration`](../10_advanced_techniques/k-best-enumeration.hpp): các tổng bằng nhau được lặp theo các tập chỉ số khác nhau; có tối đa 2^n kết quả; tổng phải vừa long long.
- [`knuth-optimization`](../10_advanced_techniques/knuth-optimization.hpp): cost PHẢI thỏa bất đẳng thức tứ giác + đơn điệu theo đoạn, nếu không kết quả SAI; dp đoạn 1 phần tử = 0.
- [`kruskal-reconstruction-tree`](../10_advanced_techniques/kruskal-reconstruction-tree.hpp): edges PHẢI đưa vào theo thứ tự tăng của w; bottleneck(u,u)=0; khác thành phần trả -1; leaves\[node\] = số đỉnh gốc dưới node.
- [`meet-in-the-middle`](../10_advanced_techniques/meet-in-the-middle.hpp): n <= ~40; đếm CẢ tập rỗng (tổng 0); dùng long long cho tổng; giá trị có thể âm.
- [`multiset-reconstruction`](../10_advanced_techniques/multiset-reconstruction.hpp): yêu cầu n>=3; output được sort không giảm; minValue/maxValue là cận ĐÓNG;
- [`ternary-search`](../10_advanced_techniques/ternary-search.hpp): ternaryMin SAI IM LẶNG nếu f có đoạn bằng nhau -> dùng convexMin (cần LỒI THẬT); hàm chỉ "gần lồi" như ceil(A/x)+B*x thì CẢ HAI có thể sai, phải quét thêm lân cận (±10).

### 11 — Greedy (tham lam trên đoạn / lịch biểu)

- [`greedy-array`](../11_greedy/greedy-array.hpp): coins phải DƯƠNG; hàm ném overflow_error nếu đáp án không biểu diễn được bằng long long.
- [`heap-scheduling`](../11_greedy/heap-scheduling.hpp): MỌI đoạn ở đây là NỬA MỞ \[l, r) nên kề nhau (r1 == l2) KHÔNG chồng lấn; với ngày ĐÓNG \[a, b\] (chạm nhau là xung đột) phải truyền {a, b+1}.
- [`heap-selection`](../11_greedy/heap-selection.hpp): output top-k dài n+1 dùng đoạn NỬA MỞ \[0,i) / \[i,n); nullopt nghĩa là chưa đủ k phần tử, k=0 luôn có tổng 0.
- [`interval-scheduling`](../11_greedy/interval-scheduling.hpp): hai nhóm hàm dùng HAI quy ước khác nhau — (a) nửa mở \[l, r) nên r == l' KHÔNG tính là giao (CSES 1629),
- [`optimal-merge`](../11_greedy/optimal-merge.hpp): trọng số phải không âm; 0/1 phần tử có chi phí 0; n lớn cần long long và kiểm tràn.
- [`path-matching`](../11_greedy/path-matching.hpp): edgeCost\[i\] nối đỉnh i,i+1 (0-based), được phép âm; maxPairs=-1 nghĩa là mọi k;

<!-- GEN:END -->

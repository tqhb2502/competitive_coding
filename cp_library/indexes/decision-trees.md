# Cây quyết định chọn kỹ thuật

Đi từ trên xuống trong nhánh phù hợp, **dừng ở kỹ thuật đầu tiên thỏa mọi điều kiện của đề**.
Mỗi lá là một snippet (`id` = tên file `.hpp`). Bảng tra theo triệu chứng: [by-use-case.md](by-use-case.md);
ngân sách theo `n`: [by-complexity.md](by-complexity.md); bẫy: [pitfalls.md](pitfalls.md).

## 0. Hỏi trước 3 câu

1. `n` bao nhiêu → ngân sách ĐPT (xem [by-complexity.md](by-complexity.md)).
2. Có **truy vấn** không? Phải trả lời **online** (theo thứ tự) hay được **offline** (đọc hết rồi xử)?
3. Dữ liệu **tĩnh** hay **thay đổi** (cập nhật điểm / cập nhật đoạn / thêm-xóa cạnh)?

## 1. Mảng & truy vấn đoạn

```text
Mảng tĩnh, không cập nhật?
├─ tổng đoạn .................................... prefix-sum
├─ XOR đoạn ..................................... prefix-xor
├─ min/max/gcd, nhiều truy vấn .................. sparse-table (idempotent) | disjoint-sparse-table (phép bất kỳ)
├─ nhỏ thứ k trong đoạn ......................... persistent-segment-tree | wavelet-matrix
└─ đếm phần tử < x trong đoạn ................... wavelet-matrix | mo-algorithm (offline)
Có cập nhật?
├─ cập nhật ĐIỂM + tổng đoạn .................... fenwick
├─ cập nhật ĐIỂM + gộp monoid (max/min/gcd) ..... segment-tree
├─ cập nhật ĐOẠN + truy vấn đoạn ................ lazy-segment-tree | sqrt-decomposition (ngắn hơn)
├─ cộng đoạn, chỉ đọc kết quả CUỐI .............. difference-array
├─ miền chỉ số tới 1e9 .......................... dynamic-segment-tree (hoặc coordinate-compression rồi fenwick)
└─ lưới 2D, cập nhật điểm + tổng chữ nhật ....... fenwick-2d
Cần đảo đoạn / chèn / xóa giữa dãy? ............. implicit-treap
Cần nhỏ thứ k trên tập ĐỘNG? .................... ordered-set (GCC) | treap
Median + tổng |x-median| trên tập động? ......... median-maintenance
Đổi một ký tự, hỏi run bằng nhau dài nhất? ...... run-intervals
Hợp đoạn nguyên động, cần thêm/xóa/tìm điểm phủ? . interval-set
Nhiều priority queue cần meld nhanh? ............. leftist-heap
Cửa sổ động cần mode + tie theo giá trị? ......... window-mode
```

## 2. Mảng: đi một lượt / tìm biên

```text
Điều kiện đơn điệu theo độ dài đoạn? ............ sliding-window
Hai chỉ số chạy một chiều trên mảng đã sắp? ..... two-pointers
"Phần tử gần nhất lớn/nhỏ hơn"? ................. monotonic-stack
Min/max mọi cửa sổ độ dài k? .................... monotonic-deque
"Giá trị nhỏ nhất còn thỏa"? .................... binary-search
Giá trị lớn nhưng chỉ cần thứ hạng? ............. coordinate-compression
Đoạn con LIỀN KỀ có tổng lớn nhất? .............. max-subarray
Sinh dãy mã Gray / số ↔ mã Gray? ................ gray-code
Đếm tần suất / RLE / mex? ....................... frequency-counting
Popcount, Hamming, đóng gói hoặc đếm bit? ....... bit-operations
Bitset có độ dài runtime / dp |= dp<<x? .......... dynamic-bitset
Hoán vị: chu trình / nghịch đảo / bậc? .......... permutation-cycles
Hoán vị ↔ mã nghịch thế/Lehmer? ................. permutation-code
Sắp xếp mà phép so sánh là oracle đắt? .......... comparison-sort
Nhiều OPEN/QUERY/CLOSE theo một trục? ........... sweep-line
Cần XOR lớn nhất / theo bit?
├─ XOR của một PHẦN TỬ với truy vấn .............. binary-trie
└─ XOR của một TẬP CON (chọn tùy ý) ............. xor-basis
Duyệt tập con của mask? ......................... submask-enumeration → nếu cần tổng trên mọi tập con: sos-dp
```

## 3. Đồ thị

```text
Đường đi ngắn nhất?
├─ cạnh đều nhau ................................ bfs
├─ cạnh 0/1 ..................................... bfs-01
├─ trọng số >= 0 ................................ dijkstra
├─ có cạnh âm / phát hiện chu trình âm .......... bellman-ford
└─ mọi cặp, n <= ~500 ........................... floyd-warshall
Cấu trúc liên thông?
├─ có hướng: nén chu trình ...................... scc  (rồi DAG: topological-sort)
├─ vô hướng: cạnh/đỉnh sống còn ................. bridges-articulation
├─ chỉ thêm cạnh, hỏi "cùng nhóm?" .............. dsu
├─ cần hoàn tác hợp nhất ........................ rollback-dsu
├─ cạnh xuất hiện & biến mất theo thời gian ..... offline-dynamic-connectivity
├─ "ngày sớm nhất liên thông" / bottleneck ...... kruskal-reconstruction-tree | parallel-binary-search
├─ ràng buộc hiệu thế giữa các đỉnh ............. weighted-dsu
└─ đếm đỉnh tới được trên DAG ................... bitset-reachability
Ràng buộc logic 2 lựa chọn? ..................... two-sat
Cây khung nhỏ nhất? ............................. mst
Đi hết mọi cạnh một lần? ........................ eulerian-path
Mỗi đỉnh đúng 1 cạnh ra? ........................ functional-graph
K đường/walk ngắn nhất? ......................... k-shortest-paths
Flood fill / rừng DFS sâu? ...................... dfs
Cần đỉnh nằm trên MỌI đường từ nguồn? ........... dominator-tree
Truy vấn tránh một đỉnh / vertex-BCC? ........... block-cut-tree
Hướng cạnh thành DAG / mọi outdegree chẵn? ...... edge-orientation
Dựng đồ thị đơn từ dãy bậc? ..................... havel-hakimi
Luồng / ghép cặp?
├─ luồng cực đại, lát cắt nhỏ nhất .............. dinic
├─ luồng + chi phí .............................. min-cost-max-flow
├─ ghép hai phía cực đại ........................ hopcroft-karp
└─ gán n-n chi phí nhỏ nhất ..................... hungarian
Thứ tự phụ thuộc / DP trên DAG? ................. topological-sort
Cần IN RA một chu trình (không chỉ có/không)? ... find-cycle
```

## 4. Cây

```text
Nhảy k bước trên hàm next / gộp monoid cạnh? ...... binary-lifting
Truy vấn tổ tiên / khoảng cách?
├─ online, nhiều truy vấn ....................... lca-binary-lifting
└─ biết trước mọi truy vấn ...................... offline-lca
Truy vấn theo CÂY CON? .......................... euler-tour (+ fenwick / segment-tree)
Truy vấn/cập nhật theo ĐƯỜNG ĐI? ................ hld
Cần đáp án cho mọi đỉnh làm gốc? ................ rerooting
Đường đi dài nhất? .............................. tree-diameter
Đếm cặp đỉnh theo khoảng cách?
├─ đúng một độ dài k, cần O(n) ................... long-path-decomposition
└─ ràng buộc khoảng cách tổng quát ............... centroid-decomposition
Tổng hợp tập giá trị của mọi cây con? ........... small-to-large
Ghép cặp cực đại trên cây / DP lên-xuống? ....... tree-dp
Cây có nhãn ↔ dãy số, đếm/sinh cây? ............. prufer
Thêm ít cạnh nhất để cây không còn cầu? ......... tree-augmentation
Hai cây có cùng hình dạng khi bỏ nhãn? .......... tree-isomorphism
Biết preorder + inorder, cần cây/postorder? ...... tree-reconstruction
```

## 5. Quy hoạch động

```text
Trạng thái là PREFIX của mảng?
├─ dãy con tăng dài nhất ........................ lis
├─ chọn theo sức chứa, 0/1 hoặc bounded .......... knapsack
├─ đổi tiền (dùng lại vô hạn) ................... coin-change
├─ lớp i chỉ phụ thuộc lớp i-1 .................. linear-dp
└─ chia đúng k đoạn, chi phí đoạn "đẹp" ......... divide-conquer-dp
Trạng thái là ĐOẠN [i, j]?
├─ game lấy hai đầu / gộp đoạn .................. interval-dp
└─ chi phí thỏa bất đẳng thức tứ giác ........... knuth-optimization
Trạng thái là TẬP CON (n <= 20)? ................ bitmask-dp  (tổng trên tập con: sos-dp)
Trạng thái là hai xâu? .......................... edit-distance | lcs
Trạng thái là CHỮ SỐ của số? .................... digit-dp
Chuyển trạng thái dạng min(a·x + b)? ............ convex-hull-trick (slope đơn điệu) | li-chao-tree (bất kỳ)
n <= 40 mà 2^n quá lớn? ......................... meet-in-the-middle
Cực tiểu hàm lồi trên miền số? .................. ternary-search
Sửa dãy thành KHÔNG GIẢM, chi phí |chênh lệch|? . slope-trick
Lấp bảng bằng domino / mặt nạ theo cột? ......... broken-profile-dp
Ô phụ thuộc trên/trái/chéo? ..................... grid-dp
Subset-sum cần dựng lại tập chỉ số? ............. subset-sum-trace
Chọn khoảng có profit, không giao nhau? .......... weighted-interval-scheduling
```

## 6. Xâu

```text
Tìm mẫu trong văn bản?
├─ một mẫu ...................................... kmp | z-function
├─ nhiều mẫu cùng lúc ........................... aho-corasick
└─ so nhanh hai đoạn bất kỳ ..................... string-hashing
Tập từ động, hỏi/đếm theo tiền tố? .............. trie
Palindrome?
├─ dài nhất / đếm tất cả ........................ manacher
└─ đếm palindrome PHÂN BIỆT ..................... eertree
Xâu con phân biệt / lặp lại?
├─ cần thứ tự từ điển hậu tố .................... suffix-array
└─ đếm/vị trí/k-th substring .................... suffix-automaton
Xâu tuần hoàn, cần dạng chuẩn? .................. min-rotation
Đếm XÂU độ dài n chứa / tránh một mẫu? .......... kmp-automaton
Cột cuối các rotation / LF mapping? ............. bwt
Đếm subsequence phân biệt (không liên tiếp)? .... distinct-subsequences
```

## 7. Toán

```text
Lũy thừa / nghịch đảo theo mod?
├─ mod NGUYÊN TỐ ................................ modular-arithmetic
└─ mod bất kỳ / hệ đồng dư ...................... extended-euclid (invMod, crt)
Tổ hợp C(n, r) mod nguyên tố, nhiều truy vấn? ... combinatorics
Tính toán số nguyên phải tránh floating point? .. integer-math
Nguyên tố / phân tích thừa số?
├─ mọi số tới ~1e6 .............................. sieve
├─ một số tới ~1e18 ............................. factorization
└─ hàm nhân tính (mu, phi), đếm nguyên tố cùng nhau . mobius
Đếm phần tử chia hết / subset theo gcd? ......... divisor-sieve
Hồi quy tuyến tính / đếm đường độ dài k lớn? .... matrix-exponentiation
Ma trận min-plus / phép cộng-nhân tổng quát? ..... semiring-matrix
Hệ phương trình tuyến tính theo mod? ............ gaussian-elimination
Nhân đa thức / tích chập?
├─ chập THƯỜNG (mod hoặc số nguyên bằng CRT) .... ntt
└─ chập XOR (i ^ j = k) ......................... walsh-hadamard
Bài toán về XOR của TẬP CON (max / thứ k / đếm)? . xor-basis
C(n, k) mod p mà n tới 1e18, p nhỏ? ............. lucas
Tổng dạng Σ f(i)·⌊n/i⌋, n tới 1e12? ............. divisor-blocks
Đếm "ít nhất một" / "không phần tử nào đúng chỗ"? inclusion-exclusion
Đếm cấu hình mà QUAY được coi là giống nhau? .... burnside
Vòng tròn, cứ k người loại một? ................. josephus
Hoán vị thứ k / hạng của hoán vị? ............... kth-permutation
Trò chơi hai người, ai thắng? ................... sprague-grundy
Xác suất/kỳ vọng của biến rời rạc? .............. probability-expectation
Phân phối thành phần cấu trúc gắn nhãn? ......... labeled-combinatorics
Đếm hoán vị theo số descent/round? .............. eulerian-numbers
```

## 8. Hình học (tọa độ nguyên, tránh số thực)

```text
Cần cross / orient / điểm trên đoạn? ............ geometry-basics
Bao lồi? ........................................ convex-hull
Diện tích đa giác / điểm nguyên trong đa giác? .. polygon-area (shoelace + Pick)
Điểm có trong đa giác? .......................... point-in-polygon
Hai ĐOẠN có cắt nhau? ........................... segment-intersection
Giao điểm hai ĐƯỜNG THẲNG? ...................... line-intersection
Cặp điểm gần nhất? .............................. closest-pair
Cặp điểm xa nhất (đường kính)? .................. rotating-calipers
Khoảng cách Manhattan online / tổng mọi cặp? .... manhattan-distance
```

## 9. Tham lam trên đoạn / lịch biểu

```text
Chọn TỐI ĐA hoạt động không trùng giờ? .......... interval-scheduling (maxNonOverlapping, nửa mở)
Chia mọi đoạn vào ÍT PHÒNG nhất (+ gán phòng)? .. interval-scheduling (minRooms / minRoomsAssign, đoạn đóng)
k người/máy làm được tối đa bao nhiêu việc? ..... heap-scheduling (maxMoviesKPeople)
Xếp thứ tự việc, tối thiểu tổng thời gian chờ? .. heap-scheduling (maxTasksProfit, SPT)
Invariant tham lam cơ bản trên mảng? ............ greedy-array
Luôn ghép hai trọng số nhỏ nhất? ................ optimal-merge
Giữ top-k ở mọi prefix/suffix? .................. heap-selection
Min-cost matching đúng k cạnh trên path? ........ path-matching
```

## 10. Khi truy vấn được XỬ LÝ OFFLINE

```text
Truy vấn đoạn, thêm/bớt 1 phần tử là rẻ? ........ mo-algorithm
Cần range-update + range-query, code ngắn? ...... sqrt-decomposition
Cạnh sống trong khoảng thời gian? ............... offline-dynamic-connectivity
Mỗi truy vấn tìm "mốc thời gian nhỏ nhất"? ...... parallel-binary-search
Đếm bộ ba (thời gian, x, y) đơn điệu? ........... cdq-divide-conquer
Mọi truy vấn LCA biết trước? .................... offline-lca
```

## 11. Liệt kê ít đáp án, dựng nghiệm & interactive

```text
Chỉ cần k tổng subset nhỏ nhất?
├─ subset có kích thước bất kỳ .................. k-best-enumeration
└─ subset có đúng m phần tử ..................... k-best-combinations
Nhiều tập con nhưng n <= ~40? ................... meet-in-the-middle
Dựng lại dãy từ multiset tổng mọi cặp? .......... multiset-reconstruction
Đề chấp nhận bất kỳ cấu hình thỏa invariant? .... constructive
Mô tả trực tiếp state kế tiếp / cần dò chu kỳ? .. simulation
Không gian nghiệm nhỏ, chọn rồi hoàn tác? ....... backtracking
Bài interactive: budget + mock I/O + bit query? . interactive-search
```

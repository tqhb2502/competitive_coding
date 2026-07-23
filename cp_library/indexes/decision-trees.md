# Cây quyết định chọn thuật toán

Các cây dưới đây giúp thu hẹp lựa chọn, không thay thế việc chứng minh. Đi từ
trên xuống, dừng tại kỹ thuật đầu tiên thỏa toàn bộ điều kiện của đề.

## Quy trình chung

1. **Có thể mô hình thành công thức trực tiếp không?**
   - Có: chứng minh công thức và kiểm tra overflow.
   - Không: tiếp tục.
2. **`n` đủ nhỏ để duyệt trạng thái/tập con không?**
   - `n <= 20`: thử bitmask DP hoặc brute force có pruning.
   - `n <= 40`: thử meet-in-the-middle.
3. **Đáp án có tính đơn điệu theo một giá trị thử không?**
   - Có: binary search đáp án + hàm kiểm tra.
4. **Các quyết định có optimal substructure và state gọn không?**
   - Có: dynamic programming.
5. **Exchange argument cho phép chốt một lựa chọn cục bộ không?**
   - Có: greedy, thường sau sorting.
6. **Đề là nhiều query trên cùng dữ liệu?**
   - Online: chọn cấu trúc dữ liệu.
   - Offline: cân nhắc sweep, Mo, CDQ, rollback.

Nếu nhiều nhánh cùng hợp lệ, so sánh độ phức tạp, độ khó cài đặt và rủi ro WA;
không mặc định chọn kỹ thuật mạnh nhất.

## Truy vấn trên mảng

```text
Dữ liệu có cập nhật?
├─ Không
│  ├─ Phép toán có inverse? ──> Prefix sum
│  ├─ Min/max/gcd idempotent? ──> Sparse table
│  └─ Phép kết hợp associative bất kỳ? ──> Segment tree / disjoint sparse table
└─ Có
   ├─ Chỉ cập nhật điểm + prefix/đoạn cộng được? ──> Fenwick
   ├─ Cập nhật điểm + phép kết hợp tổng quát? ──> Segment tree
   ├─ Cập nhật đoạn + query điểm, toàn bộ offline? ──> Difference array
   └─ Cập nhật đoạn + query đoạn online? ──> Lazy segment tree
```

Sau đó hỏi thêm:

- Có cần phiên bản lịch sử? Persistent structure hoặc xử lý offline.
- Có cần phần tử thứ `k`? Fenwick/segment tree trên tần suất hoặc wavelet tree.
- Query được sắp lại? Mo/sweep/CDQ có thể đơn giản hơn.

## Đường đi ngắn nhất

```text
Đồ thị là DAG?
├─ Có ──> Topological DP, cho phép cạnh âm
└─ Không
   Trọng số mọi cạnh bằng nhau?
   ├─ Có ──> BFS
   └─ Không
      Trọng số chỉ 0/1?
      ├─ Có ──> 0-1 BFS
      └─ Không
         Có cạnh âm?
         ├─ Không ──> Dijkstra
         └─ Có
            Cần một nguồn? ──> Bellman–Ford
            Cần mọi cặp và n nhỏ? ──> Floyd–Warshall
```

Nếu cần đường đi dài nhất, kiểm tra chu trình trước. Trên DAG dùng DP; trên đồ
thị tổng quát, bài có thể không hữu hạn hoặc NP-hard.

## Connectivity động

1. Chỉ thêm cạnh?
   - Online: DSU.
2. Có xóa cạnh?
   - Query đã biết trước và chỉ xóa: đảo thời gian, biến xóa thành thêm.
   - Thêm/xóa xen kẽ offline: segment tree theo thời gian + rollback DSU.
   - Online thật sự: cần cấu trúc dynamic connectivity nâng cao.
3. Cần biết bridge/SCC thay vì connected component?
   - Static: low-link/SCC.
   - Dynamic: không thể thay trực tiếp bằng DSU thường.

## Bài trên cây

```text
Query chỉ trên subtree?
└─ Euler tour -> bài toán đoạn

Query trên đường u-v?
├─ Chỉ LCA/khoảng cách? -> Binary lifting LCA
├─ Kết hợp được và có update? -> HLD + segment tree
└─ Không update, phép phù hợp? -> Prefix từ gốc / lifting có aggregate

Cần đáp án cho mọi cách chọn gốc?
└─ Rerooting DP

Cần query theo khoảng cách tới tập đỉnh động?
└─ Centroid decomposition
```

Luôn xử lý forest hoặc xác nhận input liên thông trước khi gọi root traversal.

## Chọn dạng DP

1. State có một prefix và vài thuộc tính nhỏ: DP tuyến tính.
2. Quyết định chọn/bỏ, tổng nhỏ: knapsack/bitset.
3. State là `[left, right]`: interval DP.
4. State là subset: bitmask DP.
5. Một chiều input rất nhỏ: profile DP.
6. Input là chữ số của số rất lớn: digit DP.
7. Input là cây: tree DP; nếu cần mọi gốc, reroot.

Nếu transition duyệt mọi `j < i`, kiểm tra theo thứ tự:

- prefix min/max hoặc monotonic deque;
- Fenwick/segment tree sau nén tọa độ;
- convex hull trick/Li Chao nếu transition tuyến tính;
- divide-and-conquer optimization nếu argmin đơn điệu;
- Knuth optimization nếu thỏa quadrangle inequality.

Không áp dụng tối ưu chỉ vì công thức “trông giống”; điều kiện đơn điệu phải
được chứng minh.

## Chuỗi

```text
Một pattern trong text? -> KMP / Z
Nhiều pattern? -> Aho–Corasick
Palindrome quanh mọi tâm? -> Manacher
Prefix/suffix/period? -> Prefix function / Z
Nhiều query so sánh substring?
├─ Cần xác định tuyệt đối -> Suffix array + LCP / suffix structure
└─ Chấp nhận xác suất -> Double rolling hash
Đếm substring khác nhau? -> Suffix automaton hoặc suffix array
```

Nếu alphabet lớn, nén ký tự hoặc dùng map có cân nhắc hằng số; đừng giả định
`'a'..'z'` khi đề không nói.

## Online hay offline

Hỏi lần lượt:

1. Query sau có phụ thuộc output query trước không?
2. Có bắt buộc in ngay không?
3. Toàn bộ update/query có được đọc trước không?
4. Đổi thứ tự có làm thao tác add/remove rẻ hơn không?
5. Có thể binary search đáp án cho mọi query song song không?

Nếu ba câu đầu cho phép, xem:

- sort query theo một đầu mút + Fenwick;
- sweep line theo tọa độ hoặc thời gian;
- Mo nếu cửa sổ thay đổi cục bộ;
- CDQ cho dominance nhiều chiều;
- parallel binary search;
- rollback structure cho segment tree theo thời gian.

## Số học và modulo

1. Modulo là số nguyên tố?
   - Có: Fermat/inverse factorial có thể dùng khi mẫu khác 0 modulo.
   - Không: dùng Euclid mở rộng hoặc phân tích prime power.
2. Có nhiều truy vấn ước/bội tới giới hạn `M`?
   - Sieve/precompute theo `M`.
3. Có recurrence với chỉ số rất lớn?
   - Matrix exponentiation hoặc linear recurrence.
4. Có convolution?
   - Nhỏ: `O(nm)`.
   - Lớn, modulo phù hợp: NTT.
   - Modulo tùy ý: nhiều NTT + CRT hoặc FFT có kiểm soát sai số.

## Hình học

1. Tọa độ nguyên và chỉ cần orientation/giao/diện tích?
   - Dùng cross product nguyên, thường với `__int128`.
2. Cần thứ tự góc?
   - Chia nửa mặt phẳng + cross; tránh chỉ dùng `atan2` để sort chính xác.
3. Có sweep event cùng tọa độ?
   - Định nghĩa rõ thứ tự add/query/remove.
4. Bắt buộc dùng số thực?
   - Dùng `long double`, so sánh với epsilon có ý nghĩa theo scale, không dùng
     epsilon tùy tiện cho quan hệ thứ tự trong comparator.

Sau khi chọn thuật toán, mở [pitfalls.md](pitfalls.md) và kiểm checklist của
đúng nhóm trước khi code.

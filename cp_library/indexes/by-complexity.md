# Chỉ mục theo độ phức tạp

Ràng buộc chỉ cho biết miền thuật toán có thể chạy; hằng số, cache, số test và
giới hạn thời gian vẫn quyết định kết quả thực tế. Bảng dưới đây là ước lượng
thận trọng cho C++17 trong khoảng 1–2 giây, không phải cam kết.

## Ước lượng nhanh từ `n`

| Quy mô điển hình | Độ phức tạp nên nghĩ tới | Ví dụ |
|---:|---|---|
| `n <= 20` | `O(2^n * poly(n))` | Bitmask DP, duyệt tập con |
| `n <= 40` | `O(2^(n/2) * poly(n))` | Meet-in-the-middle |
| `n <= 200` | `O(n^3)` | Floyd–Warshall, interval DP nhỏ |
| `n <= 3000` | `O(n^2)` với hằng số gọn | DP hai chiều, so sánh mọi cặp |
| `n <= 2e5` | `O(n log n)` | Sort, Fenwick, segment tree |
| `n <= 1e6` | `O(n)` hoặc `O(n log log n)` | Two pointers, sieve |
| `n` tới `1e9` | `O(log n)` hoặc công thức | Binary lifting, Euclid |

Nếu có hai tham số, thay `n` bằng tích thật sự. `O(nm)` với `n = m = 2e5`
không phải tuyến tính.

## Theo lớp độ phức tạp

### `O(1)` và `O(log n)`

Phù hợp cho nhiều query hoặc tọa độ rất lớn:

- công thức, gcd, modular exponentiation;
- binary search trên miền số;
- truy vấn LCA/binary lifting sau tiền xử lý;
- thao tác trên Fenwick/segment tree.

Kiểm tra overflow trong phép nhân trước khi chia hoặc lấy modulo.

### `O(n)` và `O(n + m)`

Mục tiêu thường gặp khi input đã chiếm phần lớn thời gian:

- prefix/difference, two pointers;
- BFS/DFS;
- monotonic stack/deque;
- KMP, Z-function, Manacher;
- DSU gần tuyến tính.

“Mỗi phần tử bị thêm và xóa tối đa một lần” là dạng chứng minh amortized phổ
biến cho nhóm này.

### `O(n log n)`

Mốc an toàn phổ biến cho `n` khoảng `2e5`:

- sorting;
- Fenwick/segment tree;
- Dijkstra với binary heap;
- divide-and-conquer;
- suffix array ở một số cài đặt.

Một vòng `O(log n)` có allocation, hash hoặc tree node nặng có thể chậm hơn đáng
kể so với mảng liên tục.

### `O(n sqrt(n))` và `O((n + q) sqrt(n))`

Thường xuất hiện ở:

- sqrt decomposition;
- Mo's algorithm;
- chia block theo giá trị nhỏ/lớn;
- bitset/block optimization.

Tính block size từ số update/query và chi phí add/remove thật, không mặc định
luôn dùng `sqrt(n)`.

### `O(n^2)`

Khả thi tới vài nghìn nếu:

- bộ nhớ không vô tình thành nhiều mảng `long long` lớn;
- vòng trong không có `map`, allocation hoặc modulo đắt;
- truy cập dữ liệu liên tục và cache-friendly.

Với `n = 3000`, một bảng `int` chiếm khoảng 36 MB; bốn bảng như vậy đã là
144 MB.

### `O(n^3)`

Thường chỉ phù hợp cho vài trăm:

- Floyd–Warshall;
- Gaussian elimination;
- interval DP có thêm điểm chia.

Thứ tự vòng lặp và layout bộ nhớ có thể tạo chênh lệch lớn.

### Hàm mũ và giai thừa

| Độ phức tạp | Quy mô tham khảo |
|---|---:|
| `O(2^n)` | `n` khoảng 20–25 |
| `O(3^n)` | `n` khoảng 15–17 |
| `O(n 2^n)` | `n` khoảng 20 |
| `O(2^(n/2))` | `n` khoảng 40–46 |
| `O(n!)` | `n` khoảng 10–11 |

Pruning có thể giúp test thường nhưng không thay đổi cận xấu nếu đề có input
đối kháng.

## Tiền xử lý và thao tác

Không gộp mọi thứ thành một biểu thức mơ hồ. Với cấu trúc dữ liệu, metadata nên
ghi riêng:

```json
"complexities": {
  "build": "O(n)",
  "update": "O(log n)",
  "query": "O(log n)",
  "memory": "O(n)"
}
```

Với `q` query, tổng thời gian là `build + q * query` cộng chi phí update. Nếu có
nhiều test, cộng trên tổng kích thước input chứ không dùng riêng giá trị lớn
nhất của một test.

## Worst-case, amortized và expected

- **Worst-case**: bảo đảm cho mọi chuỗi thao tác hợp lệ.
- **Amortized**: một thao tác có thể đắt, nhưng tổng của cả chuỗi bị chặn.
- **Expected**: phụ thuộc random hoặc giả thiết phân phối; không đồng nghĩa với
  worst-case.

Ghi đúng từ khóa trong `complexities`, ví dụ
`"update": "O(1) amortized"` hoặc `"query": "O(1) expected"`.

## Ước lượng bộ nhớ

Các kích thước thường dùng:

| Kiểu | Số byte thường gặp |
|---|---:|
| `char`, `uint8_t` | 1 |
| `int`, `uint32_t` | 4 |
| `long long`, `uint64_t` | 8 |
| `pair<int,int>` | thường 8 |

Ngoài payload còn có overhead của `vector`, node pointer, allocator, recursion
stack và capacity chưa dùng. `vector<vector<int>>` không tương đương một mảng
liên tục `n * m`.

## Tìm snippet theo complexity

Độ phức tạp nằm ở dòng `ĐPT:` trong comment đầu mỗi `.hpp`:

```sh
rg -n 'ĐPT:.*O\(log n\)' cp_library
rg -n 'ĐPT:' cp_library/02_data_structures
```

Sau khi chọn được snippet, đọc comment header để biết `n` trong biểu thức là số
đỉnh, số cạnh, độ dài chuỗi hay kích thước miền giá trị.

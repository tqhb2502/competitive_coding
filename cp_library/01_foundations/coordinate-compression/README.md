# Coordinate Compression — `coordinate-compression`

Coordinate compression thay mỗi giá trị bằng rank 0-based của nó trong tập giá
trị phân biệt đã sắp xếp. Rank nằm trong một đoạn liên tiếp nhỏ nhưng vẫn bảo
toàn quan hệ thứ tự.

## Khi nào dùng

Dùng kỹ thuật này khi:

- giá trị hoặc tọa độ có miền rất lớn nhưng chỉ xuất hiện tối đa `n` giá trị;
- Fenwick tree, segment tree, mảng đếm hoặc DP cần chỉ số liên tiếp;
- chỉ phép so sánh thứ tự và quan hệ bằng nhau quan trọng, không cần giữ khoảng
  cách số học thật giữa hai tọa độ.

Không dùng rank như giá trị gốc trong công thức phụ thuộc khoảng cách. Chẳng hạn
`x[j] - x[i]` không bằng `rank[j] - rank[i]`. Với sweep line cần đại diện cả
khoảng trống hoặc đoạn nguyên, có thể phải thêm endpoint lân cận trước khi nén.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
cp_library::CoordinateCompression<long long> compression(values);

std::size_t rank = compression.indexOf(value);
const long long& original = compression.valueAt(rank);
std::vector<std::size_t> ranks = compression.compress(values);

std::size_t first_not_less = compression.lowerBoundIndex(value);
std::size_t first_greater = compression.upperBoundIndex(value);
bool known = compression.contains(value);
```

- Rank luôn 0-based trong `[0, size())`.
- Constructor nhận vector bằng giá trị, nên không sửa vector của người gọi.
  `assign` thay toàn bộ tập đã đăng ký; input rỗng được hỗ trợ.
- Giá trị trùng chỉ có một rank.
- `indexOf` và `compress` chỉ chấp nhận giá trị đã đăng ký, nếu không sẽ ném
  `std::invalid_argument`.
- `valueAt` ném `std::out_of_range` nếu rank không hợp lệ.
- `values()` trả về tham chiếu chỉ đọc tới các đại diện đã sắp xếp. Tham chiếu
  này và tham chiếu từ `valueAt` bị invalidated sau `assign` hoặc khi object bị
  hủy.
- Có thể truyền comparator tùy chỉnh. Comparator phải thỏa strict weak ordering
  và gọi được trên object `const` (`operator() const`); mọi khái niệm “bằng”,
  “trước” và “sau” của API đều theo comparator đó.

## Ý tưởng

Sao chép tất cả giá trị, sắp xếp chúng bằng `Compare`, rồi xóa các phần tử tương
đương kề nhau. Hai giá trị `a`, `b` tương đương khi:

```text
!compare(a, b) && !compare(b, a)
```

Vector còn lại `values_` chứa mỗi lớp tương đương đúng một lần. Rank của một giá
trị là vị trí `lower_bound` của nó trong vector này. `upperBoundIndex` trả vị trí
ngay sau lớp tương đương, hữu ích khi đếm số tọa độ `<= x`.

## Vì sao đúng

Sau `sort`, mọi phần tử đứng theo thứ tự của `Compare`, và các phần tử tương
đương nằm liền nhau. `unique` giữ đúng một đại diện của từng nhóm liền nhau, nên
`values_` vừa có thứ tự vừa không có hai phần tử tương đương.

Vì mỗi lớp tương đương xuất hiện đúng một lần, vị trí của nó là một rank duy
nhất trong `[0, size())`. `lower_bound` tìm phần tử đầu tiên không đứng trước
giá trị cần tra. Nếu phần tử này tương đương với giá trị, đó chính là đại diện
duy nhất; nếu không, giá trị chưa được đăng ký. Vì các rank xuất hiện theo cùng
thứ tự với `values_`, thay giá trị bằng rank bảo toàn mọi so sánh thứ tự.

`valueAt(indexOf(x))` trả đại diện tương đương với `x`, nên phép nén và khôi phục
theo lớp tương đương là nghịch đảo của nhau.

## Độ phức tạp

Gọi `n` là số phần tử đầu vào và `m` là số giá trị phân biệt.

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Khởi tạo / `assign` | `O(n log n)` | `O(n)` |
| `indexOf`, `contains` | `O(log m)` | `O(1)` |
| `lowerBoundIndex`, `upperBoundIndex` | `O(log m)` | `O(1)` |
| `valueAt`, `size`, `empty` | `O(1)` | `O(1)` |
| `compress` với `k` giá trị | `O(k log m)` | `O(k)` cho kết quả |

Các cận trên là worst-case, giả sử một lần so sánh tốn `O(1)`. Object giữ
`O(m)` bộ nhớ sau khi xây dựng.

## Ví dụ

[`example.cpp`](example.cpp) nén
`{1000000000, -7, 1000000000, 42}`. Tập đã sắp xếp là
`{-7, 42, 1000000000}`, nên output là:

```text
2 0 2 1
```

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-coordinate-compression-example
/tmp/cp-library-coordinate-compression-example
```

## Bẫy thường gặp

- Quên xóa giá trị trùng, khiến cùng một tọa độ có thể bị hiểu thành nhiều vị
  trí.
- Dùng `lower_bound` trên vector chưa sắp xếp.
- Trộn rank 0-based của header với Fenwick tree 1-based; thường cần cộng `1` khi
  đưa rank vào Fenwick.
- Gọi `indexOf(x)` cho một giá trị chưa có lúc dựng. Nếu cần tra insertion
  position, dùng `lowerBoundIndex(x)`.
- Coi hiệu hai rank là khoảng cách thật giữa hai tọa độ.
- Comparator không phải strict weak ordering hoặc dùng comparator khác giữa
  bước dựng và bước tra.
- Giữ tham chiếu từ `values()` qua một lần `assign`.

## Kiểm thử

[`test.cpp`](test.cpp) kiểm tra input rỗng, hai biên biểu diễn của `int`,
duplicate, giá trị không đăng ký, inverse mapping, batch compression, tái sử
dụng bằng `assign` và comparator giảm dần.

Random differential test dùng seed `0xC001D1A7E`. Với 1.000 vector nhỏ, tập
tham chiếu độc lập là `std::set`; test so sánh danh sách unique, mọi lower/upper
bound trong một miền rộng hơn input, `contains` và phép round-trip. Khi rank sai,
test in seed, test case, query, thao tác và hai kết quả. Test 200.000 phần tử
kiểm tra số lượng unique và round-trip ở kích thước lớn.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-coordinate-compression-test
/tmp/cp-library-coordinate-compression-test
```

## Bài luyện tập

- [CSES Nested Ranges Count](../../../cses/02_sorting_searching/2169_nested-ranges-count)
  — nén endpoint phải của các đoạn thành rank trước khi dùng Fenwick tree.

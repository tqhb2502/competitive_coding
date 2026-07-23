# Duyệt mọi submask — `submask-enumeration`

Với một bitmask `mask`, entry này duyệt mọi `submask` thỏa
`submask & ~mask == 0`: mọi bit bật của `submask` đều phải là bit bật trong
`mask`. Công thức `(submask - 1) & mask` sinh mỗi submask đúng một lần mà không
phải quét toàn bộ không gian bit.

API dùng kiểu unsigned, quy định rõ thứ tự duyệt và tách phiên bản có/không có
zero mask để tránh vòng lặp vô hạn thường gặp.

## Khi nào dùng

Dùng kỹ thuật này khi:

- trạng thái là bitmask và cần thử mọi tập con của một mask cố định;
- subset DP cần chia một tập thành `submask` và phần còn lại `mask ^ submask`;
- cần thử một lớp màu, một nhóm độc lập hoặc một tập lựa chọn nằm trong tập hiện
  tại;
- số bit bật `k = popcount(mask)` đủ nhỏ để `2^k` khả thi.

Không dùng chỉ vì toàn bộ universe có ít hơn 64 bit. Một mask có `k = 50` vẫn
có hơn `10^15` submask. Nếu cần tổng trên submask của **mọi** mask, SOS DP
`O(B 2^B)` thường tốt hơn việc duyệt lồng nhau `O(3^B)`.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
cp_library::forEachSubmask(mask, [&](std::uint64_t submask) {
    // Có xử lý submask == 0.
});

cp_library::forEachNonzeroSubmask(mask, [&](std::uint64_t submask) {
    // Chỉ xử lý submask != 0.
});
```

Các hàm hỗ trợ:

```cpp
const bool contained = cp_library::isSubmask(submask, mask);
const auto bit = cp_library::singleBitOrZero<std::uint64_t>(position);
```

`Mask` phải là kiểu số nguyên unsigned khác `bool`. Visitor được gọi đồng bộ,
mỗi lần nhận một `Mask` theo giá trị. Header không cấp phát bộ nhớ và không giữ
tham chiếu tới visitor sau khi hàm trả về.

`singleBitOrZero<Mask>(position)` trả bit tương ứng, hoặc `0` nếu `position`
không nhỏ hơn số bit giá trị của `Mask`. Giá trị `0` ở đây là sentinel báo vị
trí bit không hợp lệ.

## Thứ tự và zero mask

`forEachSubmask` duyệt theo **giá trị số học giảm dần**:

```text
mask, ..., 0
```

Zero luôn xuất hiện đúng một lần và ở cuối. Riêng `mask == 0`, visitor được gọi
đúng một lần với `0`.

`forEachNonzeroSubmask` có cùng thứ tự nhưng bỏ `0`. Với `mask == 0`, visitor
không được gọi. Tách hai API giúp người dùng không phải chèn `if` hoặc sửa điều
kiện vòng lặp dễ gây lỗi.

Thứ tự giảm theo số không đồng nghĩa với thứ tự theo số phần tử hay từ điển.
Nếu thuật toán cần tăng dần, có thể lưu kết quả rồi đảo, nhưng khi đó tốn
`O(2^k)` bộ nhớ.

## Ý tưởng và invariant

Bắt đầu với `submask = mask`. Sau mỗi lần xử lý một submask khác `0`, cập nhật:

```cpp
submask = (submask - 1) & mask;
```

Phép trừ một làm bit `1` thấp nhất của `submask` tắt và bật mọi bit thấp hơn nó.
Phép `& mask` lập tức xóa các bit thấp không thuộc `mask`. Kết quả là submask
lớn nhất tiếp theo nhỏ hơn giá trị hiện tại.

Invariant của vòng lặp:

- `submask` hiện tại luôn chỉ dùng bit của `mask`;
- dãy đã thăm giảm nghiêm ngặt nên không có duplicate;
- chưa bỏ qua submask hợp lệ nào nằm giữa hai giá trị liên tiếp.

Code chỉ thực hiện `submask - 1` sau khi đã biết `submask != 0`. Dù wraparound
của unsigned được C++ định nghĩa, điều kiện rõ ràng này ngăn zero quay lại
`mask` và tạo vòng lặp vô hạn.

## Vì sao đúng

`mask` là submask lớn nhất và được thăm đầu tiên. Với một `submask > 0`, quy tắc
trừ một rồi giao với `mask` tạo đúng submask hợp lệ lớn nhất nhỏ hơn nó: mọi bit
cao hơn bit thay đổi giữ nguyên, bit `1` thấp nhất bị tắt, và các bit thấp thuộc
`mask` được bật tối đa.

Vì mỗi bước đi tới phần tử hợp lệ kế tiếp theo thứ tự giảm, không phần tử nào bị
bỏ qua hoặc lặp lại. Dãy hữu hạn và giảm nghiêm ngặt nên cuối cùng đạt `0`.
Phiên bản đầy đủ xử lý `0` rồi dừng; phiên bản nonzero dừng trước callback đó.

## Dịch bit an toàn

Không viết `1 << position` một cách máy móc:

- literal `1` có kiểu signed `int`;
- dịch vào bit dấu hoặc dịch với `position >=` độ rộng kiểu là undefined
  behavior;
- ngay cả `1ULL << 64` cũng không hợp lệ trên kiểu 64 bit.

`singleBitOrZero<Mask>` dùng `Mask{1}` và kiểm tra giới hạn **trước** khi nhánh
có phép dịch được đánh giá. Ví dụ:

```cpp
const auto bit63 = cp_library::singleBitOrZero<std::uint64_t>(63);
const auto invalid = cp_library::singleBitOrZero<std::uint64_t>(64);  // 0
```

Khi tự xây `(1 << B) - 1`, cũng phải dùng kiểu unsigned và xử lý riêng
`B == numeric_limits<Mask>::digits`.

## Độ phức tạp

Gọi `k = popcount(mask)`.

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Duyệt mọi submask | `O(2^k)` | `O(1)` ngoài callback |
| Duyệt submask của mọi mask `B` bit | `O(3^B)` | tùy callback |

Mỗi trong `k` bit bật có hai lựa chọn nên có chính xác `2^k` submask, kể cả
zero.

## Ví dụ

[`example.cpp`](example.cpp) duyệt submask của `13 = 1101₂`:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-submask-example
/tmp/cp-library-submask-example
```

Output:

```text
13 12 9 8 5 4 1 0
```

## Bẫy thường gặp

- Viết `for (sub = mask; sub; sub = (sub - 1) & mask)` rồi tưởng rằng zero đã
  được xử lý.
- Đổi thành vòng lặp vô hạn nhưng quên `break` ngay sau khi xử lý zero.
- Dùng kiểu signed; phép toán bit và trừ tại biên trở nên khó kiểm soát.
- Dịch literal `1` quá độ rộng kiểu khi dựng mask.
- Nhầm submask với subset theo index khi nhiều phần tử dùng chung một bit.
- Chạy `O(3^B)` mà không tính trước giới hạn.
- Trong subset DP, duyệt cả `submask` và `mask ^ submask` rồi đếm hai thứ tự như
  hai cách khác nhau dù bài toán chỉ cần một partition không thứ tự.

## Kiểm thử

[`test.cpp`](test.cpp):

- kiểm tra riêng `mask == 0` và mask chứa bit cao nhất của `uint64_t`;
- exhaustive mọi mask 12 bit, so đúng dãy với oracle quét giảm;
- kiểm tra phiên bản nonzero bằng cách bỏ phần tử cuối của oracle;
- chạy 500 mask 64 bit ngẫu nhiên, mỗi mask có tối đa 15 lần chọn bit, với seed
  `0x5AB5E7`;
- xác nhận số lượng `2^popcount(mask)`, tính chứa, không duplicate và thứ tự
  giảm;
- dùng `static_assert` cho vị trí dịch hợp lệ và ngoài miền.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-submask-test
/tmp/cp-library-submask-test
```

## Bài luyện tập

- [CSES Graph Coloring](../../../cses/15_advanced_graph/3308_graph-coloring) —
  duyệt các submask của tập đỉnh còn lại để chọn một lớp màu là tập độc lập.

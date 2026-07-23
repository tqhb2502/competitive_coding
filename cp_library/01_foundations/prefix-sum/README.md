# Prefix Sum — `prefix-sum`

Prefix sum lưu tổng của mọi tiền tố. Sau một lần tiền xử lý, tổng của bất kỳ
đoạn tĩnh nào được tính bằng hiệu của hai tiền tố trong `O(1)`.

## Khi nào dùng

Dùng kỹ thuật này khi:

- có nhiều truy vấn tổng trên một mảng không thay đổi;
- cần tính nhanh tổng của một cửa sổ, một đoạn con hoặc số lượng phần tử thỏa
  điều kiện sau khi đổi chúng thành `0/1`;
- công thức quy hoạch động cần lấy hiệu của hai tổng tiền tố.

Không dùng riêng prefix sum nếu phải cập nhật mảng xen kẽ với truy vấn. Khi đó
cân nhắc Fenwick tree hoặc segment tree. Với cập nhật đoạn thực hiện offline,
difference array thường phù hợp hơn.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
cp_library::PrefixSum<long long> sums(values);

long long total = sums.query(left, right);  // [left, right)
long long first = sums.prefix(end);         // [0, end)
sums.assign(other_values);                  // thay toàn bộ dữ liệu
std::size_t n = sums.size();
```

- Mọi chỉ số đều 0-based và mọi đoạn đều nửa mở `[left, right)`.
- `query(left, right)` chấp nhận đoạn rỗng khi `left == right`.
- Instance mặc định biểu diễn mảng rỗng; `prefix(0)` và `query(0, 0)` trả về
  `Sum{}`.
- Endpoint không hợp lệ hoặc `left > right` gây `std::out_of_range`.
- Constructor và `assign` không sửa vector đầu vào.
- Copy giữ nguyên dữ liệu. Sau move construction/assignment, object đích nhận
  dữ liệu còn object nguồn biểu diễn mảng rỗng và có thể `assign` lại.
- Kiểu `Sum` là kiểu tích lũy. Mặc định là `long long`; người gọi phải chọn kiểu
  đủ lớn để mọi tổng và phép trừ đều biểu diễn được.

## Ý tưởng

Với mảng `a` có `n` phần tử, header lưu vector `prefix_` dài `n + 1`:

```text
prefix_[0] = 0
prefix_[i] = a[0] + a[1] + ... + a[i - 1]
```

Phần tử trung hòa ở vị trí `0` giúp cùng một công thức xử lý cả đoạn bắt đầu tại
`0`. Tổng của `[left, right)` là:

```text
prefix_[right] - prefix_[left]
```

## Vì sao đúng

Sau khi khởi tạo, `prefix_[0] = Sum{}`, đúng bằng tổng của không phần tử.

Ở vòng lặp thứ `i`, giả sử `prefix_[i]` là tổng của `a[0..i)`. Phép gán
`prefix_[i + 1] = prefix_[i] + a[i]` khiến `prefix_[i + 1]` là tổng của
`a[0..i + 1)`. Do đó invariant đúng với mọi tiền tố theo quy nạp.

`prefix_[right]` chứa tổng của `a[0..right)`, còn `prefix_[left]` chứa chính
phần nằm trước đoạn cần hỏi. Lấy hiệu triệt tiêu `a[0..left)`, chỉ còn tổng của
`a[left..right)`. Công thức vẫn đúng khi đoạn rỗng vì hai vế bằng nhau.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Khởi tạo / `assign` | `O(n)` | `O(n)` |
| `prefix` | `O(1)` | `O(1)` |
| `query` | `O(1)` | `O(1)` |

Các cận trên là worst-case.

## Ví dụ

[`example.cpp`](example.cpp) dựng prefix sum cho `{3, -2, 5, 4}` và in tổng
`[1, 4)`, tức `-2 + 5 + 4 = 7`.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-prefix-sum-example
/tmp/cp-library-prefix-sum-example
```

## Bẫy thường gặp

- Nhầm đoạn đóng `[l, r]` của đề với API nửa mở `[l, r)`. Với input 1-based
  dạng `[a, b]`, lời gọi tương ứng thường là `query(a - 1, b)`.
- Quên phần tử trung hòa đầu tiên và phải tách riêng trường hợp `left = 0`.
- Dùng `int` cho tổng dù `n * |a[i]|` vượt giới hạn 32 bit.
- Dùng prefix sum cũ sau khi mảng gốc đã thay đổi; object không theo dõi vector
  đầu vào.
- Tràn số có dấu là undefined behavior. Header không thể tự phát hiện khi kiểu
  `Sum` quá nhỏ.

## Kiểm thử

[`test.cpp`](test.cpp) kiểm tra mảng rỗng, đoạn rỗng, số âm, giá trị trùng,
endpoint, tái sử dụng bằng `assign`, tổng vượt 32 bit và ngoại lệ chỉ số.

Random differential test dùng seed `0xC0FFEE1234`. Test cũng kiểm tra copy/move.
Với 600 mảng nhỏ, test duyệt
mọi đoạn và so sánh với phép cộng tuyến tính độc lập. Khi sai, test in seed,
test case, hai endpoint và hai kết quả. Ngoài ra mảng 200.000 phần tử kiểm tra
kiểu tích lũy và hành vi ở kích thước lớn.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-prefix-sum-test
/tmp/cp-library-prefix-sum-test
```

## Bài luyện tập

- [CSES Static Range Sum Queries](../../../cses/05_range_queries/1646_static-range-sum-queries)
  — ứng dụng trực tiếp một prefix sum để trả lời nhiều truy vấn đoạn tĩnh.

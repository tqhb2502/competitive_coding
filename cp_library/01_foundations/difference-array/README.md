# Difference Array — `difference-array`

Difference array biến một lần cộng trên cả đoạn thành hai thay đổi tại biên.
Khi mọi cập nhật đã biết trước, ta xử lý mỗi cập nhật trong `O(1)` rồi khôi
phục toàn bộ mảng bằng một lần lấy tổng tiền tố.

## Khi nào dùng

Dùng kỹ thuật này khi:

- có nhiều thao tác cộng một hằng số vào đoạn và chỉ cần kết quả cuối cùng;
- các truy vấn/cập nhật có thể xử lý offline;
- cần đánh dấu mức phủ của nhiều đoạn hoặc áp dụng kỹ thuật Imos.

Không dùng implementation này khi phải trả lời giá trị mảng xen kẽ giữa các cập
nhật: mỗi lần `materialize()` vẫn tốn `O(n)`. Trong trường hợp online, có thể lưu
difference array bằng Fenwick tree hoặc dùng lazy segment tree. Header này cũng
không hỗ trợ phép gán đoạn, nhân đoạn hay truy vấn tổng đoạn trực tiếp.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
cp_library::DifferenceArray<long long> diff(initial_values);
diff.add(left, right, delta);       // [left, right) += delta
std::vector<long long> a = diff.materialize();

diff.reset(n);                      // n số 0
diff.assign(other_values);          // thay bằng dữ liệu mới
std::size_t n = diff.size();
```

- Mọi chỉ số đều 0-based và đoạn cập nhật là nửa mở `[left, right)`.
- Đoạn rỗng là no-op, kể cả khi `delta` rất lớn.
- `materialize()` không thay đổi object; có thể gọi nhiều lần.
- Instance mặc định và `reset(0)` biểu diễn mảng rỗng.
- Copy giữ nguyên trạng thái. Sau move construction/assignment, object đích
  nhận dữ liệu còn object nguồn biểu diễn mảng rỗng và có thể `reset`/`assign`
  lại.
- Endpoint không hợp lệ hoặc `left > right` gây `std::out_of_range`.
- Kích thước `SIZE_MAX` không còn chỗ cho sentinel và gây
  `std::length_error`; allocation quá lớn khác cũng có thể ném ngoại lệ chuẩn
  của `std::vector`.
- `T` phải là kiểu số học có dấu. Người gọi phải bảo đảm mọi giá trị, hiệu và
  phép cộng trung gian đều nằm trong miền biểu diễn của `T`.

## Ý tưởng

Mở rộng mảng `a` bằng hai giá trị biên `a[-1] = a[n] = 0`, rồi định nghĩa:

```text
difference_[i] = a[i] - a[i - 1]     với 0 <= i <= n
```

Muốn cộng `delta` vào `[left, right)`, chỉ hai chênh lệch thay đổi:

```text
difference_[left]  += delta
difference_[right] -= delta
```

Các chênh lệch bên trong không đổi vì cả hai phần tử kề nhau đều tăng cùng
`delta`. Tổng tiền tố của `difference_` khôi phục lần lượt từng `a[i]`.

Header lưu cả sentinel tại `difference_[n]`. Nhờ vậy cập nhật chạm biên phải
`right = n` dùng cùng công thức, không cần nhánh đặc biệt.

## Vì sao đúng

`assign` đặt `difference_[0] = a[0]`, mọi phần tử giữa bằng
`a[i] - a[i - 1]`, và sentinel cuối bằng `0 - a[n - 1]`; do đó invariant định
nghĩa ở trên đúng sau khởi tạo. `reset` cũng thỏa invariant vì mọi giá trị bằng
`0`.

Với cập nhật `[left, right)`, tại biên trái, tăng `difference_[left]` làm mọi
tổng tiền tố từ `left` trở đi tăng `delta`. Giảm `difference_[right]` triệt tiêu
mức tăng đó từ `right` trở đi. Vì vậy chỉ các vị trí `left <= i < right` thay
đổi. Đoạn rỗng được bỏ qua nên invariant giữ nguyên.

Trong `materialize`, biến `running` sau vòng lặp tại vị trí `i` bằng
`difference_[0] + ... + difference_[i]`. Tổng này telescoping thành `a[i]`, nên
vector trả về đúng trạng thái sau mọi cập nhật.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Khởi tạo / `assign` | `O(n)` | `O(n)` |
| `reset` | `O(n)` | `O(n)` |
| `add` | `O(1)` | `O(1)` |
| `materialize` | `O(n)` | `O(n)` cho kết quả |

Các cận trên là worst-case. Object luôn chiếm `O(n)` bộ nhớ.

## Ví dụ

[`example.cpp`](example.cpp) bắt đầu từ `{2, 2, 2, 2, 2}`, cộng `3` vào
`[1, 4)`, rồi cộng `-1` vào `[3, 5)`. Kết quả in ra là:

```text
2 5 5 4 1
```

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-difference-array-example
/tmp/cp-library-difference-array-example
```

## Bẫy thường gặp

- Với đoạn đóng `[l, r]`, phải trừ tại `r + 1`; API này nhận trực tiếp đoạn nửa
  mở `[l, r)`.
- Chỉ cấp phát `n` phần tử rồi truy cập `difference_[n]` khi cập nhật đến cuối.
  Header cố ý cấp phát `n + 1`.
- Quên lấy tổng tiền tố để khôi phục kết quả.
- Gọi `materialize()` sau từng cập nhật và vô tình biến lời giải thành `O(nq)`.
- Dùng kiểu unsigned: phép trừ tại biên phải biểu diễn số âm.
- Chỉ kiểm tra giá trị cuối cùng mà quên hiệu trung gian cũng có thể tràn kiểu
  số.

## Kiểm thử

[`test.cpp`](test.cpp) kiểm tra mảng rỗng, đoạn rỗng, hai biên, số âm, phần tử
trùng, gọi `materialize()` nhiều lần, copy/move, `reset`, `assign`, kích thước
`SIZE_MAX`, ngoại lệ và số vượt 32 bit.

Random differential test dùng seed `0xD1FF3A55A9`. Với 500 test case, mỗi cập
nhật được áp dụng đồng thời lên `DifferenceArray` và một vector brute force, rồi
so sánh toàn bộ kết quả. Lỗi sẽ in seed, test case, số thứ tự cập nhật, vị trí và
hai giá trị. Test còn dùng các giá trị gần biên `long long`; test lớn thực hiện
200.000 cập nhật trên 200.000 phần tử để kiểm tra đúng độ phức tạp và kiểu tích
lũy.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-difference-array-test
/tmp/cp-library-difference-array-test
```

## Bài luyện tập

- [CSES Range Update Queries](../../../cses/05_range_queries/1651_range-update-queries)
  — dùng đúng invariant mảng hiệu; vì bài yêu cầu online nên thay vector hiệu
  tĩnh bằng Fenwick tree.

# Monotonic Deque — `monotonic-deque`

Monotonic deque giữ lại đúng các chỉ số còn khả năng trở thành min hoặc max của
cửa sổ hiện tại và tương lai. Mỗi chỉ số vào và ra deque nhiều nhất một lần,
nên xử lý mọi cửa sổ trong thời gian tuyến tính.

## Khi nào dùng

Dùng kỹ thuật này khi:

- cần min hoặc max cho mọi đoạn liên tiếp có cùng độ dài `width`;
- dữ liệu được xử lý từ trái sang phải;
- cần tốt hơn cách dùng multiset `O(n log k)`.

Không dùng deque này để lấy median, mode hay phần tử nhỏ thứ `k`; thứ tự đơn
điệu chỉ đủ để giữ một cực trị. Nếu chỉ cần tổng hoặc một trạng thái cập nhật
được bằng thêm/xóa, framework `sliding-window` đơn giản hơn.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
const auto min_indices =
    cp_library::sliding_window_min_indices(values, width);
const auto max_indices =
    cp_library::sliding_window_max_indices(values, width);

const auto minimums =
    cp_library::sliding_window_min_values(values, width);
const auto maximums =
    cp_library::sliding_window_max_values(values, width);
```

API tổng quát nhận comparator:

```cpp
const auto indices = cp_library::sliding_window_extreme_indices(
    values, width, better
);
```

`better(a, b)` mang nghĩa `a` là cực trị tốt hơn `b` và phải là strict weak
ordering. `std::less<T>` lấy min; `std::greater<T>` lấy max.

Các chỉ số là 0-based. Khi nhiều vị trí cùng có giá trị cực trị, API chỉ số trả
về vị trí **ngoài cùng bên phải**. `width == 0`, dãy rỗng hoặc `width > n` trả
về vector rỗng. Hàm không sửa input; API giá trị sao chép các phần tử kết quả.

## Ý tưởng và invariant

Deque lưu chỉ số tăng dần từ front tới back. Với comparator `better`, các giá
trị tương ứng đi từ tốt tới kém một cách nghiêm ngặt:

```text
front = cực trị hiện tại ... back = ứng viên kém nhất
```

Khi xử lý chỉ số `index`:

1. Xóa ở front mọi chỉ số đã nằm ngoài cửa sổ.
2. Trong khi phần tử ở back không tốt hơn phần tử mới, xóa back.
3. Đưa `index` vào back.
4. Khi cửa sổ đã đủ rộng, front là chỉ số cực trị.

Bước 2 cũng xóa giá trị bằng nhau. Phần tử mới tồn tại lâu hơn phần tử cũ nên
giữ bản mới là an toàn và tạo quy tắc tie-break ngoài cùng bên phải.

## Vì sao đúng

Một chỉ số bị xóa ở front đã hết hạn nên không thuộc cửa sổ nào tiếp theo. Một
chỉ số bị xóa ở back vừa không tốt hơn phần tử mới, vừa cũ hơn; trong mọi cửa
sổ tương lai còn chứa nó, phần tử mới cũng hiện diện và thắng hoặc hòa, nên chỉ
số cũ không thể cần lại.

Do đó deque chứa toàn bộ và chỉ những ứng viên chưa bị chi phối. Phần tử tốt
nhất luôn ở front, suy ra mỗi kết quả được báo đúng. Mỗi chỉ số được push một
lần, pop nhiều nhất một lần ở mỗi đầu.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Mọi cực trị cửa sổ | `O(n)` amortized | `O(k)` |

API trả về dùng thêm `O(n - k + 1)` bộ nhớ cho vector kết quả; bảng chỉ tính
deque làm việc.

## Ví dụ

[`example.cpp`](example.cpp) lấy min và chỉ số max của các cửa sổ dài `3` trong
`{4, 2, 2, 5, 1, 3}`:

```text
2 2 1 1
0 3 3 3
```

Hai giá trị `2` bằng nhau được xử lý nhất quán: cửa sổ đầu tiên giữ chỉ số `2`,
tức bản sao mới hơn.

## Bẫy thường gặp

- Lưu giá trị thay vì chỉ số nên không biết phần tử ở front đã hết hạn chưa.
- Chỉ pop khi phần tử mới tốt hơn nghiêm ngặt nhưng lại mong tie-break bên
  phải; cách cài này pop cả khi tương đương.
- Hết hạn sai một đơn vị do nhầm `[left, right)` với đoạn đóng.
- Đảo comparator: `std::less` tạo minimum, không phải maximum.
- Dùng comparator không phải strict weak ordering, khiến khái niệm “tương
  đương” không nhất quán.

## Kiểm thử

[`test.cpp`](test.cpp) so sánh cả min, max, chỉ số, giá trị và comparator tùy
chỉnh với oracle `O(nk)`. Test chạy 5.000 dãy ngẫu nhiên bằng seed `20260723`;
ngoài ra có dãy rỗng, `width = 0`, `width > n`, `n = 1`, dãy tăng/giảm, nhiều
phần tử trùng, `INT_MIN`/`INT_MAX` và dãy 200.000 phần tử với cửa sổ dài `946`.

## Bài luyện tập

- [CSES Sliding Window Minimum](../../../cses/11_sliding_window/3221_sliding-window-minimum)
  — ứng dụng trực tiếp monotonic deque để lấy min cho từng cửa sổ.

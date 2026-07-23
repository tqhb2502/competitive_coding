# Sliding Window — `sliding-window`

Sliding window giữ trạng thái của một đoạn liên tiếp `[left, right)` và cập
nhật trạng thái khi một đầu di chuyển. Header cung cấp bộ điều phối callback;
người dùng tự định nghĩa cách thêm, xóa và đọc trạng thái.

## Khi nào dùng

Dùng cửa sổ cố định khi cần cùng một thống kê cho mọi đoạn dài đúng `width`.
Dùng cửa sổ biến đổi khi tính hợp lệ có tính di truyền: mọi đoạn con của một
cửa sổ hợp lệ cũng hợp lệ. Khi đó, với mỗi đầu phải, các suffix có dạng đơn
điệu:

```text
không hợp lệ ... không hợp lệ | hợp lệ ... hợp lệ
```

khi tăng đầu trái; đồng thời một cửa sổ đã không hợp lệ không thể được sửa bằng
cách nối thêm phần tử ở bên phải. Điều này đúng, chẳng hạn, với `sum <= limit`
khi mọi phần tử không âm, hoặc “không có quá `k` giá trị phân biệt”.

Không dùng `for_each_longest_valid_suffix` nếu xóa đầu trái có thể làm cửa sổ
từ hợp lệ thành không hợp lệ rồi lại hợp lệ. Ví dụ, điều kiện tổng với số âm
không có tính đơn điệu cần thiết.

Header này chỉ điều phối biên cửa sổ. Để lấy min/max mọi cửa sổ trong `O(n)`,
dùng entry `monotonic-deque`; callback tổng quát ở đây không tự cung cấp cấu
trúc cực trị.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
cp_library::for_each_fixed_window(
    size, width, add, remove, visit
);

cp_library::for_each_longest_valid_suffix(
    size, add, remove, is_valid, visit
);
```

Hợp đồng callback:

- `add(index)` thêm phần tử tại chỉ số 0-based `index` vào trạng thái;
- `remove(index)` xóa đúng phần tử đó khỏi trạng thái;
- `visit(left, right)` đọc kết quả của cửa sổ `[left, right)`;
- `is_valid()` kiểm tra trạng thái hiện tại, không thay đổi trạng thái.

Trước mỗi lời gọi API, trạng thái do callback quản lý phải biểu diễn cửa sổ
rỗng. Hàm không tự reset trạng thái. Sau khi kết thúc, trạng thái chứa cửa sổ
được visit cuối cùng; nếu không có cửa sổ nào được visit, trạng thái ban đầu
được giữ nguyên.

Với cửa sổ cố định:

- `width == 0` gọi `visit(i, i)` cho `i = 0..size`, không gọi `add/remove`;
- `width > size` không gọi callback nào;
- nếu `1 <= width <= size`, mỗi cửa sổ được visit đúng một lần và trạng thái
  cuối chứa cửa sổ cuối cùng.

Với cửa sổ biến đổi, cửa sổ rỗng bắt buộc hợp lệ và mọi đoạn con của cửa sổ
hợp lệ cũng phải hợp lệ. Hàm gọi `visit` đúng một lần sau mỗi phần tử được
thêm; cửa sổ được visit có thể rỗng nếu phần tử vừa thêm tự nó không hợp lệ.

## Ý tưởng và invariant

### Cửa sổ cố định

Ngay trước `visit`, trạng thái chứa chính xác
`[right + 1 - width, right + 1)`. Khi chuyển sang cửa sổ kế tiếp, hàm xóa đầu
trái cũ rồi thêm đầu phải mới, nên invariant tiếp tục đúng.

### Cửa sổ biến đổi

Sau khi thêm `right`, vòng `while` tăng `left` cho tới lần đầu `is_valid()` trả
về true. Nhờ tính đơn điệu theo `left`, `[left, right + 1)` là suffix hợp lệ
dài nhất. `left` không bao giờ giảm, nên mỗi phần tử được thêm và xóa nhiều
nhất một lần.

## Vì sao đúng

Ở biến thể cố định, quy tắc một phần tử ra và một phần tử vào duy trì đúng tập
chỉ số trong cửa sổ trước mỗi lần visit.

Ở biến thể thay đổi, mọi cửa sổ bắt đầu trước `left` đã bị kiểm tra là không hợp
lệ. Khi đầu phải tăng, chúng trở thành các superwindow của chính chúng trước
đó; tính di truyền bảo đảm chúng vẫn không hợp lệ, nên không cần giảm `left`.
Vòng thu hẹp loại tiếp mọi đầu trái không hợp lệ. Cửa sổ tại `left` là hợp lệ
đầu tiên và do đó là suffix dài nhất.

## Độ phức tạp

| Biến thể | Số lần gọi callback | Bộ nhớ của bộ điều phối |
|---|---:|---:|
| Cố định | `O(n)` | `O(1)` |
| Suffix hợp lệ dài nhất | `O(n)` amortized | `O(1)` |

Tổng thời gian còn phụ thuộc chi phí của `add`, `remove`, `is_valid` và `visit`.
Trạng thái do callback lưu không được tính vào bộ nhớ của bộ điều phối.

## Ví dụ

[`example.cpp`](example.cpp) in tổng của các cửa sổ dài `3`, sau đó in suffix
dài nhất có tổng không quá `5` sau mỗi đầu phải:

```text
6 6 9
[0,1) [0,2) [1,3) [2,4) [4,5)
```

## Bẫy thường gặp

- Dùng điều kiện không đơn điệu, đặc biệt là tổng khi có số âm.
- Gọi `visit` trước khi thu hẹp xong cửa sổ không hợp lệ.
- `remove` xóa theo giá trị và xóa nhầm bản sao khi có phần tử trùng; callback
  nhận chỉ số để tránh mơ hồ này.
- Nhầm cửa sổ đóng `[left, right]` với quy ước nửa mở `[left, right)`.
- Cho rằng framework sở hữu dữ liệu; thực tế callback phải giữ dữ liệu sống
  trong suốt lời gọi.

## Kiểm thử

[`test.cpp`](test.cpp) dùng tổng làm trạng thái và so từng biên cửa sổ với
oracle duyệt trực tiếp. Có 3.000 bộ ngẫu nhiên với seed `20260723`, cùng các
trường hợp rỗng, `width = 0`, `width > n`, một phần tử không hợp lệ, phần tử
trùng, giá trị biên lớn và dãy 200.000 phần tử với `width = 946`.

## Bài luyện tập

- [CSES Playlist](../../../cses/02_sorting_searching/1141_playlist) — cửa sổ
  biến đổi với điều kiện mọi giá trị phân biệt.
- [CSES Sliding Window Sum](../../../cses/11_sliding_window/3220_sliding-window-sum)
  — cập nhật tổng cho mọi cửa sổ có độ dài cố định.

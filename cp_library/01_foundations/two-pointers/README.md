# Two Pointers — `two-pointers`

Two pointers quét một dãy bằng hai chỉ số chỉ đi theo một hướng. Entry này cài
biến thể hai đầu đối nhau trên dãy đã sắp xếp để tìm hoặc đếm cặp theo tổng.

## Khi nào dùng

Dùng kỹ thuật này khi:

- dữ liệu đã được sắp xếp không giảm, hoặc có thể sắp xếp mà vẫn giữ được thông
  tin cần trả lời;
- quyết định bỏ đầu trái hay đầu phải là đơn điệu;
- cần xét các cặp nhưng không muốn duyệt `O(n^2)` cặp.

Không áp dụng trực tiếp cho dãy chưa sắp xếp. Nếu cần trả về chỉ số ban đầu, hãy
sắp xếp các cặp `(giá trị, chỉ số)` thay vì chỉ sắp xếp giá trị.

Template `Sum` được suy ra từ đối số `target`/`limit`, không phải từ kiểu phần
tử. `Sum` phải biểu diễn chính xác từng phần tử sau chuyển kiểu, target và mọi
tổng hai phần tử; chuyển kiểu cũng phải bảo toàn equality và thứ tự. Vì vậy,
với `vector<int>` hoặc `vector<long long>`, cách an toàn thông thường là truyền
`target`/`limit` dưới dạng `long long` như `0LL`, không phải literal `0` kiểu
`int`.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
const auto pair =
    cp_library::find_pair_with_sum_sorted(values, target);

const std::uint64_t count =
    cp_library::count_pairs_with_sum_at_most_sorted(values, limit);
```

- `find_pair_with_sum_sorted` trả về một cặp chỉ số 0-based `left < right`,
  hoặc `std::nullopt`.
- `count_pairs_with_sum_at_most_sorted` đếm mọi cặp chỉ số `(i, j)` với
  `i < j`; các phần tử bằng nhau vẫn là những cặp khác nhau.
- Hai hàm không sửa `values`. Dãy rỗng hoặc có một phần tử cho kết quả lần lượt
  là `nullopt` và `0`.
- Kết quả đếm phải vừa trong `std::uint64_t`.

## Ý tưởng và invariant

Khởi tạo `left = 0`, `right = n - 1`. Trong toàn bộ vòng lặp, mọi cặp chưa bị
loại có hai đầu nằm trong đoạn `[left, right]`; `left` chỉ tăng và `right` chỉ
giảm.

Với bài toán tìm tổng bằng `target`:

- nếu tổng hiện tại nhỏ hơn target, ghép `values[left]` với bất kỳ vị trí nào
  không vượt quá `right` cũng không thể lớn hơn tổng hiện tại, nên có thể bỏ
  `left`;
- nếu tổng lớn hơn target, tương tự mọi cặp dùng `right` đều quá lớn, nên có
  thể bỏ `right`;
- nếu bằng nhau, hai chỉ số hiện tại là đáp án.

Với bài toán đếm tổng không vượt quá `limit`, khi
`values[left] + values[right] <= limit`, toàn bộ `right - left` cặp ghép
`left` với các vị trí từ `left + 1` đến `right` đều hợp lệ. Ta cộng cả khối này
rồi tăng `left`. Nếu tổng quá lớn, chỉ giảm `right`.

## Vì sao đúng

Mỗi lần di chuyển chỉ loại các cặp đã được chứng minh là không thể là đáp án,
hoặc đã đếm trọn một nhóm cặp hợp lệ. Vì hai chỉ số không đi lùi, không cặp nào
được xét lại. Khi chúng gặp nhau, không còn cặp hai chỉ số khác nhau chưa xử lý,
nên kết quả tìm và kết quả đếm đều đúng.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Tìm một cặp | `O(n)` | `O(1)` |
| Đếm cặp | `O(n)` | `O(1)` |

Chi phí sắp xếp, nếu dữ liệu ban đầu chưa được sắp xếp, là `O(n log n)` và
không nằm trong hai hàm.

## Ví dụ

[`example.cpp`](example.cpp) tìm tổng `9` trong `{1, 2, 4, 7, 11}` và đếm các
cặp có tổng không quá `8`:

```text
1 3
4
```

## Bẫy thường gặp

- Di chuyển sai đầu khi tổng nhỏ hoặc lớn hơn target.
- Cho phép `left == right`, vô tình dùng một phần tử hai lần.
- Đếm giá trị phân biệt thay vì đếm cặp chỉ số khi có phần tử trùng.
- Cộng hai `int` trước rồi mới ép sang `long long`, khiến overflow xảy ra trước
  phép ép kiểu.
- Truyền literal `int` làm target cho `vector<long long>` khiến `Sum` bị suy ra
  là `int` và làm hẹp từng phần tử trước khi cộng.
- Quên rằng sắp xếp làm mất thứ tự chỉ số ban đầu.

## Kiểm thử

[`test.cpp`](test.cpp) so sánh cả hai API với duyệt `O(n^2)` trên 4.000 dãy
ngẫu nhiên với seed `20260723`. Test còn bao phủ dãy rỗng, một phần tử, nhiều
giá trị trùng, tổng của các giá trị `INT_MIN`/`INT_MAX` và dãy 200.000 phần tử
để kiểm tra đường chạy tuyến tính.

## Bài luyện tập

- [CSES Ferris Wheel](../../../cses/02_sorting_searching/1090_ferris-wheel)
  — sắp xếp trọng lượng rồi ghép người nhẹ nhất với người nặng nhất khi tổng
  không vượt giới hạn; cả hai đầu chỉ di chuyển một chiều.

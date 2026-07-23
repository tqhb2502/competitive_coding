# Monotonic Stack — `monotonic-stack`

Monotonic stack tìm phần tử gần nhất ở bên trái hoặc bên phải thỏa quan hệ nhỏ
hơn/lớn hơn. Mỗi index chỉ được đẩy vào và lấy khỏi stack tối đa một lần, nên
bốn dạng truy vấn cơ bản đều chạy trong `O(n)`.

Entry cố ý không chọn mặc định giữa strict và non-strict. Người gọi phải ghi rõ
chính sách duplicate ngay tại chỗ gọi API.

## Khi nào dùng

Dùng monotonic stack khi:

- với mỗi `a[i]`, cần index gần nhất ở một phía có giá trị nhỏ/lớn hơn;
- cần biên trái/phải nơi `a[i]` còn là minimum hoặc maximum, chẳng hạn largest
  rectangle in histogram;
- đang đếm contribution của từng phần tử dựa trên phần tử chặn đầu tiên;
- chỉ cần kết quả trên mảng tĩnh và thứ tự index phải được giữ nguyên.

Không dùng trực tiếp nếu có update xen giữa query. Khi đó cân nhắc segment tree
hoặc một cấu trúc chuyên biệt. Sliding-window minimum có phần tử hết hạn ở đầu
hàng đợi, nên monotonic deque thường phù hợp hơn stack.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
using cp_library::MonotonicStrictness;

auto left = cp_library::nearestPreviousLess(
    values, MonotonicStrictness::Strict);
auto right = cp_library::nearestNextGreater(
    values, MonotonicStrictness::NonStrict);
```

Bốn hàm công khai:

- `nearestPreviousLess`;
- `nearestNextLess`;
- `nearestPreviousGreater`;
- `nearestNextGreater`.

Tất cả trả về `std::vector<std::size_t>` chứa index **0-based**. Nếu không có
phần tử phù hợp, sentinel là `values.size()`. Input rỗng trả về vector rỗng.
Header không sửa `values` và không giữ trạng thái giữa các lần gọi.

Tham số comparator mặc định là `std::less<T>`. Có thể truyền comparator khác,
nhưng nó phải tạo strict weak ordering. Các từ “less”, “greater” và “equal”
khi đó đều được hiểu theo comparator; tránh giá trị như floating-point `NaN`
làm hỏng thứ tự này.

## Strict, non-strict và duplicate

Với phần tử hiện tại `a[i]`, ý nghĩa chính xác là:

| Hàm | `Strict` | `NonStrict` |
|---|---|---|
| `PreviousLess` / `NextLess` | `a[j] < a[i]` | `a[j] <= a[i]` |
| `PreviousGreater` / `NextGreater` | `a[j] > a[i]` | `a[j] >= a[i]` |

Trong mọi trường hợp, `j` là index gần `i` nhất về đúng phía đã chọn.

Do đó với hai giá trị bằng nhau:

- mode `Strict` loại index bằng nhau khỏi stack;
- mode `NonStrict` giữ index bằng nhau làm đáp án hợp lệ.

Đây là khác biệt quan trọng khi đếm contribution. Ví dụ để mỗi subarray có
minimum trùng nhau được tính đúng một lần, thường một phía dùng strict và phía
còn lại dùng non-strict. Dùng cùng một chính sách ở cả hai phía có thể đếm
thiếu hoặc đếm lặp.

## Ý tưởng và invariant

Xét `nearestPreviousLess` khi duyệt từ trái sang phải. Stack chứa các index đã
gặp và chưa bị một phần tử mới chứng minh là vô dụng.

Trước khi xử lý `i`, lấy khỏi đỉnh mọi candidate không thỏa quan hệ đã chọn với
`a[i]`:

- strict less: pop khi `candidate >= a[i]`;
- non-strict less: pop khi `candidate > a[i]`.

Sau đó, nếu stack không rỗng, đỉnh là đáp án. Cuối cùng đẩy `i` vào stack.
Phiên bản greater đảo quan hệ so sánh. Phiên bản next duyệt đối xứng từ phải
sang trái.

Stack lưu **index**, không chỉ lưu giá trị. Nhờ vậy duplicate vẫn phân biệt
được vị trí và output dùng trực tiếp cho độ dài đoạn.

## Vì sao đúng

Xét một candidate `k` bị pop khi đang xử lý `i`. Giá trị tại `i` gần mọi phần
tử tương lai hơn `k`, đồng thời nó ít nhất cũng “tốt bằng” `k` theo quan hệ cần
tìm. Vì thế `k` không thể là đáp án gần nhất cho bất kỳ phần tử được xử lý sau
đó.

Sau khi pop xong, mọi index còn lại trong stack thỏa quan hệ với `a[i]`. Đỉnh
có index lớn nhất trong số đó vì các index được push theo thứ tự duyệt, nên nó
là phần tử hợp lệ gần nhất ở bên trái. Nếu stack rỗng thì mọi index bên trái đã
bị chứng minh không hợp lệ.

Lập luận đối xứng đúng khi duyệt từ phải sang trái. Điều kiện pop có hoặc không
loại phần tử bằng nhau đúng theo bảng strict/non-strict, nên duplicate không
làm thay đổi chứng minh.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Một trong bốn phép tìm | `O(n)` amortized | `O(n)` |

Một vòng `while` có thể pop nhiều phần tử, nhưng mỗi index chỉ push một lần và
pop nhiều nhất một lần trong cả lời gọi. Vì vậy tổng số thao tác stack là
`O(n)`, không phải `O(n^2)`.

## Ví dụ

[`example.cpp`](example.cpp) so sánh previous less strict và non-strict trên
`{2, 3, 3, 1}`:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-monotonic-stack-example
/tmp/cp-library-monotonic-stack-example
```

Output:

```text
-1 0 0 -1
-1 0 1 -1
```

Ở index `2`, giá trị bằng nhau tại index `1` chỉ hợp lệ trong mode non-strict.
Ví dụ đổi sentinel `values.size()` thành `-1` khi in; vector trả về vẫn dùng
kiểu `std::size_t`.

## Bẫy thường gặp

- Viết “nearest smaller” nhưng không xác định là `<` hay `<=`.
- Trả về giá trị thay vì index rồi không thể tính khoảng cách.
- Dùng `-1` trong `std::size_t`, tạo một số unsigned rất lớn. API này dùng `n`.
- Với next element, duyệt trái sang phải nhưng lại diễn giải stack như previous
  element.
- Tưởng vòng `while` lồng nhau là `O(n^2)` và thay bằng cấu trúc phức tạp hơn.
- Khi tính histogram/contribution, không quyết định bên nào sở hữu duplicate.
- Comparator không nhất quán hoặc dữ liệu chứa `NaN`.

## Kiểm thử

[`test.cpp`](test.cpp):

- kiểm tra tường minh mảng toàn duplicate;
- exhaustive mọi mảng dài đến `8` trên bảng chữ cái `{-1, 0, 1}`;
- so cả bốn hướng/quan hệ và hai mode với brute force `O(n^2)`;
- chạy 5.000 mảng ngẫu nhiên bằng seed `0x57AC`;
- kiểm tra comparator tùy chỉnh theo trị tuyệt đối.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-monotonic-stack-test
/tmp/cp-library-monotonic-stack-test
```

## Bài luyện tập

- [CSES Nearest Smaller Values](../../../cses/02_sorting_searching/1645_nearest-smaller-values)
  — dùng previous strictly less; output của đề là 1-based và dùng `0` khi không
  tồn tại nên cần đổi từ API của entry.

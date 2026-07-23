# Binary Search trên miền số nguyên — `binary-search`

Entry này tìm đúng chỗ một predicate đổi từ `false` sang `true`. Mẫu hai
sentinel giữ invariant rõ ràng, không phải xử lý riêng trường hợp “không tìm
thấy”, đồng thời midpoint vẫn an toàn khi miền chứa cả `LLONG_MIN` và
`LLONG_MAX`.

## Khi nào dùng

Dùng entry này khi:

- cần giá trị nhỏ nhất thỏa một điều kiện;
- cần giá trị lớn nhất chưa thỏa điều kiện;
- đang “binary search đáp án” và có hàm kiểm tra khả thi đơn điệu;
- miền tìm kiếm là số nguyên, kể cả kiểu unsigned hoặc một khoảng rất rộng.

Không dùng nếu predicate có dạng `false -> true -> false`, thay đổi trong lúc
tìm, hoặc kết quả nằm trên miền số thực. Nếu chưa chứng minh được tính đơn điệu,
binary search có thể trả một giá trị hợp lệ cục bộ nhưng không phải đáp án.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
const auto boundary = cp_library::binarySearchBoundary(
    false_sentinel, true_sentinel, predicate);

const auto first = cp_library::binarySearchFirstTrue(
    false_sentinel, true_sentinel, predicate);

const auto last = cp_library::binarySearchLastFalse(
    false_sentinel, true_sentinel, predicate);
```

`Integer` phải là kiểu số nguyên khác `bool`. Predicate nhận một `Integer` và
trả về giá trị chuyển được sang `bool`.

Hợp đồng bắt buộc:

1. `false_sentinel < true_sentinel`;
2. predicate tại `false_sentinel` là `false`;
3. predicate tại `true_sentinel` là `true`;
4. trên toàn đoạn đóng giữa chúng, mọi giá trị `false` đứng trước mọi giá trị
   `true`.

Hai sentinel là **giá trị thật trong miền của predicate**, không phải ký hiệu vô
cực. Hàm không tự kiểm tra điều kiện 2–4 vì việc gọi predicate có thể đắt; người
gọi chịu trách nhiệm thiết lập chúng.

Kết quả gồm hai số nguyên kề nhau:

- `last_false`: giá trị cuối cùng có predicate bằng `false`;
- `first_true`: giá trị đầu tiên có predicate bằng `true`.

Nếu chỉ cần một phía, dùng hai wrapper tương ứng. Mọi indexing và đơn vị của
kết quả giống hệt miền mà người gọi truyền vào.

## Chọn miền và sentinel

Giả sử đáp án cần tìm thuộc `[L, R]`. Mẫu thường dùng là:

- `L - 1` làm false sentinel;
- `R` làm true sentinel nếu đã biết `R` thỏa điều kiện, hoặc `R + 1` nếu
  predicate được định nghĩa và đúng tại đó.

Không tính `L - 1` khi `L` là giá trị nhỏ nhất của kiểu, và không tính `R + 1`
khi `R` là giá trị lớn nhất. Trong tình huống đó, hãy tìm một sentinel nằm bên
trong kiểu số, đổi cách định nghĩa predicate, hoặc dùng một kiểu rộng hơn.

Một cách xây dựng true sentinel phổ biến là nhân đôi cận trên cho đến khi
predicate đúng. Cách này cũng phải chặn trước phép nhân để không overflow.

## Ý tưởng và invariant

Trong toàn bộ vòng lặp:

- `false_sentinel < true_sentinel`;
- predicate ở đầu trái là `false`;
- predicate ở đầu phải là `true`;
- ranh giới cần tìm nằm giữa hai đầu này.

Khi hai đầu chưa kề nhau, `middle` nằm nghiêm ngặt giữa chúng. Nếu predicate tại
`middle` đúng, gán nó cho đầu phải; nếu sai, gán nó cho đầu trái. Vì vậy
invariant luôn được duy trì và khoảng chưa biết giảm sau mỗi vòng.

Không dùng `(low + high) / 2`, vì tổng có thể overflow. Ngay cả
`low + (high - low) / 2` vẫn có thể overflow ở phép trừ khi `low < 0 <= high`
và miền gần phủ toàn bộ kiểu signed. `safeIntegerMidpoint` tách riêng trường
hợp khác dấu, tính `low / 2 + high / 2`; các trường hợp cùng dấu dùng hiệu an
toàn.

Điều kiện dừng là `false_sentinel + 1 == true_sentinel`. Phép cộng này an toàn:
do đầu trái vẫn nhỏ hơn đầu phải, nó không thể đang là giá trị lớn nhất của
kiểu.

## Vì sao đúng

Ban đầu, hợp đồng của API bảo đảm hai đầu lần lượt là `false` và `true`, nên
ranh giới nằm giữa chúng.

Ở mỗi vòng, `middle` nằm trong khoảng mở. Tính đơn điệu cho biết:

- nếu `predicate(middle)` đúng thì mọi giá trị từ `middle` trở đi cũng đúng;
  thay đầu phải bằng `middle` không loại mất first true;
- nếu predicate sai thì mọi giá trị đến `middle` đều sai; thay đầu trái bằng
  `middle` không loại mất last false.

Khoảng giảm nghiêm ngặt nên vòng lặp kết thúc. Khi kết thúc, hai sentinel kề
nhau; không còn số nguyên nào ở giữa. Invariant khi đó suy ra chúng chính xác là
last false và first true.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Tìm cả ranh giới | `O(log D)` lần gọi predicate | `O(1)` |

`D` là khoảng cách toán học giữa hai sentinel ban đầu. Nếu một lần kiểm tra mất
`O(f(n))`, tổng thời gian là `O(f(n) log D)`.

## Ví dụ

[`example.cpp`](example.cpp) tìm thời gian nhỏ nhất để các máy tạo đủ bảy sản
phẩm:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-binary-search-example
/tmp/cp-library-binary-search-example
```

Output là `8`. Predicate cộng số sản phẩm nhưng dừng ngay khi đạt mục tiêu, nhờ
đó không để tổng tiếp tục tăng đến overflow.

## Bẫy thường gặp

- Đảo vai trò sentinel: code này yêu cầu đầu trái `false`, đầu phải `true`.
- Predicate không đơn điệu, đặc biệt khi phép tính bên trong đã overflow.
- Dùng `int` cho đáp án có thể đến `10^18`.
- Lấy `R` làm true sentinel nhưng chưa chứng minh `predicate(R)`.
- Tìm last true bằng cách đổi vài dấu so sánh trong code. Hãy đổi predicate
  thành mệnh đề phủ định phù hợp rồi vẫn dùng cùng invariant.
- Predicate có side effect hoặc phụ thuộc trạng thái biến đổi giữa các lần gọi.

## Kiểm thử

[`test.cpp`](test.cpp) kiểm tra exhaustive mọi ranh giới trên các đoạn nhỏ,
differential test với oracle tuyến tính bằng seed `0xB1A2C3`, hai sentinel kề
nhau, miền `uint64_t` đầy đủ, và miền signed từ `INT64_MIN` đến `INT64_MAX`.
Các test cực trị trực tiếp bắt lỗi từ cả hai công thức midpoint overflow phổ
biến.

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-binary-search-test
/tmp/cp-library-binary-search-test
```

## Bài luyện tập

- [CSES Factory Machines](../../../cses/02_sorting_searching/1620_factory-machines)
  — binary search thời gian nhỏ nhất đủ sản phẩm.
- [CSES Array Division](../../../cses/02_sorting_searching/1085_array-division)
  — binary search giá trị lớn nhất cho phép của một đoạn.

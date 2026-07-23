# Example Entry — `example-entry`

Một câu mô tả ngắn: kỹ thuật này giải quyết bài toán gì và lợi ích chính là gì.
Đổi cả tiêu đề, `id` và include guard trong `algorithm.hpp` khi sao chép
template.

## Khi nào dùng

Dùng entry này khi:

- dấu hiệu thứ nhất của đề bài;
- dấu hiệu thứ hai và ràng buộc tương ứng;
- cần các thao tác mà API bên dưới cung cấp.

Không dùng khi giả thiết quan trọng không được thỏa mãn. Nêu rõ phương án thay
thế nếu có một nhánh dễ nhầm.

## Giao diện

Source chính: [`algorithm.hpp`](algorithm.hpp).

```cpp
#include "algorithm.hpp"

const long long result = cp_library::sumValues(values);
```

Mô tả ngắn cho từng hàm/type công khai:

- input dùng indexing nào;
- object có sửa dữ liệu đầu vào hay không;
- output hoặc sentinel có ý nghĩa gì;
- trường hợp rỗng và giá trị biên được xử lý ra sao.

## Ý tưởng

Trình bày các trạng thái, invariant và từng pha của thuật toán. Gắn tên biến
trong phần giải thích với tên thật trong source để người đọc đối chiếu nhanh.

Nếu có nhiều biến thể, chỉ rõ biến thể được cài trong header và lý do lựa chọn.

## Vì sao đúng

1. Chứng minh invariant sau bước khởi tạo.
2. Chứng minh mỗi thao tác duy trì invariant.
3. Chứng minh invariant tại thời điểm kết thúc suy ra đúng output cần tìm.

Có thể thay cấu trúc này bằng exchange argument, song ánh hoặc quy nạp nếu phù
hợp hơn, nhưng không bỏ qua các giả thiết quan trọng.

## Độ phức tạp

| Thao tác | Thời gian | Bộ nhớ phụ |
|---|---:|---:|
| Xử lý chính | `O(n)` | `O(1)` |

Ghi rõ worst-case, amortized hoặc expected. Bảng này phải khớp với object
`complexities` trong `meta.json`.

## Ví dụ

[`example.cpp`](example.cpp) là ví dụ tối thiểu có thể biên dịch:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  example.cpp -o /tmp/cp-library-example
/tmp/cp-library-example
```

Giải thích ngắn output của ví dụ, thay vì chỉ lặp lại code.

## Bẫy thường gặp

- Sai indexing hoặc quên đổi giữa 0-based và 1-based.
- Dùng kiểu số không đủ rộng.
- Áp dụng thuật toán khi giả thiết không còn đúng.
- Quên reset trạng thái khi chạy nhiều test.

Thay danh sách mẫu bằng các lỗi đặc thù của entry.

## Kiểm thử

[`test.cpp`](test.cpp) nên bao gồm test nhỏ nhất, test điển hình, test biên và
oracle/brute force nếu phù hợp:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wshadow -pedantic \
  test.cpp -o /tmp/cp-library-test
/tmp/cp-library-test
```

Nêu cách tái hiện seed nếu entry có stress test ngẫu nhiên.

## Bài luyện tập

- [CSES Missing Number](../../../cses/01_introductory/1083_missing-number) — ví
  dụ về việc dùng tổng các phần tử; thay bằng bài phù hợp khi tạo entry thật.

Các đường dẫn tại đây phải đồng bộ với `practice` trong `meta.json`.

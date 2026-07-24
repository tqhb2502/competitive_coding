# 01 — Foundations

Nhóm các mẫu nền tảng xuất hiện trong nhiều chủ đề và thường là bước biến đổi
đầu tiên trước khi dùng một cấu trúc phức tạp hơn. Entry ở đây nên có API nhỏ,
ít giả thiết và dùng được độc lập.

## Khi nên tìm ở đây

- Cần tiền xử lý để query một đoạn tĩnh.
- Hai con trỏ hoặc cửa sổ có thể chỉ tiến, không cần quay lại.
- Predicate đơn điệu cho phép binary search.
- Giá trị lớn nhưng chỉ thứ tự tương đối quan trọng.
- Cần một invariant duyệt mảng tuyến tính như monotonic stack/deque.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Tiền xử lý mảng | Prefix sum, difference array |
| Duyệt có cấu trúc | Two pointers, sliding window |
| Tìm kiếm | Binary search, binary search đáp án |
| Chuẩn hóa dữ liệu | Coordinate compression |
| Duy trì đơn điệu | Monotonic stack, monotonic deque |
| Kỹ thuật bit cơ bản | Submask enumeration, bit operations an toàn |

## Các entry

| Entry | Dùng nhanh khi |
|---|---|
| [Prefix Sum](prefix-sum/README.md) | Dữ liệu tĩnh, cần tổng nhiều đoạn `[l, r)` |
| [Difference Array](difference-array/README.md) | Biết trước mọi cập nhật đoạn rồi mới cần mảng cuối |
| [Binary Search](binary-search/README.md) | Predicate đơn điệu, cần biên đúng/sai đầu tiên hoặc cuối cùng |
| [Coordinate Compression](coordinate-compression/README.md) | Giá trị lớn nhưng chỉ thứ tự và equality quan trọng |
| [Two Pointers](two-pointers/README.md) | Hai chỉ số chỉ tiến nhờ sort hoặc một invariant đơn điệu |
| [Sliding Window](sliding-window/README.md) | Duy trì một đoạn liên tiếp khi biên trái/phải dịch chuyển |
| [Monotonic Stack](monotonic-stack/README.md) | Cần phần tử nhỏ/lớn gần nhất hoặc thời điểm một phần tử bị loại |
| [Monotonic Deque](monotonic-deque/README.md) | Cần min/max của mọi cửa sổ cố định trong `O(n)` |
| [Submask Enumeration](submask-enumeration/README.md) | Duyệt chính xác các submask của một bitmask |

## Ranh giới với nhóm khác

- Có update/query online: xem [Data Structures](../02_data_structures/README.md).
- Được phép đổi thứ tự query là ý tưởng chính: xem
  [Offline Algorithms](../09_offline_algorithms/README.md).
- State/chuyển trạng thái quyết định lời giải: xem
  [Dynamic Programming](../05_dynamic_programming/README.md).

Một entry vẫn có thể dùng Fenwick hoặc sort trong example, nhưng nếu phần tái sử
dụng chính là Fenwick thì entry phải nằm ở Data Structures.

## Yêu cầu cho entry

Nêu rõ:

- indexing và dạng đoạn `[l, r]` hay `[l, r)`;
- điều kiện đơn điệu hoặc lý do mỗi con trỏ chỉ tiến;
- identity/sentinel nếu có;
- worst-case và amortized complexity;
- test rỗng, một phần tử, duplicate và hai đầu miền.

Xem [cách thêm snippet](../CONTRIBUTING.md),
[chỉ mục theo use case](../indexes/by-use-case.md) và
[các bẫy về indexing](../indexes/pitfalls.md).

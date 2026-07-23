# 09 — Offline Algorithms

Nhóm kỹ thuật tận dụng việc biết trước toàn bộ query/update để đổi thứ tự xử lý,
chia theo thời gian hoặc rollback trạng thái. Query id phải được giữ để phục
hồi thứ tự output.

## Khi nên tìm ở đây

- Đề không yêu cầu trả lời query ngay khi đọc.
- Sort theo một tọa độ biến query thành prefix.
- Cửa sổ query thay đổi bằng add/remove rẻ.
- Có thêm/xóa cạnh nhưng toàn bộ timeline đã biết.
- Mỗi query có predicate đơn điệu và có thể binary search song song.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Sắp sự kiện | Sweep line |
| Cửa sổ | Mo's algorithm, Mo có update |
| Dominance | CDQ divide-and-conquer |
| Tìm đáp án | Parallel binary search |
| Timeline động | Segment tree theo thời gian + rollback DSU |
| Truy vấn cây/mảng | Offline LCA, sort theo endpoint |

## Trước khi chọn entry

1. Query có phụ thuộc output trước không?
2. Có thể đổi thứ tự xử lý và phục hồi bằng `id` không?
3. Add/remove có thật sự rẻ hơn query độc lập không?
4. Event cùng thời điểm dùng đoạn đóng hay nửa mở?
5. Rollback cần lưu chính xác những field nào?

## Ranh giới với nhóm khác

- Fenwick/DSU nền tảng thuộc
  [Data Structures](../02_data_structures/README.md).
- Thuật toán graph static thuộc [Graphs](../03_graphs/README.md).
- Sweep có primitive hình học phức tạp có thể nằm ở
  [Geometry](../08_geometry/README.md).

## Yêu cầu cho entry

- Nêu thứ tự sort/tie-break và cách khôi phục thứ tự output.
- Complexity phải gồm cả sort, số lần add/remove và rollback.
- Test nhiều event cùng tọa độ/thời gian, query rỗng, update lặp và nhánh
  rollback không merge.
- Stress bằng lời giải online/brute cho input nhỏ khi có thể.

Xem [cây quyết định online/offline](../indexes/decision-trees.md#online-hay-offline)
và [bẫy offline](../indexes/pitfalls.md#offline-algorithms).

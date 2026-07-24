# 02 — Data Structures

Nhóm các cấu trúc dữ liệu tái sử dụng cho thao tác build, update và query. API
và invariant của cấu trúc quan trọng ngang với bản thân thuật toán.

## Khi nên tìm ở đây

- Dữ liệu thay đổi giữa các query.
- Cần aggregate trên prefix, đoạn hoặc toàn bộ tập.
- Cần thứ tự, phần tử thứ `k`, predecessor/successor.
- Cần hợp nhất component hoặc duy trì nhiều phiên bản.
- Một cấu trúc có thể đóng gói độc lập khỏi đề bài.

## Phạm vi

| Họ cấu trúc | Ví dụ |
|---|---|
| Prefix/đoạn | Fenwick, segment tree, lazy segment tree |
| Truy vấn tĩnh | Sparse table, disjoint sparse table |
| Tập hợp | DSU, ordered set, heap |
| Cấu trúc theo bit | Binary trie, XOR basis |
| Phiên bản/lịch sử | Persistent segment tree |
| Đường thẳng/hàm | Li Chao tree, convex hull container |

## Chọn cấu trúc

Trước khi chọn entry, ghi rõ:

1. phép merge có associative không;
2. identity là gì;
3. update điểm hay đoạn;
4. query online hay offline;
5. có cần undo/persistence không;
6. miền chỉ số liên tục hay phải nén.

Fenwick thường đơn giản và nhanh hơn segment tree nếu phép toán phù hợp. Sparse
table chỉ dùng query chồng lấn trực tiếp cho phép idempotent.

## Ranh giới với nhóm khác

- DSU dùng để giải một bài graph vẫn nằm ở đây nếu API DSU là nội dung chính.
- Euler tour/HLD/LCA thuộc [Trees](../04_trees/README.md).
- Rollback structure kết hợp trục thời gian thường được trình bày tại
  [Offline Algorithms](../09_offline_algorithms/README.md).

## Yêu cầu cho entry

- Ghi rõ complexity `build`, `update`, `query`, `memory` ở dòng `ĐPT:`.
- Nêu indexing (0/1-based), đoạn rỗng, identity và hành vi khi query ngoài miền.
- Đặt tên type/hàm **duy nhất trên toàn kho** để bundle nhiều snippet không xung đột.

Xem [cây quyết định truy vấn mảng](../indexes/decision-trees.md#truy-vấn-trên-mảng)
và [bẫy cấu trúc dữ liệu](../indexes/pitfalls.md#cấu-trúc-dữ-liệu).

# 04 — Trees

Nhóm kỹ thuật khai thác tính không chu trình của cây/forest để tiền xử lý,
query đường đi, query cây con hoặc làm dynamic programming.

## Khi nên tìm ở đây

- Cần LCA, khoảng cách hoặc tổ tiên thứ `k`.
- Cần biến subtree thành một đoạn.
- Cần update/query trên đường đi.
- Cần gộp thông tin con, đổi gốc hoặc phân rã cây.
- Khoảng cách trên cây được query nhiều lần.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Tiền xử lý | Euler tour, binary lifting, LCA |
| Query đường/cây con | HLD, flattening |
| DP trên cây | Subtree DP, rerooting |
| Phân rã | Centroid decomposition |
| Gộp dữ liệu | Small-to-large, DSU on tree |
| Cây động | Link-cut tree khi thật sự cần online |

## Quyết định quan trọng

- Aggregate nằm trên đỉnh hay trên cạnh?
- Tree có root cố định hay cần mọi root?
- Query subtree, path hay khoảng cách tới một tập đỉnh?
- Có update không?
- Input chắc chắn liên thông hay là forest?

Với HLD trên cạnh, đoạn chứa LCA thường phải loại một vị trí; đây cần được ghi
rõ trong API và test.

## Ranh giới với nhóm khác

- Segment tree dùng sau flattening nằm ở
  [Data Structures](../02_data_structures/README.md) nếu entry chỉ cài segment
  tree.
- Rerooting/tree knapsack vẫn ở Trees khi topology cây là phần cốt lõi.
- Dynamic connectivity trên graph tổng quát thuộc
  [Offline Algorithms](../09_offline_algorithms/README.md) hoặc Graphs.

## Yêu cầu cho entry

- Nêu root, parent sentinel, indexing và quy ước `tin/tout`.
- Tránh recursion overflow hoặc ghi rõ giới hạn stack.
- Test cây một đỉnh, đường thẳng, hình sao, forest nếu hỗ trợ và query có hai
  đầu trùng nhau.

Xem [cây quyết định bài trên cây](../indexes/decision-trees.md#bài-trên-cây) và
[bẫy cây](../indexes/pitfalls.md#cây).

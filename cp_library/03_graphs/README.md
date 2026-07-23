# 03 — Graphs

Nhóm thuật toán trên đồ thị tổng quát: duyệt, connectivity, đường đi, cấu trúc
thành phần và tối ưu trên cạnh. Entry phải nói rõ loại đồ thị và giả thiết về
trọng số.

## Khi nên tìm ở đây

- Input là đỉnh/cạnh hoặc chuyển trạng thái tạo thành graph.
- Cần reachability, shortest path, SCC, bridge hoặc MST.
- Cần matching, flow hay circulation.
- Cần dựng đường đi/cạnh được chọn, không chỉ giá trị tối ưu.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Duyệt | BFS, DFS lặp, topological sort |
| Đường đi | 0-1 BFS, Dijkstra, Bellman–Ford, Floyd–Warshall |
| Thành phần | SCC, bridge, articulation point, biconnected component |
| Cây khung | Kruskal, Prim, MST classification |
| Ghép cặp và luồng | Hopcroft–Karp, Dinic, min-cost flow |
| Chu trình/đường đi đặc biệt | Euler tour, functional graph |

## Trước khi chọn entry

- Đồ thị có hướng hay vô hướng?
- Có multi-edge/self-loop không?
- Trọng số có âm không?
- Input có liên thông không?
- Cần một nguồn, nhiều nguồn hay mọi cặp?
- Cần giá trị hay cả witness/path?

Dijkstra với cạnh âm và DSU cho bài có xóa cạnh là hai lựa chọn sai phổ biến.

## Ranh giới với nhóm khác

- Input chắc chắn là cây: xem [Trees](../04_trees/README.md).
- Connectivity có thêm/xóa cạnh offline: xem
  [Offline Algorithms](../09_offline_algorithms/README.md).
- Matrix exponentiation/đếm walk thuần đại số có thể thuộc
  [Mathematics](../07_mathematics/README.md).

## Yêu cầu cho entry

- Nêu rõ directed/undirected, indexing và kiểu trọng số.
- Dùng edge id khi multi-edge ảnh hưởng low-link hoặc reconstruction.
- Tránh DFS đệ quy không an toàn ở giới hạn lớn.
- Test graph rỗng, một đỉnh, disconnected, parallel edge, self-loop và trọng số
  biên khi những trường hợp đó hợp lệ.

Xem [cây quyết định đường đi](../indexes/decision-trees.md#đường-đi-ngắn-nhất)
và [bẫy đồ thị](../indexes/pitfalls.md#dfs-bfs-và-đồ-thị).

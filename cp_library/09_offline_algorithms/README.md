# 09 — Thuật toán offline

6 snippet. Tệp này **tự sinh** bởi `tools/gen_docs.py` từ header các `.hpp` — đừng sửa tay.
Tra theo dấu hiệu đề: [indexes/by-use-case.md](../indexes/by-use-case.md) · cây quyết định: [indexes/decision-trees.md](../indexes/decision-trees.md) · bẫy: [indexes/pitfalls.md](../indexes/pitfalls.md)

| Kỹ thuật | Mục đích | Bài CSES | File |
|---|---|---|---|
| CDQ Divide & Conquer | đếm thứ tự bộ phận BA CHIỀU (thời gian, x, y) offline: chia để trị + Fenwick, O(n log^2 n) | — *(không có bài trực tiếp trong bộ CSES hiện tại)* | [cdq-divide-conquer.hpp](cdq-divide-conquer.hpp) |
| Mo's Algorithm | trả lời NHIỀU truy vấn đoạn \[l, r\] OFFLINE bằng cách sắp xếp theo khối, O((n + q) * sqrt(n)) | [1734 Distinct Values Queries](https://cses.fi/problemset/task/1734) | [mo-algorithm.hpp](mo-algorithm.hpp) |
| Offline Dynamic Connectivity | cạnh XUẤT HIỆN / BIẾN MẤT theo thời gian, trả lời "u, v liên thông?" tại các mốc | [2133 Dynamic Connectivity](https://cses.fi/problemset/task/2133) | [offline-dynamic-connectivity.hpp](offline-dynamic-connectivity.hpp) |
| Offline LCA (Tarjan) | trả lời MỌI truy vấn LCA cùng lúc bằng một lượt DFS + DSU, O((n + q) α(n)) | [1135 Distance Queries](https://cses.fi/problemset/task/1135)<br>[1136 Counting Paths](https://cses.fi/problemset/task/1136)<br>[1688 Company Queries II](https://cses.fi/problemset/task/1688) | [offline-lca.hpp](offline-lca.hpp) |
| Parallel Binary Search | chặt nhị phân ĐỒNG THỜI mọi truy vấn: mẫu "ngày sớm nhất hai đỉnh liên thông" | [2101 New Roads Queries](https://cses.fi/problemset/task/2101) | [parallel-binary-search.hpp](parallel-binary-search.hpp) |
| Sqrt Decomposition | chia mảng thành các khối ~sqrt(n) hỗ trợ CỘNG ĐOẠN + TỔNG ĐOẠN, O(sqrt(n)) mỗi thao tác | [1735 Range Updates and Sums](https://cses.fi/problemset/task/1735) | [sqrt-decomposition.hpp](sqrt-decomposition.hpp) |

# 08 — Hình học

9 snippet. Tệp này **tự sinh** bởi `tools/gen_docs.py` từ header các `.hpp` — đừng sửa tay.
Tra theo dấu hiệu đề: [indexes/by-use-case.md](../indexes/by-use-case.md) · cây quyết định: [indexes/decision-trees.md](../indexes/decision-trees.md) · bẫy: [indexes/pitfalls.md](../indexes/pitfalls.md)

| Kỹ thuật | Mục đích | Bài CSES | File |
|---|---|---|---|
| Closest Pair | khoảng cách nhỏ nhất giữa hai điểm trong mặt phẳng bằng quét dọc + tập theo y, O(n log n) | [2194 Minimum Euclidean Distance](https://cses.fi/problemset/task/2194) | [closest-pair.hpp](closest-pair.hpp) |
| Convex Hull (Andrew monotone chain) | bao lồi của tập điểm nguyên theo CCW, O(n log n) (sắp xếp) | [2195 Convex Hull](https://cses.fi/problemset/task/2195) | [convex-hull.hpp](convex-hull.hpp) |
| Geometry Basics | điểm nguyên + tích có hướng (cross), tích vô hướng (dot), hướng quay (orient), điểm trên đoạn | [2189 Point Location Test](https://cses.fi/problemset/task/2189) | [geometry-basics.hpp](geometry-basics.hpp) |
| Line Intersection | giao điểm HAI ĐƯỜNG THẲNG (mỗi đường qua hai điểm nguyên), trả tọa độ thực, O(1) | — *(không có bài trực tiếp trong bộ CSES hiện tại)* | [line-intersection.hpp](line-intersection.hpp) |
| Manhattan Distance | đường kính online qua x+y/x-y và tổng khoảng cách mọi cặp bằng sort + prefix sum | [3410 Maximum Manhattan Distances](https://cses.fi/problemset/task/3410)<br>[3411 All Manhattan Distances](https://cses.fi/problemset/task/3411) | [manhattan-distance.hpp](manhattan-distance.hpp) |
| Point in Polygon | kiểm một điểm nằm TRONG / TRÊN BIÊN / NGOÀI đa giác đơn bằng số vòng (winding number), O(n) | [2192 Point in Polygon](https://cses.fi/problemset/task/2192) | [point-in-polygon.hpp](point-in-polygon.hpp) |
| Polygon Area & Pick | diện tích đa giác (công thức con giày) + số điểm nguyên trên biên / bên trong (định lý Pick) | [2191 Polygon Area](https://cses.fi/problemset/task/2191)<br>[2193 Polygon Lattice Points](https://cses.fi/problemset/task/2193) | [polygon-area.hpp](polygon-area.hpp) |
| Rotating Calipers | đường kính tập điểm (khoảng cách LỚN NHẤT giữa hai điểm) qua bao lồi + kẹp xoay, O(n log n) | — *(không có bài trực tiếp trong bộ CSES hiện tại)* | [rotating-calipers.hpp](rotating-calipers.hpp) |
| Segment Intersection | kiểm hai đoạn thẳng có giao nhau không (kể cả chạm đầu mút / chồng lên nhau), O(1) | [2190 Line Segment Intersection](https://cses.fi/problemset/task/2190) | [segment-intersection.hpp](segment-intersection.hpp) |

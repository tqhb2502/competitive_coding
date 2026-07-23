# 08 — Geometry

Nhóm primitive và thuật toán hình học. Mục tiêu mặc định là số học chính xác;
chỉ dùng floating point khi bản chất input yêu cầu.

## Khi nên tìm ở đây

- Cần orientation, giao đoạn, diện tích hoặc khoảng cách.
- Cần bao lồi, point-in-polygon hoặc closest pair.
- Có sweep theo tọa độ hình học.
- Cần sort theo góc hoặc thao tác với đường/thẳng.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Primitive | Point/vector, dot, cross, orientation |
| Đoạn/đường | Intersection, projection |
| Đa giác | Area, point-in-polygon, convex hull |
| Khoảng cách | Closest pair, rotating calipers |
| Sweep | Intersection/counting theo sự kiện |

## Chính xác số học

- Với tọa độ nguyên, giữ cross và diện tích gấp đôi ở dạng nguyên.
- Ước lượng tích lớn nhất để chọn `long long` hoặc `__int128`.
- Không dùng epsilon trong comparator của `sort` hay `set`.
- Quy định rõ điểm trên biên được tính trong hay ngoài.

## Ranh giới với nhóm khác

- Sweep line tổng quát có thể đặt ở
  [Offline Algorithms](../09_offline_algorithms/README.md); primitive hình học
  vẫn ở đây.
- Convex hull trick cho DP không phải convex hull hình học; xem Dynamic
  Programming hoặc Data Structures.

## Yêu cầu cho entry

- Nêu kiểu tọa độ, miền giá trị và semantics với collinear.
- Test điểm trùng, đoạn độ dài 0, endpoint chạm nhau, đa giác theo cả hai chiều
  và tọa độ âm/lớn.
- Nếu dùng floating point, ghi rõ tolerance và scale.

Xem [cây quyết định hình học](../indexes/decision-trees.md#hình-học) và
[bẫy hình học](../indexes/pitfalls.md#hình-học).

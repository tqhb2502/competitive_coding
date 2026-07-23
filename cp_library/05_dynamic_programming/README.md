# 05 — Dynamic Programming

Nhóm các mẫu thiết kế trạng thái, chuyển trạng thái và tối ưu DP. Entry không
chỉ đưa recurrence mà phải giải thích vì sao state chứa đủ thông tin.

## Khi nên tìm ở đây

- Nghiệm tối ưu/đếm có optimal substructure.
- Quyết định hiện tại phụ thuộc một prefix, interval, subset hoặc profile nhỏ.
- Greedy không có exchange argument.
- Recurrence đúng nhưng transition hiện tại quá chậm và cần tối ưu có điều kiện.

## Phạm vi

| Dạng DP | Ví dụ |
|---|---|
| Một chiều/prefix | LIS variants, state theo phần tử cuối |
| Chọn tập | Knapsack, subset DP |
| Đoạn | Interval DP |
| Chữ số | Digit DP |
| Miền hẹp | Profile/plug DP |
| Tối ưu transition | Monotone queue, CHT, divide-and-conquer, Knuth |
| Dựng nghiệm | Parent tracking, Hirschberg-style reconstruction |

## Thiết kế trước khi code

1. Định nghĩa state bằng một câu đầy đủ.
2. Chứng minh mọi nghiệm có thể tách theo transition.
3. Chọn thứ tự tính không đọc state chưa sẵn sàng.
4. Xác định base case và state không thể đạt.
5. Tính số state nhân chi phí mỗi transition.
6. Lập kế hoạch reconstruction nếu output cần witness.

## Ranh giới với nhóm khác

- Tree DP đặt tại [Trees](../04_trees/README.md) nếu cấu trúc cây quyết định
  transition.
- Matrix exponentiation cho recurrence dài thuộc
  [Mathematics](../07_mathematics/README.md).
- CHT/Li Chao dạng cấu trúc tái sử dụng có thể đặt ở Data Structures; entry tối
  ưu recurrence cụ thể vẫn đặt tại đây.

## Yêu cầu cho entry

- Ghi rõ nghĩa từng chiều state và sentinel.
- Phân biệt update tăng/giảm chỉ số của 0/1 và unbounded knapsack.
- Chỉ tuyên bố tối ưu D&C/Knuth khi điều kiện đơn điệu đã được nêu và chứng minh.
- Test bằng brute force cho `n` nhỏ, kể cả state không thể đạt và nhiều nghiệm
  tối ưu bằng nhau.

Xem [cây quyết định DP](../indexes/decision-trees.md#chọn-dạng-dp) và
[bẫy DP](../indexes/pitfalls.md#dynamic-programming).

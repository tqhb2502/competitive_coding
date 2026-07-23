# 07 — Mathematics

Nhóm công cụ số học, tổ hợp, đại số và xác suất. Entry phải phân biệt rõ tính
chất toán học với chi tiết cài đặt dưới modulo.

## Khi nên tìm ở đây

- Input rất lớn nhưng đáp án có công thức hoặc recurrence ngắn.
- Cần gcd, inverse, CRT, sieve hoặc factorization.
- Cần đếm tổ hợp dưới modulo.
- Cần giải hệ, nhân ma trận hoặc convolution.
- Cần kỳ vọng/xác suất chính xác hoặc có kiểm soát sai số.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Số học | GCD, extended Euclid, CRT, primality, factorization |
| Tiền xử lý | Sieve, divisor/multiple transforms |
| Tổ hợp | Factorial, binomial, inclusion–exclusion, Möbius |
| Đại số | Matrix exponentiation, Gaussian elimination |
| Đa thức | NTT, interpolation |
| Xác suất | Expected value, modular probability |

## Câu hỏi bắt buộc

- Modulo có nguyên tố không?
- Mẫu có khả nghịch dưới modulo không?
- Phép nhân lớn nhất cần `long long` hay `__int128`?
- Cần exact integer/rational hay floating point?
- Precompute tới miền giá trị nào và có đủ memory không?

## Ranh giới với nhóm khác

- NTT/FFT cài đặt chuyên sâu có thể đặt tại
  [Advanced Techniques](../10_advanced_techniques/README.md).
- XOR basis/trie có API cấu trúc dữ liệu thuộc Data Structures.
- Hình học dùng cross/dot thuộc [Geometry](../08_geometry/README.md), dù đều là
  số học nguyên.

## Yêu cầu cho entry

- Nêu miền hợp lệ của tham số và modulo.
- Chứng minh phép chia/inverse hợp lệ.
- Test `0`, `1`, số nguyên tố/hợp số, modulo nhỏ và giá trị sát overflow.
- Nếu dùng randomized primality/factorization, ghi seed và mức bảo đảm.

Xem [cây quyết định modulo](../indexes/decision-trees.md#số-học-và-modulo) và
[bẫy số nguyên/modulo](../indexes/pitfalls.md#số-nguyên-và-overflow).

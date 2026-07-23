# 06 — Strings

Nhóm thuật toán cho pattern, substring, palindrome, từ điển và cấu trúc suffix.
Mỗi entry phải công bố alphabet và quy ước indexing.

## Khi nên tìm ở đây

- Cần tìm pattern hoặc nhiều pattern trong text.
- Cần border, period, palindrome hoặc LCP.
- Có nhiều query so sánh substring.
- Cần đếm substring khác nhau hoặc duy trì tập từ.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Matching đơn | KMP, Z-function |
| Matching nhiều mẫu | Aho–Corasick |
| Palindrome | Manacher, palindromic tree |
| Từ điển | Trie |
| Cấu trúc suffix | Suffix array, LCP, suffix automaton |
| Hash | Rolling hash xác suất, double hash |

## Chọn entry

- Một pattern: KMP/Z thường đủ và xác định.
- Nhiều pattern: Aho–Corasick nếu tổng độ dài phù hợp.
- Nhiều query LCP/substrings tĩnh: suffix array + RMQ.
- Palindrome quanh mọi tâm: Manacher.
- Hash chỉ nên dùng khi xác suất va chạm được chấp nhận và ghi rõ.

## Ranh giới với nhóm khác

- Trie dùng chủ yếu cho XOR số nguyên có thể thuộc
  [Data Structures](../02_data_structures/README.md).
- DP trên hai chuỗi thuộc [Dynamic Programming](../05_dynamic_programming/README.md)
  nếu recurrence là ý tưởng cốt lõi.
- FFT/NTT để matching/chập có thể nằm ở
  [Advanced Techniques](../10_advanced_techniques/README.md).

## Yêu cầu cho entry

- Nêu alphabet, encoding, 0-based/1-based và semantics của mảng output.
- Test chuỗi rỗng nếu hỗ trợ, một ký tự, toàn giống nhau, pattern chồng lấn và
  ký tự ngoài alphabet nhỏ.
- Với hash, ghi modulo/base, cách nhân an toàn và rủi ro collision.

Xem [cây quyết định chuỗi](../indexes/decision-trees.md#chuỗi) và
[bẫy chuỗi](../indexes/pitfalls.md#chuỗi).

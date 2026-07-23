# 10 — Advanced Techniques

Nhóm các kỹ thuật chuyên sâu, thường có chứng minh hoặc cài đặt dài và không có
nhóm tự nhiên hơn. “Advanced” không phải lý do để bỏ qua điều kiện áp dụng hoặc
test đối chiếu.

## Khi nên tìm ở đây

- Độ phức tạp mục tiêu cần biến đổi nhanh hoặc tối ưu chuyên biệt.
- Kỹ thuật kết hợp nhiều cấu trúc và có invariant khó.
- Bài yêu cầu cài đặt algebraic transform, decomposition hoặc randomized
  algorithm nâng cao.
- Các nhóm 01–09 không mô tả đúng ý tưởng cốt lõi.

## Phạm vi

| Chủ đề | Ví dụ |
|---|---|
| Biến đổi nhanh | FFT, NTT, FWT |
| Tối ưu | Divide-and-conquer optimization, Knuth, Aliens trick |
| Cấu trúc nâng cao | Link-cut tree, persistent/implicit structures phức tạp |
| Đại số/chuỗi nâng cao | Polynomial algorithms, suffix structures chuyên sâu |
| Kỹ thuật ngẫu nhiên | Randomized hashing/factorization có kiểm soát |

Danh sách này không độc quyền. Flow vẫn thuộc Graphs, centroid decomposition
vẫn thuộc Trees, và Li Chao thuần API vẫn thuộc Data Structures.

## Tiêu chuẩn tài liệu

Entry nâng cao phải:

- nêu đủ điều kiện cần, không chỉ công thức cuối;
- tách primitive tái sử dụng khỏi adapter của bài cụ thể;
- giải thích invariant bằng tên biến thật trong source;
- có ví dụ tối thiểu và một oracle độc lập cho trường hợp nhỏ;
- ghi worst-case/amortized/expected chính xác;
- chỉ dùng `#pragma` hoặc instruction đặc thù khi entry công bố rõ tính không
  portable; mặc định ưu tiên C++17 portable.

## Kiểm thử

Ngoài unit test, nên có:

- differential test với brute force;
- seed tái hiện;
- input đối kháng cho complexity;
- sanitizer khi source dùng index/pointer phức tạp;
- test nhiều lần gọi để phát hiện state rò rỉ.

Xem [chỉ mục theo complexity](../indexes/by-complexity.md),
[cây quyết định](../indexes/decision-trees.md) và
[checklist trước khi nộp](../indexes/pitfalls.md#checklist-60-giây-trước-khi-nộp).

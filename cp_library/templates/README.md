# Mẫu C++17

Thư mục này chứa các điểm khởi đầu nhỏ, tự chứa cho bài lập trình thi đấu.
Hãy sao chép mẫu phù hợp sang bài mới rồi bổ sung phần đọc dữ liệu và thuật
toán cụ thể.

## Các mẫu lời giải

- `base.cpp`: khung cơ bản với hàm `solve`.
- `graph.cpp`: đồ thị có trọng số, hàm thêm cạnh và Dijkstra.
- `geometry.cpp`: kiểu `Point`, phép trừ vector, tích có hướng và orientation.
- `interactive.cpp`: hai hàm `ask`/`answer` có flush; `main` mặc định không gửi
  truy vấn nên có thể dùng cho smoke test an toàn.

Các mẫu không giả định định dạng input của một bài cụ thể. Đặc biệt, chỉ gọi
`ask` khi chương trình thật sự đang kết nối với interactor.

## Mẫu một entry thư viện

Thư mục `../_templates/entry` gồm:

- `algorithm.hpp`: thuật toán header-only mẫu;
- `example.cpp`: ví dụ nhỏ có output;
- `test.cpp`: kiểm thử bằng `assert`.

Hàm `sumValues` chỉ là nội dung giữ chỗ để cả example và test luôn biên dịch,
chạy được. Khi tạo entry mới, hãy đổi tên hàm, include guard và phần cài đặt;
đồng thời cập nhật `meta.json`, README, example và test tương ứng.

## Quy ước

- Dùng C++17 và chỉ include những header chuẩn thực sự cần.
- Thụt lề 4 dấu cách.
- Tên biến, hàm và kiểu dữ liệu bằng tiếng Anh, ưu tiên tên mô tả rõ nghĩa.
- Comment bằng tiếng Việt, tập trung vào invariant và lý do của bước xử lý.
- Không để logic phụ thuộc vào input chưa được mô tả trong template.

Từ root repository, smoke-test toàn bộ mẫu và entry bằng:

```sh
bash cp_library/tools/verify_all.sh
```

Công cụ biên dịch trong thư mục tạm, nên không để lại binary cạnh source.

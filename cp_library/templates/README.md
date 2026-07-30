# Mẫu & đồ nghề thi đấu (C++17)

Chép mẫu phù hợp thành `main.cpp` rồi viết lời giải. Mọi mẫu đều biên dịch và chạy an toàn khi
không có input (dùng cho smoke test trong `tools/verify.sh`).

## Khung lời giải

| Tệp | Dùng khi | Ghi chú |
|---|---|---|
| [base.cpp](base.cpp) | mặc định, mọi bài | fast IO + `solve()`; có sẵn dòng cho đề nhiều test case |
| [graph.cpp](graph.cpp) | bài đồ thị | đọc danh sách kề rồi gọi snippet (`bfs`/`dijkstra`), tự đổi 1-based → 0-based |
| [geometry.cpp](geometry.cpp) | bài hình học | **include** `08_geometry/geometry-basics.hpp`, KHÔNG khai báo lại `Point`/`cross` |
| [interactive.cpp](interactive.cpp) | bài tương tác | `ask`/`answer` có `flush`; `main` mặc định không gửi truy vấn |

Các mẫu include snippet theo đường dẫn từ **gốc repo**, nên biên dịch cần `-I`:

```sh
g++ -std=c++17 -O2 -Wall -Wextra -I . cp_library/templates/base.cpp -o sol
```

## Gỡ lỗi

[debug.hpp](debug.hpp) — `dbg(x, v, mp)` in ra **stderr**, tự nhận biết số/xâu/container/pair:

```sh
g++ -std=c++17 -O2 -DCP_DEBUG -I . main.cpp -o sol   # bật
g++ -std=c++17 -O2 -I . main.cpp -o sol              # tắt: dbg() biến mất, không tốn gì
```

Khi nộp thì **không** truyền `-DCP_DEBUG`, nên để `dbg()` trong code cũng an toàn.

## Stress test (tìm test làm sai)

Ba mảnh ghép: lời giải chính (`main.cpp`), [brute.cpp](brute.cpp) (ngây thơ, chắc đúng),
[gen.cpp](gen.cpp) (sinh test nhỏ ngẫu nhiên). Sửa `gen.cpp`/`brute.cpp` cho khớp định dạng đề, rồi:

```sh
bash cp_library/templates/stress.sh main.cpp brute.cpp gen.cpp 500
```

[stress.sh](stress.sh) biên dịch cả ba, chạy `gen` với seed = số vòng, so output bằng `diff -w`, và
**dừng ngay khi lệch** — in ra input cùng hai kết quả. Cần bash thật + `g++` trên PATH
(Windows: `export PATH="$HOME/Downloads/w64devkit/bin:$PATH"`).

## Quy ước

- C++17, `#include <bits/stdc++.h>`, thụt lề 4 dấu cách.
- Identifier tiếng Anh, comment tiếng Việt có dấu.
- **Không** khai báo lại kiểu/hàm đã có trong snippet (`Point`, `cross`, `Edge`, `dijkstra`…): khi bundle
  sẽ xung đột hoặc nhập nhằng. Cần gì thì `#include` snippet đó.
- Mẫu không giả định định dạng input của một bài cụ thể; chỗ cần sửa đánh dấu `TODO`.

Kiểm toàn bộ kho (gồm biên dịch mọi mẫu + smoke test `stress.sh`):

```sh
bash cp_library/tools/verify.sh
```

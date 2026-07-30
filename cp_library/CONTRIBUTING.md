# Thêm snippet vào CP Library

Mỗi kỹ thuật là **một file `.hpp`** trong nhóm phù hợp (`01_foundations` … `11_greedy`),
tên `kebab-case` (ví dụ `segment-tree.hpp`). Mục tiêu: tra nhanh, dán nhanh, gộp một file để nộp.

## Khuôn một snippet

```cpp
#pragma once
#include <bits/stdc++.h>
using namespace std;
// <Tên> — <mục đích một dòng>.
// Khi dùng: <dấu hiệu đề / bài toán>
// ĐPT: <mọi thao tác, TÍNH CẢ sort/dựng lại cấu trúc>; bộ nhớ O(...)
// Dùng: <1-3 dòng ví dụ gọi API>
// Bẫy: <index base, đoạn đóng/nửa mở, overflow, giả định đầu vào>
// CSES: —          (do tools/link_cses.py sinh từ dòng Tags trong cses/**/idea.txt — đừng sửa tay)
struct TenDuyNhat {
    // Ý nghĩa: <hàm này làm gì>.
    // Tham số: <tên = ý nghĩa; ...>.
    // Trả về: <giá trị/ý nghĩa trả về>.
    int method(int x) { /* ... */ }
};

#ifdef CP_DEMO       // g++ -std=c++17 -DCP_DEMO -x c++ <ten-file>.hpp -o demo && ./demo
int main() {
    // ví dụ nhỏ, chạy được, in kết quả kèm comment giá trị mong đợi
    return 0;
}
#endif
```

Quy ước bắt buộc:

- **Tự chứa**, biên dịch độc lập. Identifier tiếng Anh, comment **tiếng Việt có dấu**.
- **Tên type/hàm duy nhất trên toàn kho** để bundle nhiều snippet không đụng nhau; tránh tên chung
  (`gcd`, `Edge`, `Node`, `solve`…). Helper phụ nên là **lambda cục bộ** trong hàm.
- **Khối doc trên MỖI hàm**: `// Ý nghĩa:` / `// Tham số:` / `// Trả về:`. Bỏ dòng cho phù hợp —
  hàm không tham số bỏ *Tham số*, hàm `void` bỏ *Trả về* (nêu tác dụng trong *Ý nghĩa*), constructor
  chỉ có *Ý nghĩa* + *Tham số*.
- Ghi rõ **0/1-based**, **đoạn đóng `[l, r]` hay nửa mở `[l, r)`**, kiểu số / overflow, identity của phép gộp.
- Demo nằm trong `#ifdef CP_DEMO int main(){...}#endif` — **đừng để `main` trần** (bundle sẽ nhân đôi `main`).
- Hình học: dùng **nguyên khối guard `#ifndef CP_GEO_BASE`** giống các file `08_geometry` khác (copy
  y nguyên) để bundle nhiều file hình học không định nghĩa lại `Point`/`cross`.

## Bài học tràn số (đã thực sự gây sai trong kho)

- Nhân theo mod có thể > ~3e9 → ép `(__int128)`.
- `mulmod` cho số gần 2^64 → ép `(unsigned __int128)` (bản CÓ DẤU vẫn tràn).
- Mảng đếm theo **giá trị** → nén giá trị trước (`coordinate-compression`) hoặc nén trong hàm.
- `cross` hình học chỉ an toàn với |tọa độ| ≲ 1e9.

## Sau khi thêm

1. **Stress-test đối chiếu brute** (bắt buộc — đây là cửa chất lượng chính): viết chương trình so
   snippet với cách ngây thơ trên dữ liệu ngẫu nhiên nhỏ, **kèm ca biên và ca quy mô lớn** (giá trị
   tới 1e9, n tới giới hạn đề) rồi chạy tới khi sạch. Nếu sửa API lõi hoặc một lỗi có thể tái diễn,
   thêm ca tương ứng vào [`tests/regression.cpp`](tests/regression.cpp) hoặc một tệp
   `tests/*.cpp` chuyên biệt; `verify.sh` tự tìm, biên dịch và chạy tất cả các tệp đó.
2. Thêm một dòng vào bảng tra cứu trong [README.md](README.md) và, nếu hợp, vào
   [indexes/decision-trees.md](indexes/decision-trees.md) (hai tệp này viết TAY).
3. Gắn `Tags:` cho các bài CSES dùng kỹ thuật này (xem dưới), khai báo ngoại lệ nếu
   thật sự không có bài trực tiếp, rồi cập nhật link ngược trong header:

   ```sh
   python3 cp_library/tools/link_cses.py
   ```

   Snippet chưa có bài CSES trực tiếp phải được giải trình rõ trong
   [`tools/cses_unmapped_allowlist.txt`](tools/cses_unmapped_allowlist.txt);
   tool sẽ chặn mọi trường hợp thiếu mapping không được khai báo và cả ngoại lệ đã lỗi thời.

4. **Sau khi `link_cses.py` đã cập nhật header**, sinh lại phần tra cứu tự động
   (bảng "Khi dùng"/ĐPT/Bẫy, README từng nhóm và chỉ mục CSES hai chiều):

   ```sh
   python3 cp_library/tools/gen_docs.py
   ```

   Thứ tự này quan trọng vì README từng nhóm đọc chính dòng `// CSES:` vừa sinh.

5. Chạy kiểm chứng toàn kho (cần **git bash** + `g++` trên PATH):

   ```sh
   export PATH="$HOME/Downloads/w64devkit/bin:$PATH"
   bash cp_library/tools/verify.sh
   ```

   Nó biên dịch mọi `.hpp` + chạy demo, biên dịch/chạy mẫu, smoke test `stress.sh`, kiểm luồng bundle,
   chạy property/regression test, kiểm tag CSES, và kiểm **tài liệu khớp hệ thống tệp**
   (`check_docs.py`, `gen_docs.py --check`).

   Trên Windows, `verify.sh` tự tìm lần lượt `python3`, `python`, rồi `py -3`
   (yêu cầu Python ≥ 3.9). Với các lệnh Python chạy tay trong tài liệu này, nếu máy
   không có `python3` thì thay bằng `python` hoặc `py -3`.

## Gắn tag CSES

Thêm một dòng `Tags:` vào `idea.txt`, ngay sau dòng URL và trước `## Đề bài`:

```text
https://cses.fi/problemset/task/<id>

Tags: dsu, sorting

## Đề bài
```

- lowercase `kebab-case`, tăng dần theo từ điển, không trùng, phân tách bằng `, `;
- tag phải là tên một file `.hpp` (id kỹ thuật) hoặc một bổ ngữ liệt kê trong
  [`tools/tag_vocab.txt`](tools/tag_vocab.txt);
- ít nhất một tag là id kỹ thuật (kỹ thuật cốt lõi lời giải dùng);
- chỉ gắn kỹ thuật **thực sự** giải được bài đó (kể cả cách thay thế hợp lệ), không gắn thứ chỉ được nhắc.

Kiểm: `python3 cp_library/tools/check_cses_tags.py`.

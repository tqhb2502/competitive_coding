# Thêm snippet vào CP Library

Mỗi kỹ thuật là **một file `.hpp`** trong nhóm phù hợp (`01_foundations` … `10_advanced_techniques`),
đặt tên `kebab-case` (ví dụ `segment-tree.hpp`). Mục tiêu: tìm nhanh, dán nhanh, gộp một file để nộp.

## Khuôn một snippet

```cpp
#pragma once
#include <bits/stdc++.h>
using namespace std;
// <Tên> — <mục đích một dòng>.
// Khi dùng: <dấu hiệu đề / bài toán>
// ĐPT: build O(...), update O(...), query O(...); bộ nhớ O(...)
// Dùng: <1-3 dòng ví dụ gọi API>
// Bẫy: <index base, overflow, identity...>
// CSES: —          (sinh tự động bởi tools/link_cses.py từ các dòng Tags)
struct TenDuyNhat { /* ... */ };

#ifdef CP_DEMO       // g++ -std=c++17 -DCP_DEMO -x c++ <ten-file>.hpp -o demo && ./demo
int main() {
    // ví dụ nhỏ, chạy được, in kết quả dễ hiểu để minh hoạ API
    return 0;
}
#endif
```

Quy ước:

- Tự chứa, biên dịch độc lập. Identifier tiếng Anh, comment tiếng Việt.
- **Tên type/hàm duy nhất trên toàn kho** để bundle nhiều snippet không đụng nhau; tránh hàm free tên chung
  (`gcd`, `Edge`, `Node`…).
- Ghi rõ index 0/1-based, kiểu số / overflow, identity của phép gộp.
- Không cần `meta.json` / test / README riêng — comment header chính là tài liệu.
- Ví dụ minh hoạ đặt trong `#ifdef CP_DEMO int main(){...}#endif` cuối file — **đừng để `main` trần**
  (bundle nhiều snippet sẽ nhân đôi `main`). Dòng `// CSES:` do `link_cses.py` sinh, đừng sửa tay.

## Sau khi thêm

1. Cập nhật bảng tra cứu trong [README.md](README.md); nếu hợp, thêm vào các index trong [indexes/](indexes/).
2. Chạy kiểm chứng (cần **git bash** + `g++` trên PATH):

   ```sh
   export PATH="$PATH:$HOME/Downloads/w64devkit/bin"
   bash tools/verify.sh
   ```

   Nó biên dịch mọi `.hpp`, chạy template, kiểm luồng bundle và kiểm tag CSES.
3. Gắn `Tags:` cho các bài CSES dùng kỹ thuật này (xem dưới).
4. Cập nhật link ngược trong các `.hpp`: `python3 tools/link_cses.py`.

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
- chỉ gắn kỹ thuật lời giải **thực sự** dùng, không gắn phương án chỉ được nhắc trong tài liệu.

Kiểm: `python3 tools/check_cses_tags.py`.

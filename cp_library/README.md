# CP Library — kho snippet thi đấu

Kho code C++17 để dùng **khi đang thi**: tìm nhanh theo dấu hiệu đề, dán vào lời giải,
và **gộp một file để nộp**. Mỗi kỹ thuật là **một file `.hpp` tự chứa**, đầu file có
comment ngắn: *mục đích · khi nào dùng · độ phức tạp · cách dùng · bẫy*.

## Dùng nhanh khi thi

1. Chép [`templates/base.cpp`](templates/base.cpp) thành `main.cpp` (đã có fast IO + `using namespace std;`).
2. Tìm kỹ thuật:
   - biết tên: `rg -i "fenwick|segment tree" cp_library`
   - biết dạng bài: mở [indexes/by-use-case.md](indexes/by-use-case.md) hoặc [decision-trees.md](indexes/decision-trees.md).
3. `#include "cp_library/02_data_structures/fenwick.hpp"` trong `main.cpp` (hoặc dán thẳng nội dung file).
4. Gộp nộp: `python3 cp_library/tools/bundle.py main.cpp -I . -o submit.cpp` → nộp `submit.cpp`.

`bundle.py` inline mọi `#include "..."` nội bộ, giữ nguyên `#include <...>`, bỏ `#pragma once`
và tự bọc guard — output là một file `.cpp` nộp thẳng được.

## Bảng tra cứu (đang có)

### 01 — Foundations

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| Prefix Sum | tổng đoạn mảng tĩnh, không cập nhật | [prefix-sum.hpp](01_foundations/prefix-sum.hpp) |
| Difference Array | cộng đoạn nhiều lần (offline) rồi đọc kết quả | [difference-array.hpp](01_foundations/difference-array.hpp) |
| Binary Search on Answer | tìm biên vị từ đơn điệu / nhị phân trên đáp án | [binary-search.hpp](01_foundations/binary-search.hpp) |
| Coordinate Compression | nén giá trị lớn về hạng 0..k-1 | [coordinate-compression.hpp](01_foundations/coordinate-compression.hpp) |
| Two Pointers | cặp/đoạn trên mảng đã sắp, hai con trỏ một chiều | [two-pointers.hpp](01_foundations/two-pointers.hpp) |
| Sliding Window | cửa sổ co giãn, điều kiện đơn điệu theo độ dài | [sliding-window.hpp](01_foundations/sliding-window.hpp) |
| Monotonic Stack | phần tử nhỏ/lớn hơn gần nhất, histogram | [monotonic-stack.hpp](01_foundations/monotonic-stack.hpp) |
| Monotonic Deque | min/max mọi cửa sổ độ dài k | [monotonic-deque.hpp](01_foundations/monotonic-deque.hpp) |
| Submask Enumeration | duyệt submask, SOS DP, cặp bù | [submask-enumeration.hpp](01_foundations/submask-enumeration.hpp) |

### 02 — Data Structures

| Kỹ thuật | Khi nào dùng | File |
|---|---|---|
| DSU (Union-Find) | hợp nhất / cùng thành phần (chỉ thêm cạnh), Kruskal | [dsu.hpp](02_data_structures/dsu.hpp) |
| Rollback DSU | DSU có undo (dynamic connectivity offline) | [rollback-dsu.hpp](02_data_structures/rollback-dsu.hpp) |
| Fenwick Tree | update điểm + tổng đoạn | [fenwick.hpp](02_data_structures/fenwick.hpp) |
| Fenwick Tree 2D | update điểm + tổng hình chữ nhật trên lưới | [fenwick-2d.hpp](02_data_structures/fenwick-2d.hpp) |
| Sparse Table | min/max/gcd đoạn tĩnh, query O(1) | [sparse-table.hpp](02_data_structures/sparse-table.hpp) |
| Disjoint Sparse Table | phép kết hợp bất kỳ (gcd/product), query O(1) | [disjoint-sparse-table.hpp](02_data_structures/disjoint-sparse-table.hpp) |
| Segment Tree (iterative) | update điểm + truy vấn đoạn theo monoid | [segment-tree.hpp](02_data_structures/segment-tree.hpp) |
| Lazy Segment Tree | update đoạn + truy vấn đoạn (mẫu: add/sum) | [lazy-segment-tree.hpp](02_data_structures/lazy-segment-tree.hpp) |
| Trie | đếm từ theo tiền tố (bảng chữ cái) | [trie.hpp](02_data_structures/trie.hpp) |
| Binary Trie | max XOR / truy vấn theo bit | [binary-trie.hpp](02_data_structures/binary-trie.hpp) |

Các nhóm **03 Graphs … 10 Advanced Techniques** sẽ được bổ sung dần theo cùng quy ước.

## Tra ngược từ bài CSES

Mỗi bài trong [`cses/`](../cses) có dòng `Tags:` gần đầu `idea.txt` liệt kê kỹ thuật lời giải dùng:

```sh
rg -l '^Tags: .*\bfenwick\b' cses --glob idea.txt   # những bài dùng Fenwick
```

Tên tag trùng tên file `.hpp` (id kỹ thuật) cộng vài bổ ngữ trong
[`tools/tag_vocab.txt`](tools/tag_vocab.txt).

## Cấu trúc

```text
cp_library/
├── 01_foundations/ … 10_advanced_techniques/   # mỗi kỹ thuật một .hpp
├── indexes/     # tra theo dấu hiệu đề: by-use-case, by-complexity, decision-trees, pitfalls
├── templates/   # base / graph / geometry / interactive — starter thi đấu
└── tools/       # bundle.py (gộp 1 file), verify.sh, check_cses_tags.py, tag_vocab.txt
```

## Quy ước snippet

- `#pragma once` + `#include <bits/stdc++.h>` + `using namespace std;` — tự chứa, dán là chạy.
- Tên type/hàm **duy nhất & rõ nghĩa** (DSU, Fenwick, SegTree…) để bundle nhiều file không đụng nhau;
  tránh hàm free tên chung (`gcd`, `Edge`…).
- Comment header 5 dòng: *mục đích · khi dùng · độ phức tạp · cách dùng · bẫy*.
  Identifier tiếng Anh, comment tiếng Việt.
- Ghi rõ **index 0-based/1-based**, kiểu số / overflow, và identity của phép gộp.

## Kiểm chứng

```sh
export PATH="$PATH:$HOME/Downloads/w64devkit/bin"   # để tìm g++ (Windows)
bash cp_library/tools/verify.sh
```

`verify.sh` biên dịch độc lập mọi `.hpp`, chạy các template, kiểm luồng bundle-1-file, và
chạy `check_cses_tags.py`. Cần **bash thật** (git bash) — bash rút gọn của w64devkit không đủ.
Không có unit test / sanitizer; đúng đắn dựa vào kiểm mắt + đối chiếu với lời giải trong `cses/`.

## Thêm snippet mới

Xem [CONTRIBUTING.md](CONTRIBUTING.md).

# CP Library

Thư viện C++17 dành cho lập trình thi đấu, tập trung vào ba mục tiêu:

- tìm đúng kỹ thuật nhanh từ dấu hiệu của đề bài;
- lấy được một cài đặt nhỏ, tự chứa và dễ ghép vào lời giải;
- biết rõ điều kiện áp dụng, độ phức tạp và các trường hợp dễ sai.

Mỗi kỹ thuật là một **entry** độc lập. Entry không chỉ chứa code mà còn có giải
thích, ví dụ, kiểm thử và liên kết tới các bài đã áp dụng kỹ thuật đó.

## Bắt đầu nhanh

Nếu đã biết tên kỹ thuật, tìm trực tiếp bằng `rg`:

```sh
rg -n -i "dijkstra|shortest path" cp_library
rg -n '"status": "stress-tested"' cp_library
```

Nếu mới chỉ biết dạng bài, bắt đầu từ các chỉ mục:

- [Theo mục đích sử dụng](indexes/by-use-case.md)
- [Theo độ phức tạp](indexes/by-complexity.md)
- [Cây quyết định chọn thuật toán](indexes/decision-trees.md)
- [Các bẫy thường gặp](indexes/pitfalls.md)

Các chỉ mục sinh tự động từ metadata:

- [Toàn bộ catalog](indexes/catalog.md)
- [Theo tag](indexes/by-tag.md)
- [Theo trạng thái kiểm chứng](indexes/by-status.md)
- [Theo bài luyện tập](indexes/by-practice.md)

Hoặc dùng công cụ tìm kiếm metadata:

```sh
cd cp_library
python3 tools/build_index.py
python3 tools/search.py --help
python3 tools/search.py "shortest path"
```

## Cấu trúc thư viện

```text
cp_library/
├── 01_foundations/
├── 02_data_structures/
├── 03_graphs/
├── 04_trees/
├── 05_dynamic_programming/
├── 06_strings/
├── 07_mathematics/
├── 08_geometry/
├── 09_offline_algorithms/
├── 10_advanced_techniques/
├── indexes/
├── templates/
├── _templates/entry/
└── tools/
```

Mỗi thư mục con của một nhóm là một entry có kiến trúc:

```text
<entry-id>/
├── meta.json       # metadata để tìm kiếm, lập chỉ mục và kiểm tra
├── README.md       # điều kiện dùng, ý tưởng, chứng minh và các bẫy
├── algorithm.hpp   # cài đặt C++17 tự chứa
├── example.cpp     # ví dụ sử dụng tối thiểu
└── test.cpp        # unit test, oracle hoặc stress test
```

Tên file header không bắt buộc là `algorithm.hpp`; trường `source` trong
`meta.json` là nguồn chính xác để các công cụ tìm file.

## Mười nhóm kỹ thuật

| Nhóm | Nội dung chính |
|---|---|
| [01 Foundations](01_foundations/README.md) | Mẫu cơ bản: prefix sum, two pointers, binary search, nén tọa độ |
| [02 Data Structures](02_data_structures/README.md) | Fenwick, segment tree, DSU, heap và cấu trúc truy vấn |
| [03 Graphs](03_graphs/README.md) | Duyệt đồ thị, đường đi, SCC, MST, flow và matching |
| [04 Trees](04_trees/README.md) | LCA, Euler tour, HLD, centroid và kỹ thuật trên cây |
| [05 Dynamic Programming](05_dynamic_programming/README.md) | Thiết kế trạng thái và các tối ưu DP |
| [06 Strings](06_strings/README.md) | Matching, hashing, trie, suffix structure và automaton |
| [07 Mathematics](07_mathematics/README.md) | Số học, tổ hợp, đại số và xác suất |
| [08 Geometry](08_geometry/README.md) | Hình học nguyên chính xác, sweep và bao lồi |
| [09 Offline Algorithms](09_offline_algorithms/README.md) | Mo, CDQ, rollback và xử lý truy vấn offline |
| [10 Advanced Techniques](10_advanced_techniques/README.md) | Kỹ thuật nâng cao, biến đổi nhanh và tối ưu chuyên biệt |

Khi một kỹ thuật có thể thuộc nhiều nhóm, chọn nhóm mô tả **ý tưởng cốt lõi**
của cài đặt. Thêm các khái niệm còn lại vào `tags` và `aliases` để tìm kiếm
chéo nhóm.

## Metadata và mức độ tin cậy

`meta.json` dùng schema đóng phiên bản 1 và chỉ gồm các trường sau:

- `schema_version`, `id`, `title`, `category`, `summary`;
- `aliases`, `tags`;
- `status`: `draft`, `reviewed`, `verified` hoặc `stress-tested`;
- `source`: đường dẫn tương đối tới header chính;
- `complexities`: object ánh xạ tên thao tác sang mô tả độ phức tạp;
- `tests`: danh sách đường dẫn tương đối tới các file test, luôn gồm
  `test.cpp`;
- `practice`: danh sách `{title, path}` trỏ tới bài luyện tập trong repository.

Các trường lạ sẽ bị validator từ chối để lỗi chính tả trong metadata không bị
bỏ qua. Muốn bổ sung trường mới, cần tăng phiên bản schema rồi cập nhật đồng bộ
validator, công cụ sinh chỉ mục và tài liệu.

Ý nghĩa của `status`:

| Trạng thái | Bảo đảm tối thiểu |
|---|---|
| `draft` | Có khung và ý tưởng, chưa nên dùng nguyên trạng khi thi |
| `reviewed` | Đã đọc soát cài đặt, điều kiện áp dụng và chứng minh |
| `verified` | Đã biên dịch và qua bộ test xác định có oracle phù hợp |
| `stress-tested` | Đã đối chiếu ngẫu nhiên/đối kháng với lời giải độc lập |

Status không thay thế việc đọc điều kiện áp dụng. Một entry
`stress-tested` vẫn có thể sai nếu đầu vào của bài nằm ngoài giả thiết ghi trong
README.

## Công cụ

Chạy các lệnh sau từ thư mục `cp_library`:

```sh
# Tìm entry theo id, tên, alias, tag hoặc mô tả
python3 tools/search.py "range minimum"

# Sinh lại các chỉ mục từ meta.json
python3 tools/build_index.py

# Kiểm tra format và từ vựng tag trong mọi CSES idea.txt
python3 tools/check_cses_tags.py

# Ghép mọi quoted include local thành một translation unit
python3 tools/bundle.py --help
python3 tools/bundle.py _templates/entry/example.cpp \
  -o /tmp/example-bundled.cpp

# Kiểm tra tool, schema, template và toàn bộ example/test
bash tools/verify_all.sh
```

`bundle.py` chỉ inline các `#include "..."` local; header chuẩn như
`#include <vector>` được giữ nguyên. Mỗi header local được bọc bằng include
guard tổng hợp, nên include trong nhánh `#if` vẫn giữ đúng thứ tự xử lý. Công cụ
coi header local có ngữ nghĩa “include một lần”, phù hợp với quy ước header của
thư viện nhưng không dành cho X-macro header cố ý include nhiều lần. Luôn xem
`--help` trước khi dùng các tùy chọn nâng cao.

Tag của bài CSES nằm trên một dòng `Tags:` gần đầu `idea.txt`. Tên tag dùng
lowercase `kebab-case` và phải xuất hiện dưới dạng `id` hoặc tag của một entry
trong catalog. Mỗi danh sách luôn chứa ít nhất một `id` entry để chỉ rõ kỹ thuật
cốt lõi; chỉ thêm modifier giúp phân biệt cách áp dụng. Tránh các từ quá chung
hoặc chỉ mô tả API/container như `array`, `callback`, `counting`, `monotonic`
nếu chúng không làm kết quả tìm kiếm hữu ích hơn. Nhờ vậy có thể tìm từ bài
sang kiến thức bằng cùng một từ khóa.

`check_cses_tags.py` chỉ xác nhận format, vị trí và từ vựng. Công cụ không thể
kết luận tag đã đầy đủ hoặc đúng với ngữ nghĩa lời giải; phần đó vẫn cần đọc
`idea.txt` và `solution.cpp`.

Tìm mọi bài CSES đã dùng một kỹ thuật:

```sh
rg -l '^Tags: .*prefix-sum' cses --glob idea.txt
```

## Quy trình dùng trong một bài thi

1. Nhận diện ràng buộc và loại thao tác của đề.
2. Tra [cây quyết định](indexes/decision-trees.md) hoặc tìm bằng `search.py`.
3. Đọc phần “Khi nào dùng” và “Bẫy thường gặp” của entry.
4. Mở `example.cpp`, rồi include header của entry trong lời giải.
5. Kiểm lại kiểu số, indexing, trạng thái giữa nhiều test và giả thiết của API.
6. Chạy sample cùng ít nhất một test biên; dùng `bundle.py` trên file lời giải
   nếu hệ thống nộp bài yêu cầu một file duy nhất.

## Đóng góp

Xem [CONTRIBUTING.md](CONTRIBUTING.md) để biết cách tạo entry, điền metadata,
viết test và nâng `status`. Mẫu khởi đầu nằm tại
[`_templates/entry`](_templates/entry).

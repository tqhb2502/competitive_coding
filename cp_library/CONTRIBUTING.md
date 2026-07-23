# Đóng góp cho CP Library

Mỗi thay đổi nên làm cho một kỹ thuật dễ **tìm**, dễ **dùng đúng** và dễ
**kiểm chứng** hơn. Một đoạn code ngắn nhưng thiếu điều kiện áp dụng hoặc test
biên vẫn được xem là entry chưa hoàn chỉnh.

## Quy trình thêm entry

1. Tìm theo tên, alias và tag để tránh tạo bản trùng:

   ```sh
   rg -n -i "tên kỹ thuật|alias" cp_library
   python3 cp_library/tools/search.py "tên kỹ thuật"
   ```

2. Chọn đúng nhóm theo ý tưởng cốt lõi của thuật toán.
3. Từ root repository, sao chép toàn bộ `cp_library/_templates/entry` thành
   thư mục có tên `kebab-case` trong nhóm đã chọn.
4. Hoàn thiện đủ năm thành phần:

   ```text
   meta.json
   README.md
   <source>.hpp
   example.cpp
   test.cpp
   ```

5. Chạy kiểm tra entry, sinh lại chỉ mục và xem diff trước khi gửi thay đổi.

Không nâng `status` chỉ vì code giống một nguồn quen thuộc. Mức tin cậy phải
dựa trên những kiểm tra thực sự có trong entry.

## Chọn nhóm

- `01_foundations`: mẫu nền tảng, không phụ thuộc cấu trúc chuyên biệt.
- `02_data_structures`: API lưu trữ, cập nhật hoặc truy vấn là trọng tâm.
- `03_graphs`: đối tượng chính là đồ thị tổng quát.
- `04_trees`: khai thác cấu trúc cây hoặc forest.
- `05_dynamic_programming`: trạng thái và chuyển trạng thái là ý tưởng chính.
- `06_strings`: dữ liệu và phép toán cốt lõi là chuỗi.
- `07_mathematics`: số học, tổ hợp, đại số, xác suất.
- `08_geometry`: điểm, vector, đường, đa giác hoặc không gian hình học.
- `09_offline_algorithms`: được phép đổi thứ tự truy vấn/sự kiện là mấu chốt.
- `10_advanced_techniques`: kỹ thuật chuyên sâu không có nhóm tự nhiên hơn.

Nếu phân vân giữa hai nhóm, đặt entry tại nhóm của cài đặt tái sử dụng chính,
sau đó thêm nhóm còn lại vào `tags`.

## Quy ước `meta.json`

Schema hiện tại có `schema_version = 1`:

```json
{
  "schema_version": 1,
  "id": "example-entry",
  "title": "Example Entry",
  "category": "01_foundations",
  "summary": "Một câu mô tả ngắn về bài toán entry giải quyết.",
  "aliases": ["alternative name"],
  "tags": ["example", "foundation"],
  "status": "draft",
  "source": "algorithm.hpp",
  "complexities": {
    "time": "O(n)",
    "memory": "O(1)"
  },
  "tests": ["test.cpp"],
  "practice": [
    {
      "title": "CSES Missing Number",
      "path": "cses/01_introductory/1083_missing-number"
    }
  ]
}
```

Đây là schema đóng: mỗi object phải chứa đúng các trường được mô tả bên dưới.
Validator sẽ từ chối trường lạ để bắt các lỗi chính tả và metadata chưa được
công cụ hỗ trợ.

Quy tắc cho từng trường:

- `id`: duy nhất trong toàn thư viện, dùng `kebab-case` và trùng tên thư mục.
- `title`: tên dễ đọc, ưu tiên tên thông dụng trong tài liệu thuật toán.
- `category`: đúng tên một trong mười thư mục nhóm.
- `summary`: một câu tự đủ nghĩa; không lặp lại nguyên tiêu đề.
- `aliases`: các tên tìm kiếm thay thế; không được trùng với `id` hoặc alias của
  bất kỳ entry nào khi so sánh không phân biệt hoa thường.
- `tags`: mảng slug `kebab-case` viết thường; thêm các khái niệm tiếng Anh
  thường được dùng khi tìm kiếm.
- `status`: chỉ nhận `draft`, `reviewed`, `verified`, `stress-tested`.
- `source`: đường dẫn tương đối từ thư mục entry, phải nằm bên trong entry và
  trỏ tới file `.hpp`.
- `complexities`: object không rỗng, mọi key và value đều là chuỗi không rỗng.
  Dùng key theo thao tác thật như `build`, `query`, `update`, `time`, `memory`;
  ghi rõ `amortized` hoặc `expected` nếu có.
- `tests`: mảng đường dẫn tương đối từ thư mục entry tới file `.cpp`; luôn gồm
  `test.cpp`, có thể thêm test khác, và mọi đường dẫn phải nằm bên trong entry
  rồi tồn tại.
- `practice`: mỗi phần tử là object chứa đúng hai trường `{title, path}`;
  `path` tính từ root repository, ví dụ
  `cses/05_range_queries/1648_dynamic-range-sum-queries`.

Không dùng đường dẫn tuyệt đối, `..` để thoát khỏi entry cho `source`/`tests`,
hoặc URL ngoài repository trong `practice.path`.

## Mức độ kiểm chứng

### `draft`

- Metadata hợp lệ.
- README nêu rõ mục tiêu và điều kiện áp dụng.
- Source, example và test có thể còn đang hoàn thiện.

### `reviewed`

- Header biên dịch độc lập trong C++17.
- Đã kiểm tra bằng tay invariant, độ phức tạp, overflow và trường hợp biên.
- Example phản ánh API thật; README khớp với source.

### `verified`

- Example và mọi file trong `tests` biên dịch với warning nghiêm ngặt.
- Có test cho input nhỏ nhất, rỗng nếu API cho phép, giá trị trùng và biên số.
- Kết quả được so với oracle, brute force hoặc đáp án tính độc lập khi phù hợp.

### `stress-tested`

- Có randomized differential test hoặc test đối kháng có khả năng bắt lỗi logic.
- Seed có thể tái hiện; thất bại in đủ dữ liệu để replay.
- Đã chạy ở giới hạn đủ lớn để kiểm tra độ phức tạp và kiểu số.

## Viết `README.md` của entry

Dùng tiếng Việt có dấu, câu ngắn và công thức có khoảng trắng dễ đọc. Giữ các
phần sau nếu phù hợp:

- bài toán entry giải quyết và tín hiệu nhận biết;
- điều kiện áp dụng, input không hỗ trợ;
- API của header;
- ý tưởng và invariant/chứng minh;
- bảng độ phức tạp;
- ví dụ sử dụng;
- bẫy thường gặp;
- chiến lược kiểm thử;
- bài luyện tập trong repository.

Không sao chép nguyên đề bài hoặc viết một chứng minh chung chung kiểu “hiển
nhiên đúng”. Chứng minh nên gắn từng invariant với đúng đoạn cài đặt.

## Viết header C++17

- Chỉ include các header chuẩn thật sự cần.
- Header phải tự chứa: một file chỉ include header này vẫn biên dịch.
- Dùng include guard ổn định.
- Đặt symbol trong namespace `cp_library`.
- Không dùng `using namespace std;` trong header.
- Tránh macro, biến global có trạng thái và `#pragma` phụ thuộc máy.
- API dùng tên tiếng Anh mô tả rõ; comment tiếng Việt giải thích invariant và
  lý do, không diễn giải lại từng dòng.
- Ghi rõ indexing, ownership, mutability, sentinel và hành vi với input rỗng.
- Dùng kiểu số đủ rộng; nếu người gọi phải chọn kiểu, nêu giới hạn trong README.
- Một instance phải dùng lại an toàn theo đúng hợp đồng; nếu cần `clear`, cung
  cấp và kiểm thử nó.

## Viết `example.cpp`

Example là đoạn nhỏ nhất cho thấy cách dùng đúng:

- include source bằng đường dẫn tương đối trong entry;
- tạo một input cụ thể;
- gọi API chính;
- in hoặc kiểm tra một kết quả có thể hiểu ngay;
- không chứa parser dài hay một lời giải bài thi hoàn chỉnh.

## Viết `test.cpp`

Test phải xác định và chạy không cần input ngoài. Tối thiểu nên có:

- trường hợp nhỏ nhất;
- trường hợp điển hình;
- biên làm lộ lỗi indexing, overflow hoặc phần tử trùng;
- nhiều lần gọi trên cùng instance nếu API cho phép.

Với thuật toán khó, ưu tiên oracle đơn giản cho `n` nhỏ rồi random nhiều seed.
Đừng dùng chính thuật toán cần test làm oracle.

## Kiểm tra trước khi hoàn tất

Chạy từ root repository:

```sh
python3 cp_library/tools/build_index.py
bash cp_library/tools/verify_all.sh
git add -N cp_library/<category>/<entry-id>
git diff --check
git status --short
```

Sau khi sinh chỉ mục, chỉ giữ các thay đổi được tạo từ metadata hiện tại. Không
sửa tay phần được công cụ đánh dấu là generated. `git add -N` chỉ đánh dấu
intent-to-add để `git diff` nhìn thấy file mới; lệnh này không đưa nội dung file
vào staging area.

Checklist cuối:

- metadata và README nói cùng một độ phức tạp;
- `source`/`tests` không thoát khỏi thư mục entry;
- mọi practice path tồn tại từ root repository;
- example và test dùng đúng API công khai;
- không có file nhị phân, output test hoặc dữ liệu tạm;
- không thay đổi entry ngoài phạm vi của đóng góp.

## Gắn tag cho bài CSES

Sau khi hoàn tất một nhóm kiến thức và sinh lại catalog, quét toàn bộ
`cses/**/idea.txt` để tìm những lời giải thực sự dùng các kỹ thuật của nhóm.
Thêm một dòng sau URL và trước phần `## Đề bài`:

```text
Tags: binary-search, monotone-predicate, search-on-answer
```

Quy ước:

- tag viết bằng lowercase `kebab-case`;
- tag phải là `id` hoặc một tag đã có trong metadata của catalog;
- mỗi bài đã gắn tag phải có ít nhất một `id` entry làm kỹ thuật cốt lõi;
- danh sách tăng dần theo từ điển, không trùng và phân cách đúng bằng `, `;
- merge với tag cũ, không ghi đè tag của các nhóm đã hoàn thành;
- chỉ gắn kỹ thuật lời giải đang dùng, không gắn một phương án thay thế chỉ
  được nhắc trong tài liệu;
- không gắn `binary-search` chỉ vì một lệnh `lower_bound` phụ trợ nếu tìm kiếm
  nhị phân không phải bước thuật toán có ý nghĩa;
- chỉ thêm modifier giúp phân biệt cách áp dụng; tránh tag quá chung hoặc chỉ
  mô tả API/container như `array`, `callback`, `counting`, `monotonic` nếu
  chúng không cải thiện việc tìm kiếm.

Chạy validator từ root repository:

```sh
python3 cp_library/tools/check_cses_tags.py
```

`verify_all.sh` cũng chạy kiểm tra này. Validator chỉ kiểm tra format, vị trí và
từ vựng, không chứng minh độ đầy đủ hoặc đúng ngữ nghĩa. Một `idea.txt` chưa có
tag vẫn hợp lệ trong giai đoạn thư viện chưa bao phủ kỹ thuật chính của bài.

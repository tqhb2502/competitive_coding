# Các bẫy thường gặp

Trang này là checklist chống WA, RE và TLE. Không cần đọc toàn bộ trước mỗi bài;
chọn đúng mục liên quan tới thuật toán đang dùng.

## Trước khi code

- Viết lại miền giá trị lớn nhất của mọi tổng, tích và số đường đi.
- Xác định rõ indexing: input 1-based, source/API 0-based hay ngược lại.
- Phân biệt đoạn đóng `[l, r]` với nửa mở `[l, r)`.
- Ghi giả thiết của thuật toán: trọng số không âm, đồ thị liên thông, modulo
  nguyên tố, phép toán associative, query offline...
- Tính tổng số thao tác qua **mọi** test, không chỉ một test lớn nhất.

## Số nguyên và overflow

- `int * int` overflow trước khi được gán vào `long long`. Cast một toán hạng
  trước phép nhân.
- `INF + weight` có thể overflow. Chỉ cộng khi trạng thái hữu hạn hoặc chọn
  sentinel còn khoảng an toàn.
- LCM nên tính `a / gcd(a, b) * b`, vẫn phải kiểm miền kết quả.
- Cross product của tọa độ tới `1e9` thường cần `long long`; hiệu/tích lớn hơn
  có thể cần `__int128`.
- `size_t` là unsigned: biểu thức `size() - 1` lỗi khi container rỗng.
- `abs(LLONG_MIN)` không biểu diễn được.
- Shift `1 << bit` dùng `int`; dùng `1LL`, `1ULL` và bảo đảm `bit` nhỏ hơn độ
  rộng kiểu.

## Modulo

- Chuẩn hóa số âm bằng `(value % mod + mod) % mod` hoặc một hàm tương đương.
- Không chia modulo bằng phép chia nguyên.
- Fermat chỉ áp dụng khi modulo nguyên tố và mẫu khác 0 modulo.
- `a * b % mod` có thể overflow `long long` nếu modulo rất lớn.
- Khi trừ nhiều lần, chuẩn hóa đủ để giá trị không âm.
- Đáp án đếm có thể cần modulo nhưng chỉ số/tần suất trung gian vẫn cần kiểu
  riêng.

## Indexing và biên

- Binary search: xác định invariant của `lo`, `hi` trước vòng lặp.
- Prefix sum kích thước `n + 1` giúp đoạn `[l, r)` là `prefix[r]-prefix[l]`.
- Fenwick thường nhận index từ 1; wrapper 0-based phải thống nhất ở cả update
  và query.
- Segment tree cần biết query rỗng trả identity nào.
- Với circular array, modulo của số âm trong C++ vẫn âm.
- Test bắt buộc: `n=0` nếu hợp lệ, `n=1`, toàn bằng nhau, tăng dần, giảm dần,
  query chạm hai đầu.

## Sorting và comparator

- Comparator phải là strict weak ordering; không dùng `<=`.
- Không dùng epsilon trong comparator của `sort`/`set`.
- Khi event cùng tọa độ, thứ tự add/query/remove quyết định đoạn đóng hay mở.
- Sort pair mặc định so cả hai trường; kiểm tie-break có đúng không.
- Sau coordinate compression, giữ duplicate nếu chúng đại diện cho nhiều phần
  tử, chỉ unique miền tọa độ.

## Container và iterator

- Không gọi `top`, `front`, `back` hoặc dereference iterator khi rỗng/end.
- `map[key]` tự chèn key; dùng `find`/`contains` nếu chỉ muốn kiểm tra.
- `erase(value)` và `erase(iterator)` có semantics khác nhau.
- Insert/erase có thể invalidate iterator/reference.
- `vector<bool>` là specialization proxy, không phải mảng `bool` thông thường.
- `unordered_map` có worst-case xấu; reserve khi số key lớn và cân nhắc input
  đối kháng.

## Nhiều test và trạng thái global

- Reset toàn bộ phần đã dùng của adjacency, frequency, visited và pool node.
- Timestamp tránh `O(MAX)` clear nhưng timestamp cũng có thể overflow trong
  chương trình rất dài.
- Không giữ capacity/heap cũ nếu bộ nhớ cộng dồn qua test.
- Random seed cố định khi debug; in seed khi stress fail.
- Static local trong header tồn tại qua nhiều lần gọi và có thể phá tính độc lập.

## DFS, BFS và đồ thị

- DFS đệ quy có thể tràn stack ở cây đường dài; dùng traversal lặp khi `n` lớn.
- Với cạnh vô hướng, thêm cả hai chiều nhưng chỉ gắn cùng edge id khi low-link.
- Dijkstra không đúng với cạnh âm.
- BFS nhiều nguồn phải đưa mọi nguồn vào queue ở khoảng cách 0 trước khi chạy.
- Đồ thị có thể rời rạc; vòng ngoài phải bắt đầu traversal từ mọi đỉnh cần thiết.
- Topological order có ít hơn `n` đỉnh nghĩa là có chu trình.
- Khi reconstruct path, phân biệt “chưa thăm” với parent của nguồn.
- Floyd–Warshall: chặn phép cộng hai `INF`.

## DSU

- `find` trước khi đọc size hoặc so root.
- Union by size/rank phải cập nhật size tại root mới.
- DSU thường không hỗ trợ xóa cạnh.
- Rollback DSU không dùng path compression trừ khi log đầy đủ mọi thay đổi.
- Với offline dynamic connectivity, interval hoạt động của cạnh phải thống nhất
  là đóng hay nửa mở.

## Cây

- Cây input vẫn có thể là forest nếu đề không bảo đảm liên thông.
- Parent của root cần sentinel không bị dùng như index.
- LCA binary lifting cần đủ số tầng cho độ sâu lớn nhất.
- Euler `tin/tout` phải thống nhất mỗi đỉnh xuất hiện một lần hay hai lần.
- HLD trên cạnh và trên đỉnh khác nhau ở việc loại LCA khỏi một đoạn.
- Rerooting cần tách đóng góp từ con hiện tại; prefix/suffix aggregate phải có
  identity đúng.
- Centroid decomposition phải reset dữ liệu khoảng cách theo component phù hợp.

## Dynamic programming

- State chưa tính và state có đáp án 0 không được dùng cùng sentinel.
- Kiểm thứ tự vòng lặp của knapsack: giảm dần cho 0/1, tăng dần cho unbounded.
- Rolling array chỉ hợp lệ nếu transition không cần lớp đã ghi đè.
- `-INF + value` gây overflow tương tự `INF + value`.
- Reconstruction cần lưu đủ parent hoặc có kế hoạch tính lại.
- Với bitmask, số state là `1 << n`; cast đúng kiểu và kiểm memory trước.
- Tối ưu divide-and-conquer/Knuth cần chứng minh điều kiện, không suy ra từ vài
  test nhỏ.

## Greedy

- “Chọn tốt nhất hiện tại” phải đi kèm exchange argument hoặc invariant.
- Tie-break có thể ảnh hưởng tính khả thi dù không ảnh hưởng giá trị local.
- Sort theo endpoint, start, duration hoặc ratio dẫn tới các greedy khác nhau.
- Priority queue lazy deletion phải loại hết phần tử stale ở đỉnh, không chỉ một.
- Kiểm counterexample nhỏ bằng brute force trước khi tin một quy tắc greedy.

## Chuỗi

- Prefix function, Z và suffix array có quy ước chỉ số khác nhau.
- KMP sau một match phải fallback để tìm match chồng lấn.
- Alphabet không luôn là 26 chữ thường.
- Sentinel ghép hai chuỗi phải không xuất hiện trong input.
- Rolling hash có va chạm; double hash vẫn là xác suất.
- Phép nhân hash/modulo cần kiểu đủ rộng.
- Suffix array/LCP query phải xử lý hai suffix giống nhau và đoạn rỗng.
- UTF-8 không đồng nghĩa một byte là một ký tự; bài thi thường nói rõ alphabet.

## Hình học

- Cross bằng 0 cần xử lý collinear riêng.
- Giao đoạn gồm cả endpoint khác với giao proper.
- Polygon có thể cho theo chiều kim đồng hồ; diện tích có dấu.
- Point-in-polygon phải quy định điểm trên biên.
- Sort góc cần tách nửa mặt phẳng trước khi dùng cross.
- Không trộn `double` comparator với container ordered.
- Epsilon phải phụ thuộc ý nghĩa đại lượng; số nguyên chính xác tốt hơn nếu đủ.

## Cấu trúc dữ liệu

- Identity của sum là 0, min là `INF`, max là `-INF`; không dùng một identity cho
  mọi monoid.
- Lazy tag “không có update” phải khác tag gán giá trị 0.
- Khi có cả assign và add, thứ tự compose tag rất quan trọng.
- Sparse table chỉ cho query chồng lấn khi phép toán idempotent.
- Fenwick min/max thường không hỗ trợ update đảo ngược tùy ý.
- Persistent node pool phải đủ số node theo `updates * log n`.
- Li Chao cần thống nhất miền nguyên/rời rạc và tránh overflow khi tính đường
  thẳng.

## Offline algorithms

- Lưu query id để trả output theo thứ tự gốc.
- Mo cần hàm add/remove là nghịch đảo đúng; update theo thời gian cần thêm bước
  apply/rollback.
- CDQ phải tránh đếm một cặp hai lần ở ranh giới.
- Sweep line cần nén đúng tọa độ query lẫn update.
- Parallel binary search chỉ dùng khi predicate đơn điệu cho từng query.
- Rollback phải đưa stack về đúng snapshot, kể cả nhánh không tạo merge.

## Hiệu năng và TLE

- `O(n log n)` với `map` node-based khác xa Fenwick trên mảng.
- Tránh allocation trong vòng lặp nóng; reserve khi biết kích thước.
- Duyệt dữ liệu theo layout liên tục để giảm cache miss.
- `endl` flush; dùng `'\n'` nếu không cần tương tác.
- Không copy vector lớn ngoài ý muốn trong range loop/lambda.
- Complexity đúng nhưng hằng số lớn vẫn cần benchmark input đối kháng.
- Đừng dùng `#pragma` để che một thuật toán sai cấp độ phức tạp.

## Header và bundle

- Header phải include đủ dependency của chính nó.
- Không có `using namespace std;` trong header.
- Include guard phải duy nhất.
- Function/variable định nghĩa trong header cần `inline`, template hoặc linkage
  phù hợp để tránh multiple definition.
- Không để main, parser đề bài hoặc output debug trong source `.hpp`.
- Sau bundle, kiểm tra trùng tên symbol/macro với lời giải.
- API, README, example, test và metadata phải dùng cùng tên source.

## Checklist 60 giây trước khi nộp

- [ ] Đọc lại đúng format input/output và số test.
- [ ] Kiểm `long long`/`__int128` tại mọi phép nhân và tổng lớn.
- [ ] Kiểm 0-based/1-based và đoạn đóng/nửa mở.
- [ ] Chạy sample, test nhỏ nhất và một test đối kháng.
- [ ] Xóa debug output, file I/O local và assert phụ thuộc input.
- [ ] Ước lượng lại tổng thao tác và bộ nhớ.
- [ ] Xác nhận mọi giả thiết của thuật toán đúng với đề.

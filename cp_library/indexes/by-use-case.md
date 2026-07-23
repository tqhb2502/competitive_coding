# Chỉ mục theo mục đích sử dụng

Trang này dành cho lúc chưa nhớ tên thuật toán nhưng đã nhận ra thao tác hoặc
cấu trúc của đề. Mỗi dòng là một điểm bắt đầu; luôn kiểm lại ràng buộc và giả
thiết trong README của entry trước khi dùng.

## Mảng, đoạn và truy vấn

| Nhu cầu | Kỹ thuật nên xem trước | Nhóm |
|---|---|---|
| Tổng/XOR trên đoạn, không cập nhật | Prefix sum, sparse table nếu phép toán idempotent | Foundations, Data Structures |
| Cập nhật điểm, truy vấn prefix/đoạn | Fenwick tree | Data Structures |
| Cập nhật và truy vấn kết hợp tổng quát | Segment tree | Data Structures |
| Cập nhật cả đoạn | Difference array nếu offline; lazy segment tree nếu online | Foundations, Data Structures |
| Min/max cửa sổ trượt | Monotonic deque | Foundations |
| Phần tử thứ `k`, đếm nhỏ hơn một giá trị | Nén tọa độ + Fenwick/segment tree; persistent tree nếu cần phiên bản | Data Structures |
| Truy vấn tĩnh kết hợp được | Sparse table, disjoint sparse table | Data Structures |
| Nhiều truy vấn không bắt buộc giữ thứ tự | Mo, sweep line, CDQ | Offline Algorithms |
| Tìm biên đầu tiên thỏa tính đơn điệu | Binary search, binary lifting trên cấu trúc | Foundations |

Trước khi chọn cấu trúc, ghi rõ bốn thông tin: dữ liệu tĩnh hay động, cập nhật
điểm hay đoạn, phép toán có nghịch đảo/idempotent không, và truy vấn phải trả
lời online hay có thể sắp lại.

## Sắp xếp, chọn và ghép cặp

| Dấu hiệu | Kỹ thuật |
|---|---|
| Hai đầu tiến một chiều sau khi sort | Two pointers |
| Đếm cặp theo thứ tự hoặc nghịch thế | Merge-sort counting, Fenwick |
| Chọn phần tử thứ `k` | `nth_element`, heap, binary search theo đáp án |
| Ghép cặp tối ưu bằng exchange argument | Sort + greedy |
| Sự kiện mở/đóng theo một tọa độ | Sweep line |
| Giá trị lớn nhưng chỉ thứ tự quan trọng | Coordinate compression |

Với greedy, README cần nêu exchange argument hoặc invariant cụ thể; “chọn phần
tử tốt nhất hiện tại” không phải là chứng minh.

## Đồ thị

| Nhu cầu | Kỹ thuật nên xem trước |
|---|---|
| Reachability, thành phần liên thông | DFS/BFS; DSU nếu chỉ có thêm cạnh |
| Khoảng cách không trọng số | BFS |
| Trọng số `0/1` | 0-1 BFS |
| Trọng số không âm | Dijkstra |
| Có cạnh âm, cần phát hiện chu trình âm | Bellman–Ford |
| Mọi cặp đỉnh, `n` nhỏ | Floyd–Warshall |
| DAG | Topological order + DP |
| Thành phần liên thông mạnh | Kosaraju hoặc Tarjan SCC |
| Cầu, khớp, thành phần hai cạnh | Low-link |
| Cây khung nhỏ nhất | Kruskal/Prim |
| Ghép cặp hai phía | Hopcroft–Karp hoặc flow |
| Công suất/chi phí | Max flow, min-cost flow |
| Thêm/xóa cạnh offline | Segment tree theo thời gian + rollback DSU |

Xem thêm [cây quyết định đường đi và connectivity](decision-trees.md).

## Cây

| Nhu cầu | Kỹ thuật nên xem trước |
|---|---|
| Tổ tiên chung, khoảng cách | Euler tour + binary lifting LCA |
| Truy vấn/cập nhật cây con | Euler flattening + Fenwick/segment tree |
| Truy vấn đường đi | Heavy-light decomposition |
| Tổng thông tin từ con lên cha | Tree DP |
| Đổi gốc đáp án cho mọi đỉnh | Rerooting DP |
| Truy vấn khoảng cách động theo đỉnh | Centroid decomposition |
| Gộp tần suất ở cây con | Small-to-large / DSU on tree |
| Cấu trúc cây thay đổi | Link-cut tree hoặc kỹ thuật offline phù hợp |

Phân biệt rõ “cây là input” với “cây là cấu trúc dữ liệu”. Entry về LCA nằm ở
Trees; segment tree dùng sau Euler tour vẫn nằm ở Data Structures.

## Dynamic programming

| Dấu hiệu | Hướng tiếp cận |
|---|---|
| Chọn/bỏ với tổng hoặc sức chứa nhỏ | Knapsack DP, bitset |
| Tiền tố và trạng thái cuối nhỏ | Linear DP |
| Hai đầu của một đoạn | Interval DP |
| Chữ số rất lớn, ràng buộc theo prefix | Digit DP |
| `n` nhỏ, trạng thái là tập đỉnh | Bitmask DP |
| Bề rộng lưới nhỏ | Profile/plug DP |
| Chuyển `dp[i] = min_j(...)` | CHT, Li Chao, divide-and-conquer hoặc Knuth tùy điều kiện |
| Cần dựng nghiệm | Lưu parent hoặc truy vết lại từ invariant |

Đừng tối ưu một recurrence trước khi xác nhận state không bỏ sót thông tin và
thứ tự tính không tạo phụ thuộc vòng.

## Chuỗi

| Nhu cầu | Kỹ thuật nên xem trước |
|---|---|
| Tìm một pattern | KMP hoặc Z-function |
| Nhiều pattern | Aho–Corasick |
| Prefix/suffix, border, period | Prefix function, Z-function |
| So sánh substring tĩnh | Suffix array/LCP; rolling hash nếu chấp nhận xác suất |
| Đếm/duyệt substring khác nhau | Suffix automaton, suffix array |
| Palindrome | Manacher; palindrome tree nếu cần cấu trúc động |
| Từ điển prefix | Trie |
| Ghép chuỗi bằng phép biến đổi tuyến tính | Polynomial hashing/FFT tùy bài |

Nếu hashing quyết định đúng/sai, ưu tiên cấu trúc xác định hoặc dùng double hash
và ghi rõ xác suất va chạm.

## Toán học

| Nhu cầu | Kỹ thuật nên xem trước |
|---|---|
| GCD, nghiệm phương trình tuyến tính | Euclid mở rộng |
| Đồng dư nhiều modulo | CRT |
| Chia modulo số nguyên tố | Nghịch đảo modular, lũy thừa nhanh |
| Nhiều truy vấn tổ hợp | Factorial + inverse factorial |
| Đếm theo ước/bội | Sieve, divisor transform |
| Bao hàm–loại trừ | Subset enumeration, Möbius inversion |
| Truy hồi tuyến tính | Matrix exponentiation, linear recurrence |
| Hệ phương trình | Gaussian elimination theo trường tương ứng |
| Đa thức/chập | FFT/NTT |

Kiểm tra modulo có nguyên tố không trước khi dùng Fermat hoặc inverse factorial.

## Hình học

| Nhu cầu | Kỹ thuật nên xem trước |
|---|---|
| Hướng quay, thẳng hàng | Cross product |
| Hai đoạn cắt nhau | Orientation + kiểm tra bounding box |
| Diện tích đa giác | Shoelace |
| Điểm trong đa giác | Ray casting/winding |
| Bao lồi | Monotone chain |
| Cặp điểm gần nhất | Divide-and-conquer hoặc sweep line |
| Giao nhiều sự kiện trục tọa độ | Sweep line + cấu trúc đoạn |

Ưu tiên số nguyên chính xác cho orientation và diện tích gấp đôi. Chỉ dùng
floating point khi đối tượng đầu vào thực sự yêu cầu.

## Xử lý offline

Xem nhóm [09 Offline Algorithms](../09_offline_algorithms/README.md) khi:

- toàn bộ query đã biết trước;
- có thể đổi thứ tự trả lời rồi phục hồi theo `id`;
- update có thể đưa lên trục thời gian;
- điều kiện query trở thành dominance hoặc sweep theo một tọa độ;
- bài online khó nhưng có thể chia để trị theo thời gian/giá trị.

Từ root repository, có thể tìm bằng:

```sh
python3 cp_library/tools/search.py "offline"
python3 cp_library/tools/search.py "rollback"
python3 cp_library/tools/search.py "sweep line"
python3 cp_library/tools/search.py --tag mo
python3 cp_library/tools/search.py --tag cdq
```

## Cần dựng nghiệm, không chỉ giá trị

- Lưu parent nếu tổng số state cho phép.
- Với DP lớn, cân nhắc truy vết lại bằng cách tính lại một phần.
- Với matching/flow, đọc nghiệm từ cạnh còn flow.
- Với greedy/DSU, lưu chính các quyết định đã thực hiện.
- Với chia để trị, đảm bảo thứ tự output được phục hồi bằng query id.

Một thuật toán tính đúng giá trị chưa tự động cung cấp cách dựng nghiệm trong
cùng độ phức tạp; kiểm tra yêu cầu output từ đầu.

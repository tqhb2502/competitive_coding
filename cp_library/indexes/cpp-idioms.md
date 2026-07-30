# Mẹo C++/STL hay quên khi thi

Không phải thuật toán, mà là những chi tiết ngôn ngữ/thư viện chuẩn hay làm mất điểm oan.

## Vào / ra

```cpp
ios::sync_with_stdio(false); cin.tie(nullptr);   // BẮT BUỘC nếu dùng cin/cout với dữ liệu lớn
cout << '\n';                                    // KHÔNG dùng endl trong vòng lặp (endl = flush, rất chậm)
printf("%lld\n", x);                             // long long là %lld, không phải %d
cout << fixed << setprecision(6) << x;           // in số thực đúng số chữ số đề yêu cầu
while (cin >> x) { ... }                         // đọc tới hết input
cin >> n; cin.ignore(); getline(cin, s);         // đọc DÒNG sau khi đã >> : phải bỏ ký tự '\n' còn lại
```

- Trộn `scanf/printf` với `cin/cout` sau khi `sync_with_stdio(false)` là **hỏng thứ tự** — chọn một kiểu.
- Bài tương tác: sau mỗi truy vấn phải `<< flush` (xem [templates/interactive.cpp](../templates/interactive.cpp)).

## Số học

```cpp
long long                                        // tổng 2e5 phần tử × 1e9 = 2e14 -> int TRÀN
(__int128)a * b % mod                            // mod > ~3e9; gần 2^64 thì (unsigned __int128)
(a + b - 1) / b                                  // ceil(a/b) — CHỈ đúng khi a, b > 0
-7 / 2 == -3, -7 % 2 == -1                       // C++ làm tròn VỀ 0, dư mang dấu của số bị chia
((x % m) + m) % m                                // chuẩn hóa số dư về [0, m)
```

- So sánh số thực: dùng `abs(a - b) < 1e-9`, hoặc tốt hơn — **giữ nguyên số nguyên** (kho hình học làm vậy).
- `pow` trả `double` → sai với số nguyên lớn; dùng lũy thừa nhanh ([modular-arithmetic](../07_mathematics/modular-arithmetic.hpp)).

## STL hay quên

```cpp
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());    // khử trùng: PHẢI sort trước
lower_bound(a.begin(), a.end(), x) - a.begin();  // chỉ đúng trên dãy ĐÃ SẮP
s.lower_bound(x);                                // với set/map: dùng hàm THÀNH VIÊN (O(log n))
nth_element(a.begin(), a.begin() + k, a.end());  // lấy phần tử thứ k: O(n), không cần sort hết
priority_queue<int, vector<int>, greater<>> pq;  // heap NHỎ nhất (mặc định là lớn nhất)
iota(p.begin(), p.end(), 0);                     // p = 0,1,2,...
accumulate(a.begin(), a.end(), 0LL);             // 0LL — nếu ghi 0 thì cộng dồn theo int -> TRÀN
do { ... } while (next_permutation(a.begin(), a.end()));   // duyệt hoán vị: phải sort tăng trước
__gcd(a, b), __builtin_popcountll(x), __builtin_ctzll(x), __lg(x)   // sẵn có trên GCC
bitset<1000> b; b.count(); b._Find_first();      // phép bit trên khối 64-bit, nhanh gấp ~64 lần
auto [x, y] = p;                                 // structured binding (C++17)
```

- **Comparator phải là thứ tự yếu nghiêm ngặt**: dùng `<`, tuyệt đối không `<=` (dùng `<=` làm `sort` sập).
- `unordered_map` bị đánh sập bằng test đối kháng (hash cố định) → dùng `map`, hoặc trộn seed ngẫu nhiên.
- `vector<bool>` là kiểu đặc biệt (bit-packed): cần mảng byte thật thì dùng `vector<char>`.
- Lấy tham chiếu vào `vector` rồi `push_back` → **tham chiếu treo** (vector realloc). Ghi nhớ khi cấp phát node.

## Bẫy nhiều test case

- Reset **mọi** trạng thái toàn cục giữa các test (mảng, biến đếm, cấu trúc dữ liệu) — lỗi kinh điển.
- `memset` lớn trong mỗi test → tổng chi phí bùng nổ; chỉ xóa phần đã dùng.

## Đệ quy & stack

- Đệ quy sâu > ~1e5 khung dễ tràn stack (Windows/judge hạn chế): các snippet trong kho đã dùng **DFS lặp**
  ở những chỗ nguy hiểm (`scc`, `bridges-articulation`, `euler-tour`, `hld`, `offline-lca`, `small-to-large`…).
- Lambda đệ quy: `function<void(int)> dfs = [&](int u){...};` (chậm hơn hàm thường ~2 lần, đủ dùng).

## Trước khi nộp

Xem [pitfalls.md](pitfalls.md) mục "14 bẫy chí tử" và "Quy trình 30 giây trước khi nộp".

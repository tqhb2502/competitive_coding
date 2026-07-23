# Danh mục

_Được sinh bởi `tools/build_index.py`; không chỉnh sửa thủ công._

| Mã | Tên | Nhóm | Trạng thái | Thẻ | Độ phức tạp |
| --- | --- | --- | --- | --- | --- |
| `binary-search` | [Binary Search trên miền số nguyên](../01_foundations/binary-search/README.md) | `01_foundations` | `stress-tested` | `binary-search`, `monotone-predicate`, `search-on-answer` | memory: O(1); predicate-calls: O(log(true_sentinel - false_sentinel)) |
| `coordinate-compression` | [Coordinate Compression](../01_foundations/coordinate-compression/README.md) | `01_foundations` | `stress-tested` | `binary-search`, `coordinate-compression`, `sorting` | build: O(n log n); lookup: O(log n); memory: O(n) |
| `difference-array` | [Difference Array](../01_foundations/difference-array/README.md) | `01_foundations` | `stress-tested` | `difference-array`, `offline`, `range-update` | build: O(n); materialize: O(n); memory: O(n); update: O(1) |
| `monotonic-deque` | [Monotonic Deque](../01_foundations/monotonic-deque/README.md) | `01_foundations` | `stress-tested` | `deque`, `monotonic`, `sliding-window`, `window-extrema` | memory: O(k) phụ trợ và O(n-k+1) output; time: O(n) amortized |
| `monotonic-stack` | [Monotonic Stack](../01_foundations/monotonic-stack/README.md) | `01_foundations` | `stress-tested` | `array`, `monotonic-stack`, `nearest-element` | memory: O(n); time: O(n) amortized |
| `prefix-sum` | [Prefix Sum](../01_foundations/prefix-sum/README.md) | `01_foundations` | `stress-tested` | `prefix-sum`, `range-query`, `static-array` | build: O(n); memory: O(n); query: O(1) |
| `sliding-window` | [Sliding Window](../01_foundations/sliding-window/README.md) | `01_foundations` | `stress-tested` | `array`, `callback`, `sliding-window`, `two-pointers` | memory: O(1) ngoài trạng thái do callback quản lý; time: O(n) callback calls |
| `submask-enumeration` | [Duyệt mọi submask](../01_foundations/submask-enumeration/README.md) | `01_foundations` | `stress-tested` | `bitmask`, `enumeration`, `submask` | memory: O(1) ngoài callback; time: O(2^popcount(mask)) |
| `two-pointers` | [Two Pointers](../01_foundations/two-pointers/README.md) | `01_foundations` | `stress-tested` | `array`, `counting`, `sorted-array`, `two-pointers` | memory: O(1); time: O(n) |

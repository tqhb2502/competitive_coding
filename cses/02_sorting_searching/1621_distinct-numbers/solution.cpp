#include <iostream>
#include <unordered_set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Hash set tự động loại bỏ các phần tử trùng lặp.
    unordered_set<long long> distinct;
    // Đặt trước dung lượng và hạ max_load_factor để giảm số lần rehash.
    distinct.reserve(static_cast<size_t>(n) * 2);
    distinct.max_load_factor(0.7F);

    // Đọc n số và đưa tất cả vào set.
    for (int i = 0; i < n; ++i) {
        long long value;
        cin >> value;
        distinct.insert(value);
    }

    // Kích thước của set chính là số giá trị phân biệt.
    cout << distinct.size() << '\n';
    return 0;
}

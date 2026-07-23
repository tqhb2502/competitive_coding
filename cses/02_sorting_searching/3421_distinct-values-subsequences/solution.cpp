#include <iostream>
#include <unordered_map>

using namespace std;

constexpr long long MODULO = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Đếm số lần xuất hiện của từng giá trị bằng hash map (cnt[v]).
    unordered_map<int, int> frequency;
    frequency.reserve(2 * n);
    frequency.max_load_factor(0.7F);
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        ++frequency[value];
    }

    // Nhân dồn tích (1 + cnt[v]) trên mọi giá trị phân biệt, theo modulo.
    long long answer = 1;
    for (const auto& [value, count] : frequency) {
        (void)value;
        answer = answer * (count + 1) % MODULO;
    }

    // Trừ 1 để loại bỏ dãy con rỗng; cộng MODULO tránh kết quả âm.
    cout << (answer + MODULO - 1) % MODULO << '\n';
    return 0;
}

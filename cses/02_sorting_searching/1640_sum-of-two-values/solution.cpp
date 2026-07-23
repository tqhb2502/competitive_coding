#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long target;
    cin >> n >> target;

    // Hash map ánh xạ: giá trị đã gặp -> vị trí (1-based) của phần tử đứng trước
    unordered_map<long long, int> seen;
    seen.reserve(static_cast<size_t>(n) * 2);
    seen.max_load_factor(0.7F);

    // Duyệt mảng một lượt từ trái sang phải
    for (int index = 1; index <= n; ++index) {
        long long value;
        cin >> value;

        // Nếu phần bổ sung (target - value) đã gặp trước đó thì in ra cặp vị trí
        const auto match = seen.find(target - value);
        if (match != seen.end()) {
            cout << match->second << ' ' << index << '\n';
            return 0;
        }

        // Ghi lại vị trí đầu tiên gặp giá trị này (không ghi đè nếu đã có)
        seen.emplace(value, index);
    }

    // Không tìm được cặp nào thỏa mãn
    cout << "IMPOSSIBLE\n";
    return 0;
}

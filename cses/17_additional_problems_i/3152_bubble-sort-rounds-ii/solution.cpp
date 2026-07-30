#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    vector<long long> values(static_cast<size_t>(n));
    for (long long& value : values) {
        cin >> value;
    }

    // Cửa sổ trượt: phần tử kết quả kế tiếp chỉ có thể đến từ min(n, k+1)
    // phần tử chưa lấy đầu tiên.
    int window = n;
    if (k < n) {
        window = static_cast<int>(k) + 1;
    }

    // Hàng đợi ưu tiên lưu cặp (giá trị, vị trí); vị trí giữ thứ tự ổn định
    // giữa các phần tử bằng nhau.
    using Element = pair<long long, int>;
    priority_queue<Element, vector<Element>, greater<Element>> candidates;
    for (int index = 0; index < window; ++index) {
        candidates.emplace(values[static_cast<size_t>(index)], index);
    }

    // Dựng dãy kết quả từ trái sang phải: mỗi bước lấy phần tử nhỏ nhất ra,
    // rồi đưa phần tử chưa xét kế tiếp vào cửa sổ.
    int nextIndex = window;
    bool first = true;
    while (!candidates.empty()) {
        auto [value, index] = candidates.top();
        candidates.pop();
        (void)index;

        if (!first) {
            cout << ' ';
        }
        cout << value;
        first = false;

        if (nextIndex < n) {
            candidates.emplace(values[static_cast<size_t>(nextIndex)], nextIndex);
            ++nextIndex;
        }
    }
    cout << '\n';
}

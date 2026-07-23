#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

using Integer = long long;
using State = pair<Integer, int>;  // (tổng của tập con, chỉ số phần tử lớn nhất)

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;
    int count;
    cin >> size >> count;
    vector<Integer> values(static_cast<size_t>(size));
    // Mọi số âm luôn có lợi: cộng vào base rồi thay phần tử bằng giá trị tuyệt đối.
    Integer base = 0;
    for (Integer &value : values) {
        cin >> value;
        if (value < 0) {
            base += value;
            value = -value;
        }
    }
    // Sắp xếp mảng không âm tăng dần để cây sinh tổng theo thứ tự không giảm.
    sort(values.begin(), values.end());

    // Min-heap liệt kê các tổng tập con không âm theo thứ tự tăng dần.
    priority_queue<State, vector<State>, greater<State>> queue;
    // Tổng nhỏ nhất là base (ứng với tập rỗng).
    cout << base;
    --count;
    if (count > 0) {
        queue.emplace(values[0], 0);
    }
    while (count-- > 0) {
        const auto [sum, index] = queue.top();
        queue.pop();
        cout << ' ' << base + sum;  // In tổng gốc = base + tổng tập con không âm.
        if (index + 1 < size) {
            // Giữ a[index] và thêm phần tử kế tiếp a[index+1].
            queue.emplace(sum + values[static_cast<size_t>(index + 1)], index + 1);
            // Thay a[index] bằng a[index+1].
            queue.emplace(sum - values[static_cast<size_t>(index)]
                              + values[static_cast<size_t>(index + 1)],
                          index + 1);
        }
    }
    cout << '\n';
}

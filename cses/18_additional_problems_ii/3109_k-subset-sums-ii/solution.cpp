#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

using Integer = long long;

// Một trạng thái ứng với một phân hoạch: tổng hiện tại, số phần dương (parts),
// giá trị phần cuối (last) và giá trị phần áp cuối (previous).
struct State {
    Integer sum;
    int parts;
    int last;
    int previous;

    // Heap nhỏ nhất theo tổng.
    bool operator>(const State &other) const {
        return sum > other.sum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;        // n: số phần tử
    int subsetSize;  // m: kích thước mỗi tập con
    int count;       // k: số tổng cần liệt kê
    cin >> size >> subsetSize >> count;
    vector<Integer> values(static_cast<size_t>(size));
    for (Integer &value : values) {
        cin >> value;
    }
    // Sắp xếp tăng dần để mọi độ tăng trên cạnh của cây phân hoạch đều không âm.
    sort(values.begin(), values.end());

    // Tập cơ sở là m phần tử nhỏ nhất, cho tổng nhỏ nhất.
    Integer initial = 0;
    for (int index = 0; index < subsetSize; ++index) {
        initial += values[static_cast<size_t>(index)];
    }

    priority_queue<State, vector<State>, greater<State>> queue;
    queue.push({initial, 0, 0, 0});
    for (int answer = 0; answer < count; ++answer) {
        // Lấy trạng thái có tổng nhỏ nhất và in ra.
        const State current = queue.top();
        queue.pop();
        if (answer != 0) {
            cout << ' ';
        }
        cout << current.sum;

        // Con 1: nối thêm một phần bằng 1 (nếu chưa đủ m phần dương).
        if (current.parts < subsetSize) {
            const int index = subsetSize - current.parts - 1;
            queue.push({current.sum + values[static_cast<size_t>(index + 1)]
                                      - values[static_cast<size_t>(index)],
                        current.parts + 1, 1, current.last});
        }

        // Con 2: tăng phần cuối lên 1 nếu không đè lên phần áp cuối và chỉ số
        // vẫn còn nằm trong mảng.
        if (current.parts > 0
            && (current.parts == 1 || current.last < current.previous)) {
            const int baseIndex = subsetSize - current.parts;
            const int valueIndex = baseIndex + current.last;
            if (valueIndex + 1 < size) {
                queue.push({current.sum + values[static_cast<size_t>(valueIndex + 1)]
                                          - values[static_cast<size_t>(valueIndex)],
                            current.parts, current.last + 1, current.previous});
            }
        }
    }
    cout << '\n';
}

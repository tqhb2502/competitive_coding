#include <iostream>
#include <queue>
#include <vector>

using namespace std;

using Integer = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int size;
    cin >> size;

    // Max-heap lưu các mức đại diện cho nghiệm tối ưu của tiền tố (slope trick / PAVA)
    priority_queue<Integer> levels;
    Integer answer = 0;

    for (int index = 0; index < size; ++index) {
        Integer value;
        cin >> value;

        // Đưa giá trị hiện tại vào heap
        levels.push(value);

        // Nếu đỉnh heap lớn hơn giá trị hiện tại thì thứ tự bị vi phạm:
        // cộng chi phí bắt buộc, gộp khối và điều chỉnh trung vị của khối gộp
        if (levels.top() > value) {
            answer += levels.top() - value;
            levels.pop();
            levels.push(value);
        }
    }

    cout << answer << '\n';
}

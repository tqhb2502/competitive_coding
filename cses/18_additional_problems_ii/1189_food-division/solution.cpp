#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

using Integer = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int children;
    cin >> children;

    // difference[i] = lượng hiện có - lượng mong muốn của trẻ thứ i
    vector<Integer> difference(static_cast<size_t>(children));
    for (Integer &value : difference) {
        cin >> value;
    }
    for (Integer &value : difference) {
        Integer wanted;
        cin >> wanted;
        value -= wanted;
    }

    // Tổng tiền tố: prefix[i] = số đơn vị thức ăn phải chuyển qua cạnh thứ i
    // (đây chính là các giá trị s_i cần chọn trung vị)
    vector<Integer> prefix(static_cast<size_t>(children));
    Integer running = 0;
    for (int index = 0; index < children; ++index) {
        running += difference[static_cast<size_t>(index)];
        prefix[static_cast<size_t>(index)] = running;
    }

    // Trung vị làm cho tổng khoảng cách tuyệt đối nhỏ nhất -> số bước ít nhất
    nth_element(prefix.begin(), prefix.begin() + children / 2, prefix.end());
    const Integer median = prefix[static_cast<size_t>(children / 2)];

    // Đáp án: tổng khoảng cách tuyệt đối từ mỗi s_i tới trung vị
    Integer answer = 0;
    for (const Integer value : prefix) {
        answer += llabs(value - median);
    }
    cout << answer << '\n';
}

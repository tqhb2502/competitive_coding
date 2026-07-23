#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // position[x] = vị trí của giá trị x trong mảng thứ nhất (hoán vị của 1..n)
    vector<int> position(n + 1, -1);
    for (int i = 0; i < n; ++i) {
        int value;
        cin >> value;
        position[value] = i;
    }

    // Duyệt mảng thứ hai; chỉ giữ giá trị cũng có ở mảng thứ nhất (x <= n) và
    // thay bằng vị trí của nó. LCS của hai hoán vị trở thành LIS trên dãy vị trí.
    vector<pair<int, int>> sequence;  // (vị trí trong mảng một, giá trị gốc)
    sequence.reserve(min(n, m));
    for (int i = 0; i < m; ++i) {
        int value;
        cin >> value;
        if (value <= n) {
            sequence.emplace_back(position[value], value);
        }
    }

    // Patience sorting tìm LIS chặt tăng theo thành phần vị trí.
    const int length = static_cast<int>(sequence.size());
    vector<int> predecessor(length, -1);  // truy vết phần tử liền trước trong LIS
    vector<int> tailValue;                // đuôi nhỏ nhất của LIS cho mỗi độ dài
    vector<int> tailIndex;                // chỉ số phần tử tạo ra mỗi đuôi
    tailValue.reserve(length);
    tailIndex.reserve(length);

    for (int i = 0; i < length; ++i) {
        const int current = sequence[i].first;
        // Tìm vị trí chèn: đuôi đầu tiên >= current sẽ bị thay thế.
        const int place = static_cast<int>(
            lower_bound(tailValue.begin(), tailValue.end(), current) - tailValue.begin());
        if (place > 0) {
            predecessor[i] = tailIndex[place - 1];
        }
        if (place == static_cast<int>(tailValue.size())) {
            tailValue.push_back(current);  // kéo dài LIS thêm một đơn vị
            tailIndex.push_back(i);
        } else {
            tailValue[place] = current;    // hạ đuôi để mở rộng về sau
            tailIndex[place] = i;
        }
    }

    // Truy vết ngược từ đuôi của LIS dài nhất rồi đảo lại để đúng thứ tự.
    vector<int> answer;
    int currentIndex = tailIndex.back();
    while (currentIndex != -1) {
        answer.push_back(sequence[currentIndex].second);
        currentIndex = predecessor[currentIndex];
    }
    reverse(answer.begin(), answer.end());

    cout << answer.size() << '\n';
    for (int value : answer) {
        cout << value << ' ';
    }
    cout << '\n';
}

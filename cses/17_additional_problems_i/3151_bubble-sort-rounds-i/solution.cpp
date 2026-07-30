#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Lưu mỗi phần tử kèm vị trí ban đầu để giữ đúng thứ tự các giá trị bằng nhau.
    vector<pair<long long, int>> elements;
    elements.reserve(static_cast<size_t>(n));
    for (int position = 0; position < n; ++position) {
        long long value;
        cin >> value;
        elements.emplace_back(value, position);
    }

    // Sắp xếp theo (giá trị, vị trí ban đầu) để biết vị trí đích của mỗi phần tử.
    sort(elements.begin(), elements.end());

    // Mỗi vòng bubble sort chỉ dịch một phần tử sang trái tối đa một ô, nên đáp án
    // là khoảng cách dịch trái lớn nhất = max(vị trí ban đầu - vị trí đích).
    int rounds = 0;
    for (int sortedPosition = 0; sortedPosition < n; ++sortedPosition) {
        rounds = max(rounds,
                     elements[static_cast<size_t>(sortedPosition)].second - sortedPosition);
    }

    cout << rounds << '\n';
}

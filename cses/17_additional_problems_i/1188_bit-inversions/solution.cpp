#include <iostream>
#include <iterator>
#include <set>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string bits;
    cin >> bits;
    const int n = static_cast<int>(bits.size());

    // Tập các biên giữa hai đoạn đồng nhất: luôn có 0 và n, thêm i khi s[i-1] khác s[i]
    set<int> boundaries{0, n};
    for (int i = 1; i < n; ++i) {
        if (bits[i - 1] != bits[i]) {
            boundaries.insert(i);
        }
    }

    // Multiset lưu độ dài mọi đoạn giữa hai biên liên tiếp; phần tử lớn nhất là đáp án
    multiset<int> lengths;
    for (auto it = next(boundaries.begin()); it != boundaries.end(); ++it) {
        lengths.insert(*it - *prev(it));
    }

    // Xóa đúng một bản sao của một độ dài khỏi multiset
    const auto eraseOneLength = [&lengths](int value) {
        lengths.erase(lengths.find(value));
    };

    // Toggle một biên: nếu chưa có thì thêm và tách đoạn thành hai;
    // nếu đã có thì xóa và ghép hai đoạn kề thành một
    const auto toggleBoundary = [&boundaries, &lengths, &eraseOneLength](int point) {
        const auto found = boundaries.find(point);
        if (found == boundaries.end()) {
            // Thêm biên: tách đoạn [left, right] thành [left, point] và [point, right]
            const auto rightIterator = boundaries.upper_bound(point);
            const int right = *rightIterator;
            const int left = *prev(rightIterator);
            eraseOneLength(right - left);
            lengths.insert(point - left);
            lengths.insert(right - point);
            boundaries.insert(point);
        } else {
            // Xóa biên: ghép [left, point] và [point, right] thành [left, right]
            const int left = *prev(found);
            const int right = *next(found);
            eraseOneLength(point - left);
            eraseOneLength(right - point);
            lengths.insert(right - left);
            boundaries.erase(found);
        }
    };

    int changes;
    cin >> changes;
    for (int query = 0; query < changes; ++query) {
        int position;
        cin >> position;
        --position;
        // Đảo bit tại position chỉ ảnh hưởng trạng thái của hai biên position và position+1
        if (position > 0) {
            toggleBoundary(position);
        }
        if (position + 1 < n) {
            toggleBoundary(position + 1);
        }
        // In độ dài đoạn đồng nhất dài nhất hiện tại
        cout << *lengths.rbegin() << (query + 1 == changes ? '\n' : ' ');
    }
}

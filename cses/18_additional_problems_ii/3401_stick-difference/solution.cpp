#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>

using namespace std;

// Trạng thái trong heap: giá trị hiện tại (cận trên hoặc cận dưới của một
// thanh), chỉ số thanh và số mảnh đang chia của thanh đó.
struct HeapState {
    int value;
    int stick;
    int pieces;

    bool operator<(const HeapState& other) const {
        if (value != other.value) {
            return value < other.value;
        }
        return stick < other.stick;
    }
};

// Một thanh "xấu" trên đoạn các k trong [start, finish]: khoảng số đoạn của nó
// bị rỗng, cần sửa bằng hạ cận dưới xuống lowerRepair hoặc tăng cận trên lên
// upperRepair. coordinate là chỉ số nén của lowerRepair.
struct BadInterval {
    int start;
    int finish;
    int lowerRepair;
    int upperRepair;
    int coordinate = -1;
};

// Segment tree theo toạ độ lo (đã nén). Mỗi node lưu hi lớn nhất và lo hoạt
// động lớn nhất, hỗ trợ truy vấn quét prefix maximum để tính đáp án cho một k.
class PrefixMaximumTree {
public:
    explicit PrefixMaximumTree(const vector<int>& coordinates)
        : coordinates_(coordinates),
          maximumHigh_(coordinates.size() * 4U, negativeInfinity()),
          maximumActiveLow_(coordinates.size() * 4U, negativeInfinity()) {}

    // Cập nhật hi lớn nhất tại toạ độ index (negativeInfinity nghĩa là rỗng).
    void update(int index, int high) {
        update(1, 0, static_cast<int>(coordinates_.size()) - 1, index, high);
    }

    // Trả về độ rộng khả thi nhỏ nhất cho một k, với cận trên ban đầu
    // initialMaximum (= R) và cận dưới cuối cùng finalLower (= L).
    long long query(int initialMaximum, int finalLower) const {
        // Không có thanh xấu nào: đáp án đơn giản là R - L.
        if (coordinates_.empty() || maximumActiveLow_[1] == negativeInfinity()) {
            return static_cast<long long>(initialMaximum) - finalLower;
        }
        int prefixMaximum = initialMaximum;
        long long answer = numeric_limits<long long>::max();
        process(1, 0, static_cast<int>(coordinates_.size()) - 1,
                prefixMaximum, answer);
        // Trường hợp cuối: sửa mọi thanh còn lại bằng phía dưới (dùng L).
        answer = min(answer,
                     static_cast<long long>(prefixMaximum) - finalLower);
        return answer;
    }

private:
    static constexpr int negativeInfinity() {
        return numeric_limits<int>::min() / 4;
    }

    void update(int node, int left, int right, int index, int high) {
        if (left == right) {
            maximumHigh_[node] = high;
            maximumActiveLow_[node] =
                high == negativeInfinity() ? negativeInfinity()
                                           : coordinates_[left];
            return;
        }
        const int middle = left + (right - left) / 2;
        if (index <= middle) {
            update(node * 2, left, middle, index, high);
        } else {
            update(node * 2 + 1, middle + 1, right, index, high);
        }
        maximumHigh_[node] =
            max(maximumHigh_[node * 2], maximumHigh_[node * 2 + 1]);
        maximumActiveLow_[node] = max(maximumActiveLow_[node * 2],
                                      maximumActiveLow_[node * 2 + 1]);
    }

    // Quét các node theo thứ tự lo tăng dần, duy trì prefixMaximum của hi.
    void process(int node, int left, int right, int& prefixMaximum,
                 long long& answer) const {
        // Node rỗng thì bỏ qua.
        if (maximumActiveLow_[node] == negativeInfinity()) {
            return;
        }
        // Cắt tỉa: không lo nào trong node cải thiện được đáp án hiện có, chỉ
        // cần gộp maximum hi của node rồi bỏ qua.
        if (answer != numeric_limits<long long>::max() &&
            static_cast<long long>(prefixMaximum) -
                    maximumActiveLow_[node] >=
                answer) {
            prefixMaximum = max(prefixMaximum, maximumHigh_[node]);
            return;
        }
        // Cả node không làm tăng prefixMaximum: ứng viên tốt nhất dùng lo lớn
        // nhất của node, bỏ qua cả node.
        if (maximumHigh_[node] <= prefixMaximum) {
            answer = min(answer,
                         static_cast<long long>(prefixMaximum) -
                             maximumActiveLow_[node]);
            return;
        }
        if (left == right) {
            answer = min(answer,
                         static_cast<long long>(prefixMaximum) -
                             coordinates_[left]);
            prefixMaximum = max(prefixMaximum, maximumHigh_[node]);
            return;
        }
        // Chưa thoả điều kiện cắt tỉa: đi xuống hai con để cập nhật mốc prefix.
        const int middle = left + (right - left) / 2;
        process(node * 2, left, middle, prefixMaximum, answer);
        process(node * 2 + 1, middle + 1, right, prefixMaximum, answer);
    }

    const vector<int>& coordinates_;
    vector<int> maximumHigh_;
    vector<int> maximumActiveLow_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int stickCount = 0;
    int maximumCuts = 0;
    cin >> stickCount >> maximumCuts;
    vector<int> length(stickCount);
    for (int& value : length) {
        cin >> value;
    }

    // minimumUpper[k] = R nhỏ nhất sao cho tổng ceil(a_i/R) <= n+k. Dùng heap
    // lấy max để mỗi bước tăng số mảnh của thanh có ceil(a/q) lớn nhất.
    vector<int> minimumUpper(maximumCuts + 1, 0);
    priority_queue<HeapState> upperQueue;
    for (int stick = 0; stick < stickCount; ++stick) {
        upperQueue.push({length[stick], stick, 1});
    }
    minimumUpper[0] = upperQueue.top().value;
    for (int cuts = 1; cuts <= maximumCuts; ++cuts) {
        HeapState state = upperQueue.top();
        upperQueue.pop();
        ++state.pieces;
        state.value =
            (length[state.stick] + state.pieces - 1) / state.pieces;
        upperQueue.push(state);
        minimumUpper[cuts] = upperQueue.top().value;
    }

    // maximumLower[k] = L lớn nhất sao cho tổng floor(a_i/L) >= n+k, đồng thời
    // giữ L <= min_i a_i. Trộn các dãy floor(a/2), floor(a/3), ... lấy dần max.
    vector<int> maximumLower(maximumCuts + 1, 0);
    priority_queue<HeapState> lowerQueue;
    maximumLower[0] = *min_element(length.begin(), length.end());
    for (int stick = 0; stick < stickCount; ++stick) {
        lowerQueue.push({length[stick] / 2, stick, 2});
    }
    for (int cuts = 1; cuts <= maximumCuts; ++cuts) {
        HeapState state = lowerQueue.top();
        lowerQueue.pop();
        maximumLower[cuts] = min(maximumLower[cuts - 1], state.value);
        ++state.pieces;
        state.value = length[state.stick] / state.pieces;
        lowerQueue.push(state);
    }

    // Tìm k nhỏ nhất trong [0, maximumCuts] thoả predicate (binary search).
    const auto firstIndex = [maximumCuts](const function<bool(int)>& predicate) {
        int left = 0;
        int right = maximumCuts + 1;
        while (left < right) {
            const int middle = left + (right - left) / 2;
            if (middle <= maximumCuts && predicate(middle)) {
                right = middle;
            } else {
                left = middle + 1;
            }
        }
        return left;
    };

    vector<BadInterval> intervals;
    intervals.reserve(maximumCuts);
    vector<int> coordinates;
    coordinates.reserve(maximumCuts);

    // Với mỗi thanh và mỗi x = pieces cố định, tìm đoạn liên tiếp các k mà thanh
    // bị xấu (floor(a/L)=x và ceil(a/R)=x+1) rồi ghi lại khoảng cần sửa.
    for (const int value : length) {
        const int finalRequiredPieces =
            (value + minimumUpper[maximumCuts] - 1) /
            minimumUpper[maximumCuts];
        for (int pieces = 1; pieces < finalRequiredPieces; ++pieces) {
            // Hai đầu đoạn k mà floor(a/L[k]) = pieces.
            const int firstLower = firstIndex([&](int cuts) {
                return value / maximumLower[cuts] >= pieces;
            });
            const int afterLower = firstIndex([&](int cuts) {
                return value / maximumLower[cuts] >= pieces + 1;
            });
            // Hai đầu đoạn k mà ceil(a/R[k]) = pieces + 1.
            const int firstUpper = firstIndex([&](int cuts) {
                return (value + minimumUpper[cuts] - 1) /
                           minimumUpper[cuts] >=
                       pieces + 1;
            });
            const int afterUpper = firstIndex([&](int cuts) {
                return (value + minimumUpper[cuts] - 1) /
                           minimumUpper[cuts] >=
                       pieces + 2;
            });

            // Giao hai đoạn để lấy khoảng k mà thanh thực sự xấu.
            const int start = max({1, firstLower, firstUpper});
            const int finish =
                min({maximumCuts, afterLower - 1, afterUpper - 1});
            if (start > finish) {
                continue;
            }
            if (value / maximumLower[start] != pieces ||
                (value + minimumUpper[start] - 1) /
                        minimumUpper[start] !=
                    pieces + 1) {
                continue;
            }

            // Hai lựa chọn sửa: hạ cận dưới hoặc tăng cận trên.
            const int lowerRepair = value / (pieces + 1);
            const int upperRepair = (value + pieces - 1) / pieces;
            intervals.push_back(
                {start, finish, lowerRepair, upperRepair, -1});
            coordinates.push_back(lowerRepair);
        }
    }

    // Nén toạ độ các giá trị lo.
    sort(coordinates.begin(), coordinates.end());
    coordinates.erase(unique(coordinates.begin(), coordinates.end()),
                      coordinates.end());

    // Sự kiện thêm/xóa mỗi khoảng xấu theo đoạn thời gian [start, finish].
    vector<vector<int>> additions(maximumCuts + 2);
    vector<vector<int>> removals(maximumCuts + 2);
    for (int index = 0; index < static_cast<int>(intervals.size()); ++index) {
        BadInterval& interval = intervals[index];
        interval.coordinate = static_cast<int>(
            lower_bound(coordinates.begin(), coordinates.end(),
                        interval.lowerRepair) -
            coordinates.begin());
        additions[interval.start].push_back(index);
        removals[interval.finish + 1].push_back(index);
    }

    // Quét k từ 1..m: mỗi toạ độ giữ multiset các upperRepair đang hoạt động,
    // segment tree lưu hi lớn nhất tại toạ độ đó rồi truy vấn đáp án cho k.
    PrefixMaximumTree tree(coordinates);
    vector<multiset<int>> activeHigh(coordinates.size());
    constexpr int NEGATIVE_INFINITY = numeric_limits<int>::min() / 4;

    for (int cuts = 1; cuts <= maximumCuts; ++cuts) {
        // Xóa các khoảng hết hạn tại k này.
        for (const int index : removals[cuts]) {
            const BadInterval& interval = intervals[index];
            auto& values = activeHigh[interval.coordinate];
            const auto iterator = values.find(interval.upperRepair);
            values.erase(iterator);
            tree.update(interval.coordinate,
                        values.empty() ? NEGATIVE_INFINITY : *values.rbegin());
        }
        // Thêm các khoảng bắt đầu tại k này.
        for (const int index : additions[cuts]) {
            const BadInterval& interval = intervals[index];
            auto& values = activeHigh[interval.coordinate];
            values.insert(interval.upperRepair);
            tree.update(interval.coordinate, *values.rbegin());
        }

        cout << tree.query(minimumUpper[cuts], maximumLower[cuts])
             << (cuts == maximumCuts ? '\n' : ' ');
    }
}

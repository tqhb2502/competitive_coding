#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

struct Point {
    int x;
    int y;
};

// Fenwick tree lưu giá trị lớn nhất theo tiền tố (prefix max)
class FenwickMaximum {
public:
    explicit FenwickMaximum(int size) : tree_(static_cast<size_t>(size) + 1, NEGATIVE_INFINITY) {}

    void update(int index, int value) {
        for (int position = index + 1; position < static_cast<int>(tree_.size());
             position += position & -position) {
            tree_[static_cast<size_t>(position)] =
                max(tree_[static_cast<size_t>(position)], value);
        }
    }

    int queryPrefix(int count) const {
        int result = NEGATIVE_INFINITY;
        for (int position = count; position > 0; position -= position & -position) {
            result = max(result, tree_[static_cast<size_t>(position)]);
        }
        return result;
    }

    static constexpr int NEGATIVE_INFINITY = -1'000'000'000;

private:
    vector<int> tree_;
};

// Xử lý một góc phần tư ứng với hướng dấu (signX, signY).
// Biến đổi mọi điểm thành X = signX*x, Y = signY*y; trong góc này khoảng cách
// Manhattan bằng X_query + Y_query - (X_site + Y_site), nên cần tìm điểm trại có
// X_site + Y_site lớn nhất thỏa X_site <= X_query và Y_site <= Y_query.
void processQuadrant(const vector<Point>& reserved, const vector<Point>& freeCampsites,
                     int signX, int signY, vector<int>& answers) {
    vector<int> reservedOrder(reserved.size());
    vector<int> queryOrder(freeCampsites.size());
    iota(reservedOrder.begin(), reservedOrder.end(), 0);
    iota(queryOrder.begin(), queryOrder.end(), 0);

    auto reservedX = [&](int index) {
        return signX * reserved[static_cast<size_t>(index)].x;
    };
    auto queryX = [&](int index) {
        return signX * freeCampsites[static_cast<size_t>(index)].x;
    };
    // Sắp xếp điểm trại và truy vấn theo X tăng dần để quét theo thứ tự
    sort(reservedOrder.begin(), reservedOrder.end(),
         [&](int first, int second) { return reservedX(first) < reservedX(second); });
    sort(queryOrder.begin(), queryOrder.end(),
         [&](int first, int second) { return queryX(first) < queryX(second); });

    // Nén tọa độ Y của các điểm trại để đánh chỉ số cho Fenwick tree
    vector<int> yCoordinates;
    yCoordinates.reserve(reserved.size());
    for (const Point& point : reserved) {
        yCoordinates.push_back(signY * point.y);
    }
    sort(yCoordinates.begin(), yCoordinates.end());
    yCoordinates.erase(unique(yCoordinates.begin(), yCoordinates.end()), yCoordinates.end());

    FenwickMaximum fenwick(static_cast<int>(yCoordinates.size()));
    size_t added = 0;
    for (int queryIndex : queryOrder) {
        int transformedQueryX = signX * freeCampsites[static_cast<size_t>(queryIndex)].x;
        int transformedQueryY = signY * freeCampsites[static_cast<size_t>(queryIndex)].y;

        // Thêm mọi điểm trại có X_site <= X_query vào Fenwick tree
        while (added < reservedOrder.size()
               && reservedX(reservedOrder[added]) <= transformedQueryX) {
            int siteIndex = reservedOrder[added];
            int transformedSiteY = signY * reserved[static_cast<size_t>(siteIndex)].y;
            int compressed = static_cast<int>(lower_bound(yCoordinates.begin(), yCoordinates.end(),
                                                          transformedSiteY)
                                              - yCoordinates.begin());
            fenwick.update(compressed, reservedX(siteIndex) + transformedSiteY);
            ++added;
        }

        // Truy vấn prefix trên Y <= Y_query để lấy X_site + Y_site lớn nhất
        int count = static_cast<int>(upper_bound(yCoordinates.begin(), yCoordinates.end(),
                                                 transformedQueryY)
                                     - yCoordinates.begin());
        int bestSiteSum = fenwick.queryPrefix(count);
        if (bestSiteSum != FenwickMaximum::NEGATIVE_INFINITY) {
            // Lấy min qua các góc để có khoảng cách gần nhất của điểm trống
            answers[static_cast<size_t>(queryIndex)] =
                min(answers[static_cast<size_t>(queryIndex)],
                    transformedQueryX + transformedQueryY - bestSiteSum);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    int m;
    cin >> n >> m;
    vector<Point> reserved(static_cast<size_t>(n));
    vector<Point> freeCampsites(static_cast<size_t>(m));
    for (Point& point : reserved) {
        cin >> point.x >> point.y;
    }
    for (Point& point : freeCampsites) {
        cin >> point.x >> point.y;
    }

    // Quét đủ bốn hướng dấu; mỗi cặp điểm luôn thuộc ít nhất một góc
    vector<int> answers(static_cast<size_t>(m), INT_MAX);
    for (int signX : {-1, 1}) {
        for (int signY : {-1, 1}) {
            processQuadrant(reserved, freeCampsites, signX, signY, answers);
        }
    }

    // Đáp án là khoảng cách lớn nhất trên tất cả các điểm trại trống
    cout << *max_element(answers.begin(), answers.end()) << '\n';
}
